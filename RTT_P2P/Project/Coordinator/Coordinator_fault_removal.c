/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator.c
* @brief:     Э�����ļ������ļ���Ҫ���ڲ���ֲ�ʽ�ļ��п�����
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-22
* @update:    
*/

#include <stdbool.h>
#include "Coordinator.h"

 /**
  * @brief :Э���Թ����Ƴ�״̬��������Start
  * @param
  * @return: 0-����
  * @update: [2018-08-23][�����][BRIEF]
  */
 StateResult CoordinatorRemovalState_Start(FaultDealHandle* handle)
{
	SwitchProperty* switchProperty = handle->switchProperty;
	DeviceIndicate*  di = (DeviceIndicate*)(handle->switchProperty->pDeviceIndicate);
    StateResult result = RESULT_NULL;
    if (handle->nextState != REMOVAL_START)
    {
        return RESULT_ERROR_MATCH;
    }

	handle->state = REMOVAL_START;

	if (handle->IsFault(handle))
	{
        handle->isRun = true;
		//���͹�����Ϣ
		handle->TransmitMessage(handle, STATUS_MESSAGE);
        PrintIDTipsTick(switchProperty->id, "Fault TransmitMessage");
		handle->limitTime = handle->t1;
		handle->GetNowTime(handle);
		handle->nextState = REMOVAL_GATHER;
	}
	else if (handle->IsTrigger(handle))
	{
		//���ͷǹ�����Ϣ
        handle->isRun = true;
		switchProperty->distributionArea->SignExitFaultMessage(switchProperty);
		handle->TransmitMessage(handle, STATUS_MESSAGE);
        PrintIDTipsTick(switchProperty->id, "Trigger TransmitMessage");
		handle->limitTime = handle->t2;
		handle->GetNowTime(handle);
        handle->step = 0;

		handle->nextState =  REMOVAL_DELAY_GATHER;
	}
	//�Ƿ�Ϊ�����ڼ�ܾ����չ���
	else if (handle->IsRejectInsulate(handle))
	{
		handle->isRun = true;
		handle->step = INSULATE_BACKUP;
		PrintIDTipsTick(switchProperty->id, "Meet Reject Insulate");
		handle->nextState = INSULATE_DEAL;
	}
	//�Ƿ�����ת����
	else if (handle->IsPermitInTransferPowerMode(handle))
	{
		handle->isRun = true;
		handle->step = 0;
		PrintIDTipsTick(switchProperty->id, "Meet In TransferPower Mode");
		handle->nextState = TRANSFER_CONNECT;
	}
	else if (handle->IsPowerIncomeLossFault(handle))
	{
		handle->isRun = true;
		handle->step = INSULATE_BACKUP;
		PrintIDTipsTick(switchProperty->id, "Power income Loss Fault");
		handle->nextState = INSULATE_DEAL;
	}


	else
	{
		handle->nextState =  REMOVAL_START;
	}

	handle->lastState = handle->state;
	return result;
}


