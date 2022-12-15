#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <wiringPi.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <math.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include "value_define.h"

//Sonic
#define TRIG 15
#define ECHO 18

//Gyro
#define CS_GPIO 8 //CS 핀 설정
#define SPI_CH 0
#define SPI_SPEED 1000000 // 1MHz
#define SPI_MODE 3
#define BW_RATE 0x2C //통신 속도 설정
#define POWER_CTL 0x2D //Power Control Register
#define DATA_FORMAT 0x31
#define DATAX0 0x32 //X-Axis Data 0

//UART2(Bluetooth)
#define BAUD_RATE 9600
static const char* UART2_DEV = "/dev/ttyAMA1";

int mode = 0;
int model_no = 0;

//메시지 큐 식별자
int msg_q;

//ADXL345에서 값을 읽는 함수
void readRegister_ADXL345(char registerAddress, int numBytes, char * values){
    //read 기능을 시작하기 위해서는 주소의 최상위 비트가 1로 설정되어야 함.
    values[0] = 0x80 | registerAddress;
    // 멀티 바이트 읽기를 사용하는 경우 6번 비트도 1로 설정
    if(numBytes > 1) values[0] = values[0] | 0x40;
    digitalWrite(CS_GPIO, LOW); // Low : CS Active
    // 데이터 읽고 values에 저장
    wiringPiSPIDataRW(SPI_CH, values, numBytes + 1);
    digitalWrite(CS_GPIO, HIGH); // High : CS Inactive
}

//ADXL345에 값을 쓰는 함수
void writeRegister_ADXL345(char address, char value)
{
    unsigned char buff[2];
    buff[0] = address;
    buff[1] = value;

    digitalWrite(CS_GPIO, LOW); // Low : CS Active
    wiringPiSPIDataRW(SPI_CH, buff, 2);
    digitalWrite(CS_GPIO, HIGH); // High : CS Inactive
}

float getSonicSens() { //Get distance with Sonic (Zoom)
    float distance, start, stop;

    digitalWrite(TRIG,LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG,LOW);

    while ( digitalRead(ECHO) == LOW );
    start = micros();

    while ( digitalRead(ECHO) == HIGH );
    stop = micros();

    distance = (float)(stop - start) / 5.882;

    return distance;
}

float getRotateSens(int angle) {
    unsigned char buffer[100];
    short x, y= 0 , z= 0;

    if(wiringPiSPISetupMode(SPI_CH, SPI_SPEED,SPI_MODE) == -1) return 1;

    pinMode(CS_GPIO, OUTPUT); //Chip Select로 사용할 핀은 OUTPUT으로 설정
    digitalWrite(CS_GPIO, HIGH); //IDLE 상태로 유지

    writeRegister_ADXL345(DATA_FORMAT,0x09); //범위 설정 +- 4G
    writeRegister_ADXL345(BW_RATE,0x0C); //Output Data Rage 400 Hz
    writeRegister_ADXL345(POWER_CTL,0x08); //측정 모드

    double a, b, c;
    readRegister_ADXL345(DATAX0,6,buffer); //데이터수신

    x = ((short)buffer[2]<<8)|(short)buffer[1]; //X축 값 변환
    y = ((short)buffer[4]<<8)|(short)buffer[3]; //Y축 값 변환
    z = ((short)buffer[6]<<8)|(short)buffer[5]; //Z축 값 변환
    a = x * 3.9 * 0.001*9.81;
    b = y * 3.9 * 0.001*9.81;
    c = z * 3.9 * 0.001*9.81;
    float roll = 180*atan(b/sqrt(a*a+c*c))/M_PI;
    float pitch = 180*atan(a/sqrt(b*b+c*c))/M_PI;
    if(angle == 1) return roll;
    else if(angle == 2) return pitch;
    else return 0;
}

void getBluetooth() { //Read value with UART (Zoom, Rotate)
    int fd_serial;
    int pitch, roll, yaw, zoom;
    
    char ReadBuf[20] = "";
    if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0){
        printf ("Unable to open serial device.\n") ;
        return;
    }
    if (serialDataAvail(fd_serial) < 0) {
        return;
    }

    unsigned char x;
    int idx = 0;

    while (idx != -1) {
        if(read (fd_serial, &x, 1) != 1) return -1; //serialRead
        fflush (stdout); //?

        if (x == 'A') {
            pitch = atoi(ReadBuf) * 100;
            sendData(1, pitch);
            ReadBuf[0] = '\0';
            idx = 0;
        } else if (x == 'B') {
            roll = atoi(ReadBuf) * 100;
            sendData(2, roll);
            ReadBuf[0] = '\0';
            idx = 0;
        } else if (x == 'C') {
            yaw = atoi(ReadBuf) * 100;
            sendData(3, yaw);
            ReadBuf[0] = '\0';
            idx = 0;
        } else if (x == 'D') {
            zoom = atoi(ReadBuf) * 100;
            sendData(0, zoom);
            idx = -1;
        } else {
            ReadBuf[idx] = x;
            idx++;
        }
    }
}

bool sendData(int type, float data1) {
    //value를 다른프로세스로 보냄
    msg_q = msgget((key_t)MSG_Q_KEY, IPC_CREAT | 0666);
    if (msg_q == -1) {
        perror("msgget()");
        return -1;
    }
    struct DataType dataType;
    memset(&dataType, 0, sizeof(dataType));
    dataType.type = SendType_Data;
    dataType.data_Type = type;
    dataType.data = data1;

    if (msgsnd(msg_q, &dataType, sizeof(dataType) - sizeof(long), IPC_NOWAIT) < 0) {
        perror("msgsnd()");
    }

    sleep(1);
    return true;
}

void *func_thread() {
    int zoom, roll, pitch;

    while(mode != -1) {
        if(mode == 0) {
            zoom = (int)getSonicSens() * 100;
            sendData(0, zoom);
            roll = (int)getRotateSens(1) * 100;
            sendData(1, roll);
            pitch = (int)getRotateSens(2) * 100;
            sendData(2, pitch);
        }
        else if(mode == 1) {
            getBluetooth();
        }
    }
}


int main() {
    
    if(wiringPiSetupGpio() < 0 ){
        printf("wiringPiSetup() is failed\n");
        return 1;
    } //wiringPi(GPIO)

    pinMode(TRIG, OUTPUT); //Sonic
    pinMode(ECHO, INPUT); //Sonic

    msg_q = msgget((key_t)MSG_Q_KEY, IPC_CREAT | 0666);
    if (msg_q == -1) {
        perror("msgget()");
        return -1;
    }

    struct ModelType modelType;

    pthread_t thread;
    pthread_create(&thread, NULL, func_thread, NULL);
   
    /*while(1) {
        printf("Distance : %2.2f mm \n",getSonicSens());
        delay(1000);
    }*/ //test sonic
    
    /*while(1) {
        printf("%f %f \n",getRotateSens(1), getRotateSens(2));
        delay(1000);
    }*/ //test Rotate

    char ch;
    while(ch != 'q') {
        system("clear");
        printf("Input Mode : %s \n", (mode == 0) ? "Wired Sensor" : "Bluetooth");
        printf("Model No.%d \n", model_no);
        printf("─────────────────────────────────────── \n");
        printf("M : Toggle Mode, 0~9 : Select Model \n");
        printf("─────────────────────────────────────── \n");
        printf("Select Menu : ");
        ch = getchar();
        
        if (ch == 'q') {
            mode = -1;
            continue;
        }
        else if (ch == 'm') {
            mode = 1 - mode;
        }
        else if ('0' <= ch && ch <= '9') {
            model_no = ch - '0';
            // Send Model No To Rendering Program 
            memset(&modelType, 0, sizeof(modelType));
            modelType.type = SendType_Model;
            modelType.model = model_no;
            if (msgsnd(msg_q, &modelType, sizeof(modelType) - sizeof(long), IPC_NOWAIT) < 0) {
                perror("msgsnd()");
            }
            sleep(1);
        }
    }

    pthread_join(thread, NULL);
    return 0;
}
