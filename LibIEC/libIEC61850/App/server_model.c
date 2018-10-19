/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      server_model.c
  * @brief:     61850数据映射
  * @version:   V0.0.0
  * @author:    Zhang Yufei
  * @date:      2018-08-23
  * @update:
  */
#include "goose_receiver.h"
#include "iec61850_server.h"
#include "hal_thread.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hal_filesystem.h"
#include "iec61850_config_file_parser.h"

#include "ied_data_ref.h"

#include "server_datamapping.h"

#include "goose_misc.h"

#include "station_manager.h"
#include "Coordinator.h"
#include "GooseParser.h"
#include "miscellaneous.h"
#include "extern_interface.h"
#include "status_update.h"
//#define MODEL_CONFIG_PATH "H:\\CodeResourceLib\\Net\\IEC61850\\libIEC61850\\libiec61850-1.2.2-V2\\vs-2015\\examples\\server_example_config_file\\Debug\\stu_v0.01.cfg"

/*测完发布订阅，下面的两个头文件包含以及TestIO_Init函数和Iec61850Server函数中给PF10，PH3的操作就可以删去了*/
#include "drv_gpio.h"
#include "stm32f429xx.h"


extern void UpdateLocalPublicRef(ServerModelManager* manager);
extern void EthernetHookEnable(void);


void GooseSubscriberInstanceStart_remote(GooseReceiver receiver, DatasetSubscriber* daSb);


/**
 * 61850服务管理
 */
ServerModelManager g_ServerModelManager;






/**
* @brief : 遥信goose订阅回调
* @param : GooseSubscriber subscriber
* @param : void* parameter
* @return: void
* @update: [2018-10-19][张宇飞][添加注释]
*/
void gooseListenerRemote(GooseSubscriber subscriber, void* parameter)
{
	DeviceIndicate* di = (DeviceIndicate*)parameter;
	if (!di)
	{
		return;
	}
//    return;
//	printf("GOOSE event:\n");
//	printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
//		GooseSubscriber_getSqNum(subscriber));
//	printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

//	uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

//	printf("  timestamp: %u.%u\n", (uint32_t)(timestamp / 1000), (uint32_t)(timestamp % 1000));

	MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

	//MmsDatasetToDataAtrributeSet(values, di);
    GooseCheckAdd(subscriber, &(di->ringCheck));
}

/**
* @brief : 生成订阅gooses实例
* @param :
* @param :
* @return: DatasetSubscriber* 分配 好的空间
* @update: [2018-08-22][张宇飞][创建]
*/
void GooseSubscriberInstanceStart_remote(GooseReceiver receiver, DatasetSubscriber* daSb)
{
	printf("Using interface eth0\n");
	GooseReceiver_setInterfaceId(receiver, "eth0");
	GooseSubscriber subscriber;
	for (uint8_t i = 0; i < daSb->count; i++)
	{
		subscriber = GooseSubscriber_create(daSb->indicateCollect[i].goCbRef, NULL);
		GooseSubscriber_setAppId(subscriber, daSb->indicateCollect[i].appId);
		GooseSubscriber_setListener(subscriber, gooseListenerRemote, (void*)(&(daSb->indicateCollect[i])));
		GooseReceiver_addSubscriber(receiver, subscriber);
		RingQueueInit(&(daSb->indicateCollect[i].ringCheck), 1000);
		printf("goCbRef:%s, appId, id:0x%x.\n", daSb->indicateCollect[i].goCbRef, daSb->indicateCollect[i].appId);
	}

	GooseReceiver_start(receiver);

	// GooseReceiver_stop(receiver);

	//GooseReceiver_destroy(receiver);
	//return 0;
}




/**
  * @brief :iec61850App
  * @param  Svoid
  * @return: 0--
  * @update: [2018-08-15][创建]
  */
int Iec61850Server(void)
{
    int tcpPort = 102;
    uint8_t id;	



    if (g_StationManger.pWorkPoint)
    {
        id = (uint8_t)(g_StationManger.pWorkPoint->id);
        System_getConfigFile(id);
    }
    else
    {
        perror("g_StationManger.pWorkPoint is Null\n");
        return -1;
    }

	g_ServerModelManager.model = CreateIedModeFromConfig( (char*)System_getConfigFullName(id));
    if (!g_ServerModelManager.model)
    {
        printf("CreateIedModeFromConfig is null\n");
        return -1;
    }

    LogicalDeviceDataRefInit( g_ServerModelManager.model);
    g_ServerModelManager.receiver = GooseReceiver_create();
	if (!g_ServerModelManager.receiver)
	{
		perror("GooseReceiver_create failre\n");
		return  -1;
	}

	g_ServerModelManager.server = IedServer_create(g_ServerModelManager.model);

	IedServer_start(g_ServerModelManager.server , tcpPort);

	if (!IedServer_isRunning(g_ServerModelManager.server )) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(g_ServerModelManager.server );
		return -1;
	}



	//绑定本地开关
	BindLocalSwitchStatus();

	//获取订阅数据集
	bool result = ServerModelManager_updateGooseSubscribeData(System_getGooseConfigFullName(id));
	if(result)
	{
		GooseSubscriberInstanceStart_remote(g_ServerModelManager.receiver,
				g_ServerModelManager.dsSubscriber);
	}
	else
	{
		printf("ServerModelManager_updateGooseSubscribeData! failure.\n");
		return -1;
	}
    EthernetHookEnable();
    //首先进行绑定更新
    LocalPropertyToDataArribute(g_StationManger.pWorkPoint->topology.localSwitch,
				g_StationManger.pWorkPoint->topology.localSwitch->pDeviceIndicate);

	/* Start GOOSE publishing */
	IedServer_enableGoosePublishing(g_ServerModelManager.server );
       
	while (true) {
		//统一更新
		//IedServer_lockDataModel(g_ServerModelManager.server );

      //  IedServer_forceUpdatePublish_Ex(g_ServerModelManager.server, IED_LD0_GGIO17_Ind8_stVal);
		//以此触发
		//IedServer_unlockDataModel(g_ServerModelManager.server );

		Thread_sleep(11000);
	}



	IedServer_stop(g_ServerModelManager.server );

	IedServer_destroy(g_ServerModelManager.server );

	IedModel_destroy(g_ServerModelManager.model);
	


}
