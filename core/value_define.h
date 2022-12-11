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
    long type; ///< 메시지큐 내에 저장된 각 메시지의 유형을 식별하기 위한 정보. 반드시 이 위치에 정의되어야 한다.
    float data1; ///< 필요에 의해 정의한 사용자 데이터
    float data2; ///< 필요에 의해 정의한 사용자 데이터
};

struct ModelType
{
    long type; ///< 메시지큐 내에 저장된 각 메시지의 유형을 식별하기 위한 정보. 반드시 이 위치에 정의되어야 한다.
    int model;
};