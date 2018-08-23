/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator.c
* @brief:     Э�����ļ������ļ���Ҫ���ڲ���ֲ�ʽ�ļ��п�����
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-22
* @update:    
*/

#include "common_def.h"
#include "distribution_def.h"
#include "iec61850_model.h"
#include "Coordinator_def.h"

#include "lib_memory.h"
#include "Coordinator.h"



/**
* @brief :��ʼ������ָʾ�ռ�
* @param : uint8_t neighbourCount �ھ�����
* @param : uint8_t daCount ���ݼ�����
* @return: NeighborCollect* ���� �õĿռ�
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_init(DeviceIndicate* ic , uint8_t daCount)
{
	if (!ic || daCount==0)
	{
		return false;
	}
	ic->count = daCount;
	ic->daCollect = (DataAttribute**)GLOBAL_CALLOC(sizeof(DataAttribute*), daCount);
	ic->indexTrans = (uint8_t*)GLOBAL_CALLOC(sizeof(uint8_t), daCount);//ת����������
	if ((!ic->daCollect)
		|| (!ic->indexTrans))
	{
		return false;
	}
	for (uint8_t k = 0; k < daCount; k++)
	{
		ic->indexTrans[k] = k;
	}
	return true;
}

/**
* @brief : �����豸ָʾ�������ڴ�ռ�
* @param : DeviceIndicate*  self ��������ָ��
* @return: ָ��
* @update: [2018-08-23][�����][����]
*/
DeviceIndicate* DeviceIndicate_crate(uint8_t daCount)
{
	DeviceIndicate* ic = (DeviceIndicate*)GLOBAL_CALLOC(sizeof(DeviceIndicate), 1);
	if (!ic)
	{
		return NULL;
	}
	bool result = sDeviceIndicate_init(ic , daCount);
	if (result)
	{
		return ic;
	}
	else
	{
		return NULL;
	}
}

/**
* @brief :�����ھӺϼ�
* @param : uint8_t neighbourCount �ھ�����
* @param : uint8_t daCount ���ݼ�����
* @return: NeighborCollect* ���� �õĿռ�
* @update: [2018-08-21][�����][����]
*[2018-08-22][�����][�����м����]
*/
bool DeviceIndicate_initValue(DeviceIndicate* di, LogicalDevice* ld, char** ref, uint8_t* index, uint8_t daCount)
{

	if ((!di) || (!ld) || (!ref) || (!daCount))
	{
		printf("Error Input, (!ld) || (!ref) || (!daCount)\n");
		return false;
	}
	DataAttribute* da;
	for(uint8_t i = 0; i < daCount; i++ )
	{
		da =  (DataAttribute*)ModelNode_getChild((ModelNode*)ld, ref[i]);
		if(!da)
		{
			printf("Error Unfind. ref: %s\n", *ref);
			continue;
		}
		di->daCollect[i] = da;
		di->indexTrans[i] = index[i];
	}
	return true;
}
/**
* @brief :�����ھӺϼ�
* @param : uint8_t neighbourCount �ھ�����
* @param : uint8_t daCount ���ݼ�����
* @return: NeighborCollect* ���� �õĿռ�
* @update: [2018-08-21][�����][����]
*[2018-08-22][�����][�����м����]
*/
NeighborCollect* NeighborCollect_create(uint8_t neighbourCount, uint8_t daCount)
{
	NeighborCollect* nc = (NeighborCollect*)GLOBAL_CALLOC(sizeof(NeighborCollect), 1);
	if (!nc)
	{
		return NULL;
	}
	nc->count = neighbourCount;

	nc->indicateCollect = (DeviceIndicate*)GLOBAL_CALLOC(sizeof(DeviceIndicate), neighbourCount);
	if (!nc->indicateCollect)
	{
		return NULL;
	}
	DeviceIndicate* ic;
	for (uint8_t i = 0; i< neighbourCount; i++)
	{
		ic = &(nc->indicateCollect[i]);
		bool result = DeviceIndicate_init(ic, daCount);
		if (!result)
		{
			return NULL;
		}
	}

	return nc;
}





/**
* @brief :MMS���ݼ�ת��Ϊָ�������Լ�
* @param : const MmsValue* self
* @param : DeviceIndicate* deviceIndicat �豸ָʾ
* @return: void
* @update: [2018-08-22][�����][����]
*/
void MmsDatasetToDataAtrributeSet(const MmsValue* self, DeviceIndicate* deviceIndicate)
{
	if (!self || !deviceIndicate)
	{
		printf("!self || !deviceIndicate)\n");
		return;
	}

	char buffer[1024];
	switch (MmsValue_getType(self))
	{
	case MMS_STRUCTURE:
	case MMS_ARRAY:
	{
		int arraySize = MmsValue_getArraySize(self);

		for (uint8_t k = 0; k < deviceIndicate->count; k++)
		{
			MmsValue* value = (MmsValue*)MmsValue_getElement(self, deviceIndicate->indexTrans[k]);
			DataAttribute* da = deviceIndicate->daCollect[k];
			if (MmsValue_getType(da->mmsValue) == MmsValue_getType(value))
			{
				//da->mmsValue = MmsValue_clone(value);
				//MmsValue_delete(value);
				MmsValue_setBoolean(da->mmsValue, MmsValue_getBoolean(value));
				MmsValue_printToBuffer(da->mmsValue, buffer, 1024);
				//printf(Ref2[i]);
				//printf(":%s\n", buffer);
			}
			else
			{
				printf("type error!\n");
			}
		}
		break;
	}
	}

}

void
gooseListenerRemote(GooseSubscriber subscriber, void* parameter)
{
	DeviceIndicate* di = (DeviceIndicate*)parameter;
	if (!di)
	{
		return;
	}
	printf("GOOSE event:\n");
	printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
		GooseSubscriber_getSqNum(subscriber));
	printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

	uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

	printf("  timestamp: %u.%u\n", (uint32_t)(timestamp / 1000), (uint32_t)(timestamp % 1000));

	MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);


	MmsDatasetToDataAtrributeSet(values, di);
}

/**
* @brief : ���ɶ���goosesʵ��
* @param :
* @param :
* @return: NeighborCollect* ���� �õĿռ�
* @update: [2018-08-22][�����][����]
*/
void GooseSubscriberInstanceStart_remote(GooseReceiver receiver, char** goCbRef,
	uint16_t* appId, DeviceIndicate** deviceIndicate, uint8_t count)
{
	printf("Using interface eth0\n");
	GooseReceiver_setInterfaceId(receiver, "eth0");
	GooseSubscriber subscriber;
	for (uint8_t i = 0; i < count; i++)
	{
		subscriber = GooseSubscriber_create(goCbRef[i], NULL);
		GooseSubscriber_setAppId(subscriber, appId[i]);
		GooseSubscriber_setListener(subscriber, gooseListenerRemote, deviceIndicate[i]);
	}

	GooseReceiver_start(receiver);

	// GooseReceiver_stop(receiver);

	//GooseReceiver_destroy(receiver);
	//return 0;
}




/**
* @brief : ��ȡ�豸״̬
* @param : DeviceIndicate*  self ��������ָ��
* @return: �Ƿ��й���
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_getBooleanStatus(DeviceIndicate* self, DeviceStatusCode code)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[code]]->mmsValue);
}


/**
* @brief : ��ȡ���ع�����Ϣ
* @param : DeviceIndicate*  self ��������ָ��
* @return: �Ƿ��й���
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_isFaultMessage(DeviceIndicate* self)
{

	return MmsValue_getBoolean(self->daCollect[self->indexTrans[0]]->mmsValue);
}
/**
* @brief : �Ƿ�Ϊǰ�����
* @param : DeviceIndicate*  self ��������ָ��
* @return: ˫����Ϣ������
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_isForwardFault(DeviceIndicate* self)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[1]]->mmsValue);
}


/**
* @brief : �Ƿ�Ϊǰ�����
* @param : DeviceIndicate* self ��������ָ��
* @return: ˫����Ϣ������
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_isReverseFault(DeviceIndicate*  self)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[2]]->mmsValue);
}
/**
* @brief : �Ƿ�Ϊǰ�����
* @param : DeviceIndicate* self ��������ָ��
* @return: ˫����Ϣ������
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_isCommunicationFault(DeviceIndicate*  self)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[4]]->mmsValue);
}
/**
* @brief : �Ƿ�Ϊǰ�����
* @param : DeviceIndicate* self ��������ָ��
* @return: ˫����Ϣ������
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_isBilateralVoltage(DeviceIndicate*  self)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[5]]->mmsValue);
}
/**
* @brief : �Ƿ�Ϊǰ�����
* @param : DeviceIndicate* self ��������ָ��
* @return: ˫����Ϣ������
* @update: [2018-08-22][�����][����]
*/
bool SwitchProperty_isOneSideVoltage(DeviceIndicate*  self)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[6]]->mmsValue);
}
/**
* @brief : �Ƿ�Ϊǰ�����
* @param : DeviceIndicate* self ��������ָ��
* @return: ˫����Ϣ������
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_isDistributionExit(DeviceIndicate*  self)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[7]]->mmsValue);
}
/**
* @brief : �Ƿ�Ϊǰ�����
* @param : DeviceIndicate* self ��������ָ��
* @return: ˫����Ϣ������
* @update: [2018-08-22][�����][����]
*/
bool DeviceIndicate_getSwitchPos(DeviceIndicate*  self)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[8]]->mmsValue);
}
