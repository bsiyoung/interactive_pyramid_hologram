#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <wiringPi.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "value_define.h"

int mode = 0;
int model_no = 0;

//메시지 큐 식별자
int msg_q;

float getSonicSens() {
    float i = 0;
    //초음파센서
    return i;
}

float getRotateSens() {
    float i = 0;
    //가속도센서
    return i;
}

float getBluetooth() {
    float i = 0;
    //블루투스관련
    return i;
}

bool sendData(float data1, float data2) {
    //value를 다른프로세스로 보냄
    msg_q = msgget((key_t)MSG_Q_KEY, IPC_CREAT | 0666);
    if (msg_q == -1) {
        perror("msgget()");
        return -1;
    }
    struct DataType dataType;
    memset(&dataType, 0, sizeof(dataType));
    dataType.type = SendType_Data;
    dataType.data1 = data1;
    dataType.data2 = data2;

    if (msgsnd(msg_q, &dataType, sizeof(dataType) - sizeof(long), IPC_NOWAIT) < 0) {
        perror("msgsnd()");
    }

    sleep(1);
    return true;
}

void *func_thread() {
    float data1, data2;
    while(mode != -1) {
        if(mode == 0) {
            data1 = getSonicSens();
            data2 = getRotateSens();
            sendData(data1, data2);
        }
        else if(mode == 1) {
            //data 1? 2? = getBluetooth();
            sendData(data1, data2);
        }

        //sendData(0, data1); sendData(1, data2);
    }
}


int main() {

    //센서용 wiringPi
    /*if(wiringPiSetupGpio() < 0 ){
        printf("wiringPiSetup() is failed\n");
        return 1;
    }*/

    pthread_t thread;
    pthread_create(&thread, NULL, func_thread, NULL);

    msg_q = msgget((key_t)MSG_Q_KEY, IPC_CREAT | 0666);
    if (msg_q == -1) {
        perror("msgget()");
        return -1;
    }

    struct ModelType modelType;
    
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

        sleep(1);
    }

    pthread_join(thread, NULL);
    return 0;
}