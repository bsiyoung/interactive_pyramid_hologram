#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <wiringPi.h>

int mode = 0;
int model_no = 0;

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

bool sendData(int type, float value) {
    //value를 다른프로세스로 보냄
    return true;
}

void *func_thread() {
    float data1, data2;
    while(mode != -1) {
        if(mode == 0) {
            data1 = getSonicSens();
            data2 = getRotateSens();
        }
        else if(mode == 1) {
            //data 1? 2? = getBluetooth();
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
        }
    }

    pthread_join(thread, NULL);
    return 0;
}