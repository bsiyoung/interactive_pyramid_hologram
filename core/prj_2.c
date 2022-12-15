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
mqd_t msg_q;
const char* name = "/posix_mq";
char buf[BUFSIZ];
char type[BUFSIZ];

#define TYPE_QUIT           0
#define TYPE_MODEL_SELECT   1
#define TYPE_ZOOM           2
#define TYPE_YAW            3
#define TYPE_PITCH          4
#define TYPE_ROLL           5

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

float getBluetooth() { //Read value with UART (Zoom, Rotate)
    int fd_serial;
    if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0){
        printf ("Unable to open serial device.\n") ;
        return 1 ;
    }

    unsigned char x;
    if(read (fd_serial, &x, 1) != 1) return -1;

    return (float)x; //받는 데이터 정의 필요
}

bool sendData(int type, int data) {
    if (msg_q == NULL)
        return false;

    //value를 다른프로세스로 보냄
    mq_send(msg_q, (const char*)&type, sizeof(int), 0)
    mq_send(msg_q, (const char*)&data, sizeof(int), 0);

    return true;
}

void *func_thread() {
    float zoom, roll, pitch, yaw;

    while(mode != -1) {
        if(mode == 0) {
            zoom = getSonicSens();
            sendData(TYPE_ZOOM, (int)(zoom * 100));
            roll = getRotateSens(1);
            sendData(TYPE_ROLL, (int)(roll * 100));
            pitch = getRotateSens(2);
            sendData(TYPE_PITCH, (int)(pitch *100));
        }
        else if(mode == 1) {
            //data 1? 2? = getBluetooth();
        }
    }
}


int main() {
    msg_q = mq_open(name, O_WRONLY);
    
    if(wiringPiSetupGpio() < 0 ){
        printf("wiringPiSetup() is failed\n");
        return 1;
    } //wiringPi(GPIO)

    pinMode(TRIG, OUTPUT); //Sonic
    pinMode(ECHO, INPUT); //Sonic

    pthread_t thread;
    pthread_create(&thread, NULL, func_thread, NULL);
   
    /*while(1) {
        printf("Distance : %2.2f mm \n",getSonicSens());
        delay(1000);
    }*/ //test sonic
    
    /*while(1) {
        printf("%f %f \n",getRotateSens(1), getRotateSens(2));
        delay(1000);
    }*/ //test Gyro

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
            sendData(TYPE_QUIT, 0);
            continue;
        }
        else if (ch == 'm') {
            mode = 1 - mode;
        }
        else if ('0' <= ch && ch <= '9') { // Send Model No To Rendering Program 
            model_no = ch - '0';
            sendData(TYPE_MODEL_SELECT, model_no);
        }
    }

    pthread_join(thread, NULL);
    return 0;
}