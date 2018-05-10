/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      point_table_config.h
  * @brief:     The point_table_config of the project.
  * @version:   ver 1.1
  * @author:    Mr.Sxr
  * @date:      2018-05-09
  * @update:    2018-05-09 [][]
  */
	
#ifndef __POINT_TABLE_CONFIG_H__
#define __POINT_TABLE_CONFIG_H__

//enum MENU_NUM
//{
//    
//};

struct tagTelesignalCfg
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    unsigned char *pVal;   // 值	
};

struct tagTelemetryCfg
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    float *pVal;           // 值	
    char *pUnit;           // 单位
    float *pCalifactor;    // 校准系数		
	float *pZerodrift;     // 零漂    
	float *pDeadzone;      // 死区	
};
	
struct tagCalibrateFactor
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    float Val;             // 值	
//    char *pUnit;           // 单位
};

struct tagValueParaCfg
{
	unsigned char enable;  // 使能
	unsigned char menuNum; // 菜单编号
    char *pName;           // 名称
    float *pVal;           // 值	
    char *pUnit;           // 单位
    float valMax;	       // 上限值	
    float valMin;          // 下限值
	unsigned char dataType;// 数据类型
	char *pContent[5];        // 数据显示内容
	char *pNote;           // 备注
};

extern struct tagTelesignalCfg TelesignalCfg[];
extern struct tagTelemetryCfg TelemetryCfg[];
extern struct tagCalibrateFactor CalibrateFactorCfg[];
extern struct tagValueParaCfg ParameterCfg[];
extern struct tagValueParaCfg FixedValueCfg1[];
extern struct tagValueParaCfg FixedValueCfg2[];


#endif /* __POINT_TABLE_CONFIG_H__ */

/* END OF FILE ---------------------------------------------------------------*/
