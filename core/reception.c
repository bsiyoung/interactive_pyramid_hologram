#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdio.h>

#include "value_define.h"


// 메시지큐 식별자
int msg_q;
// 데이터 수신 쓰레드
pthread_t data_thread;
// 모델 수신 쓰레드
pthread_t model_thread;

//데이터를 받아서 출력
static void * DataThread(void *arg)
{
  int ret;
  struct DataType dataType;

  while(1) {
    ret = msgrcv(msg_q, &dataType, sizeof(dataType) - sizeof(long), SendType_Data, 0);
    if (ret <= 0) {
      perror("msgrcv()");
      continue;
    }
    printf("Data is received - data_Type: %d, data: %f\n", dataType.data_Type, dataType.data);
  }
}

//모델 번호를 받아서 출력
static void * ModelThread(void *arg)
{
  int ret;
  struct ModelType modelType;

  while(1) {
    ret = msgrcv(msg_q, &modelType, sizeof(modelType) - sizeof(long), SendType_Model, 0);
    if (ret <= 0) {
      perror("msgrcv()");
      continue;
    }

    printf("ModelNumber is received - mdoel_no: %d \n", modelType.model);
  }
}


int main(int argc, char *argv[])
{
  // 메시지큐를 연다. (해당 메시지큐가 시스템에 존재하지 않으면 생성한다)
  msg_q = msgget((key_t)MSG_Q_KEY, IPC_CREAT | 0666);
  if (msg_q == -1) {
    perror("msgget()");
    return -1;
  }

  // 메시지큐를 통해 데이터를 수신할 쓰레드를 생성한다.
  if (pthread_create(&data_thread, NULL, DataThread, NULL) < 0) {
    perror("pthread_create()");
    return -1;
  }
  
  // 메시지큐를 통해 모델 번호를 수신할 쓰레드를 생성한다.
  if (pthread_create(&model_thread, NULL, ModelThread, NULL) < 0) {
    perror("pthread_create()");
    return -1;
  }

  // 각 쓰레드가 종료될 때까지 대기한다.
  pthread_join(data_thread, NULL);
  pthread_join(model_thread, NULL);

  return 0;
}