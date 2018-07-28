/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_monitor.c
* @brief:     ���ڷֲ�ʽ�����أ�ά������״̬
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-28
* @update:
*/


#include "distribution.h"
#include "distribution_monitor.h"
#include "distribution_config.h"

/**
* @brief :�����Ϣ����Ч��
* @param  StationManger* manager
* @return: 0--����
* @update: [2018-07-16][�����][����]
*/
ErrorCode CheckMessageValid(StationPoint* station)
{
	CHECK_POINT_RETURN_LOG(station, NULL, ERROR_NULL_PTR, 0);
	ListDouble* pSwitchList = &(station->topology.globalSwitchList);
	CHECK_POINT_RETURN_LOG(pSwitchList, NULL, ERROR_NULL_PTR, 0);
	SwitchProperty* nodeSwitch;
	FOR_EARCH_LIST_START(pSwitchList);
	{
		nodeSwitch = GET_SWITCH_ELEMENT(m_foreach);
		//�����Ч״̬��ʱ���Ƿ�ʱ
		if (nodeSwitch->isValid)
		{
			if (SystemIsOverTime(nodeSwitch->updateTime, MONITOR_LIFT_TIME))
			{
				nodeSwitch->isValid = false;
			}
		}

	}
	FOR_EARCH_LIST_END();
	

}

