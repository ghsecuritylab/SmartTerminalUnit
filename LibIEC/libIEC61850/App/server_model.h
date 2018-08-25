/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      server_model.h
  * @brief:     61850����ӳ��
  * @version:   V0.0.0
  * @author:    Zhang Yufei
  * @date:      2018-08-23
  * @update:
  */

#ifndef __SERVER_MDODEL_H_
#define __SERVER_MDODEL_H_


#include "iec61850_model.h"
#include "coordinator_def.h"

int Iec61850Server(void);

extern ServerModelManager g_ServerModelManager;
#endif 
