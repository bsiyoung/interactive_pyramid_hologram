#include <stdint.h>

/// 메시지큐 키 값
#define MSG_Q_KEY (0x64)

enum eSendType
{
    SendType_Unused = 0,
    SendType_Data = 1,
    SendType_Model = 2,
};

struct DataType
{
    long type;
    int data_Type;
    float data;
};

struct ModelType
{
    long type;
    int model;
};