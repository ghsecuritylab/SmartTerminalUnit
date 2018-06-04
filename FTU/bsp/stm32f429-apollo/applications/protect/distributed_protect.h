
#ifndef _DISTRIBUTED_PROTECT_H_
#define _DISTRIBUTED_PROTECT_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "single_list.h"
#include "common_data.h"
#include "drv_do.h"


#define DISTRIBUT_MAXTIMERS (32)

#define DISTRIBUT_ENTIMERS 0x80000000
#define DISTRIBUT_TITIMERS 0x7fffffff


//STEADYSTATE
#define  _DISTRIBUT_S_TRUNK                       0x00000001   //����
#define  _DISTRIBUT_S_BRANCH                      0x00000002   //֧��
#define  _DISTRIBUT_S_BREAKER                     0x00000004   //��·����Ĭ�ϣ�
#define  _DISTRIBUT_S_LOADSWTICH                  0x00000008   //�ֶΣ�Ĭ�ϣ�

//VARIABLESTATE
#define  _DISTRIBUT_V_SWTICHOPEN                  0x00000001   //��
#define  _DISTRIBUT_V_SWTICHCLOSE                 0x00000002   //��
#define  _DISTRIBUT_V_POSITIVE                    0x00000004   //����
#define  _DISTRIBUT_V_BACKWARD                    0x00000008   //����
#define  _DISTRIBUT_V_BLOCKING                    0x00000100   //����
#define  _DISTRIBUT_V_REQUIRED_SPACER             0x00000200   //��Ҫ����
#define  _DISTRIBUT_V_FINISH_SPACER               0x00000400   //��ɸ���
#define  _DISTRIBUT_V_REQUIRED_ACT                0x00000800   //��Ҫ����
#define  _DISTRIBUT_V_OVERCURRENT_ACT             0x00001000   //�������¶���
#define  _DISTRIBUT_V_FAILURE_OPERATE             0x00010000   //�ܶ�
#define  _DISTRIBUT_V_VOLTAGE_M                   0x00020000   //M����ѹ
#define  _DISTRIBUT_V_VOLTAGE_N                   0x00040000   //N����ѹ
#define  _DISTRIBUT_V_OVERCURRENT                 0x00080000   //����
#define  _DISTRIBUT_V_COMMUNICAT_FAULT_SELF       0x01000000   //ͨѶ���ϣ�����������
#define  _DISTRIBUT_V_COMMUNICAT_FAULT_OTHER      0x02000000   //ͨѶ���ϣ�������������
#define  _DISTRIBUT_V_EXIT_SELF                   0x04000000   //�����˳�������
#define  _DISTRIBUT_V_EXIT_OTHER                  0x08000000   //�����˳���������
#define  _DISTRIBUT_V_CLEAN_COMMUNICAT_EXIT       0x10000000   //���״̬��ͨѶ���ϡ������˳���

//OPREAT
#define  _DISTRIBUT_O_IP                          0x000000ff   //IP
#define  _DISTRIBUT_O_SWTICHOPEN                  0x00000100   //��
#define  _DISTRIBUT_O_SWTICHCLOSE                 0x00000200   //��

//FLAG
#define  _DISTRIBUT_FLAG_SWTICHOPEN                  0x00000001   //��
#define  _DISTRIBUT_FLAG_SWTICHCLOSE                 0x00000002   //��
#define  _DISTRIBUT_FLAG_DOUBLEVOLTAGE               0x00000010   //��˫����ѹ״̬
#define  _DISTRIBUT_FLAG_FLAUT                       0x00000100   //���ϵ��ж�
#define  _DISTRIBUT_FLAG_OVERCURRENT_ACT             0x00001000   //�������¶���


enum
{
    DISTRIBUT_FALSE,                // 
    DISTRIBUT_TRUE
};

//#pragma pack(push,1)

typedef struct
{
    struct
    {
        uint32_t *gTime;       
    }valstr;
//    struct
//    {   
//    }parastr;
}FunctionalRetreatSts;

typedef struct
{
    struct
    {
        uint32_t *gTime;       
    }valstr;
//    struct
//    {   
//    }parastr;
}SelfstateJudgeSts;

typedef struct
{
    struct
    {
        uint32_t *gTime;       
    }valstr;
//    struct
//    {   
//    }parastr;
}FaultIsolationSts;

typedef struct
{
    uint32_t by0;
    uint32_t by1;
    uint32_t by2;
    uint32_t by3;
    uint32_t by4;
    uint32_t by5;
    uint32_t variableState;
    uint32_t operation;
}ComStr;
    
#define  _DISTRIBUT_STOREQUEUE_SIZE                 256
typedef struct
{
    uint8_t getflag;
    ComStr comstr;
}StoreQueueMemberStr;

//typedef struct
//{
//    StoreQueueMemberStr store[_DISTRIBUT_STOREQUEUE_SIZE];
//    StoreQueueMemberStr *storeIn;
//    StoreQueueMemberStr *storeOut;
//}StoreQueueStr;

typedef struct 
{ 
    uint8_t ip;
    uint32_t steadyState;
    ComStr comstr;
    uint32_t *gTime;
}DevStr;


typedef struct 
{
    uint16_t addr;           
    uint8_t *value;       
}STelesignalStr;

typedef struct 
{
    STelesignalStr switchOpen;             
    STelesignalStr switchClose;           
}TelesignalStr;

typedef struct 
{
    float *Ia;                       
    float *Ib;                   
    float *Ic;                    
    float *I0;                     
    float *Uab;                    
    float *Ubc;                     
    float *Uac;                    
    float *U0;                                           
}TelemetryStr;

typedef struct
{
    TelesignalStr yx;
    TelemetryStr yc;
    uint16_t fevent_yc_addr[10];
	float *pWorkMode;
    uint8_t (*opening)(uint16_t addr, uint8_t operateType);
    uint8_t (*closing)(uint16_t addr, uint8_t operateType);
    uint8_t (*outputSoe)(uint16_t addr, uint8_t state);
    uint8_t (*outputFevent)(uint16_t yx_addr, uint16_t *yc_addr, uint16_t yc_num);
}ComProSts;

//#pragma pack(pop)

#endif /* END _DISTRIBUTED_PROTECT_H_ */

