/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator_def.h
* @brief:     �����ļ�
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-22
* @update:    
*/
#ifndef __CORDINATOR_DEF_H
#define __CORDINATOR_DEF_H

#include "common_def.h"
#include "iec61850_model.h"
typedef struct TagDeviceIndicate
{
	uint8_t count;
	DataAttribute** daCollect;//�������Ժϼ�
	uint8_t* indexTrans;//���ݼ���Ӧ������
}DeviceIndicate;



typedef struct TagNeighborCollect
{
	uint8_t count;
	DeviceIndicate* indicateCollect;//ָʾ���Լ�
}NeighborCollect;

typedef enum TagDeviceStatusCode
{
  DEVICE_IED_FAULT      =     0,  //�Ƿ�Ϊ����
  DEVICE_IED_FAULT_FORWARE =     1,   //�Ƿ��������
  DEVICE_IED_FAULT_REVERSE =     2,  //�Ƿ�Ϊ�������
  DEVICE_IED_COMMUNICATION_CORRECT = 3,//�Ƿ�ͨѶ����
  DEVICE_IED_BILATERAL_VOLTAGE = 4,//�Ƿ�Ϊ˫����ѹ
  DEVICE_IED_ONSESIDE_VOLTAGE=     5,//�Ƿ��ǵ�����ѹ
  DEVICE_IED_DISTRIBUTION_EXIT = 6,//�ֲ�ʽ�Ƿ�Ͷ��
  DEVICE_IED_SWITCH_POS 			= 7,//����λ��
  DEVICE_IED_REMOVE_SUCESS = 8,//�г��ɹ�
  DEVICE_IED_INSULATE_SUCESS = 9,//����ɹ�
  DEVICE_IED_ACTION =      10,//����

  DEVICE_IED_ALM1 =   16,//�Ƿ��Ǹ澯1
  DEVICE_IED_ALM2=     17,//�Ƿ��Ǹ澯2
  DEVICE_IED_ALM3 =     18,//�Ƿ��Ǹ澯3
}DeviceStatusCode;

#endif

