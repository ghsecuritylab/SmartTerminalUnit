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
//#include "extern_interface.h"
#include "ied_data_ref.h"

#include "server_datamapping.h"



#include "station_manager.h"
#include "Coordinator.h"
#include "GooseParser.h"

#define MODEL_CONFIG_PATH  "//sojo//stu.cfg"
#define GOOSE_CONFIG_PATH  "//sojo//stu-goose.txt"
//#define MODEL_CONFIG_PATH "H:\\CodeResourceLib\\Net\\IEC61850\\libIEC61850\\libiec61850-1.2.2-V2\\vs-2015\\examples\\server_example_config_file\\Debug\\stu_v0.01.cfg"


extern void UpdateLocalPublicRef(ServerModelManager* manager);


void GooseSubscriberInstanceStart_remote(GooseReceiver receiver, DatasetSubscriber* daSb);


/**
 * 61850服务管理
 */
ServerModelManager g_ServerModelManager;








void GetNeighbourCount(void)
{



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


    g_ServerModelManager.model = CreateIedModeFromConfig( MODEL_CONFIG_PATH);
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
	bool result = ServerModelManager_updateGooseSubscribeData(GOOSE_CONFIG_PATH);
	if(result)
	{
		GooseSubscriberInstanceStart_remote(g_ServerModelManager.receiver,
				g_ServerModelManager.dsSubscriber);
	}
	else
	{
		printf("ServerModelManager_updateGooseSubscribeData! failure.\n");
	}
	//DataSet* dsGoose = IedModel_lookupDataSet(g_ServerModelManager.model, "STU1LD0/LLN0$dsGoose");

	/* Start GOOSE publishing */
	IedServer_enableGoosePublishing(g_ServerModelManager.server );


	//GetNeighbourCount();
	int32_t cn = 0;
	float i = 100;
	float v = 1000;
	float f = 50;
	float angle = 1;
	float va = 10000;
	bool state = false;
	float ratio = 0;
	while (true) {

		ratio = 1- (float)cn*0.01;

		i = 100  * ratio;
		v = 1000  * ratio;
		f = 50  * ratio;
		angle = 1  * ratio;
		va = 10000  * ratio;
		state = !state;


		(cn++ > 10) ? (cn = -10) : (cn++);

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
        printf("goCbRef:%s, appId, id:0x%x.\n", daSb->indicateCollect[i].goCbRef, daSb->indicateCollect[i].appId);
	}

	GooseReceiver_start(receiver);

	// GooseReceiver_stop(receiver);

	//GooseReceiver_destroy(receiver);
	//return 0;
}





