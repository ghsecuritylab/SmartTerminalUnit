/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      point_table_config.c
  * @brief:     The point_table_config  of the project.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2018-05-09
  * @update:    [2018-05-09][name][change]
  */
	
	
/* INCLUDES FILES ------------------------------------------------------------*/
#include "common_data.h"
#include "point_table_config.h"


/* 遥信 */
struct tagTelesignalCfg TelesignalCfg[] = 
{
//   使能    名称	        点号                                         遥信           描述
    { 1,  "开关分位",     &g_TelesignalAddr.switchOpen,                  NULL,          {"分", "合"},      {"合->分","分->合"}},
    { 1,  "开关合位",     &g_TelesignalAddr.switchClose,                 NULL,          {"分", "合"},      {"合->分","分->合"}},
    { 1,  "储能/有压",    &g_TelesignalAddr.operatingMechanism,          NULL,          {"无", "有"},      {"有->无","无->有"}},
    { 1,  "低气压",       &g_TelesignalAddr.lowPressure,                 NULL,          {"无", "告警"},    {"告警->复位","无->告警"}},
    { 1,  "电源故障告警", &g_TelesignalAddr.powerFaultAlarm,             NULL,          {"无", "告警"},    {"告警->复位","无->告警"}},
    { 1,  "电池欠压告警", &g_TelesignalAddr.batteryUnderVoltageAlarm,    NULL,          {"无", "告警"},    {"告警->复位","无->告警"}},
    { 1,  "电池活化状态", &g_TelesignalAddr.batteryActivationStatus,     NULL,          {"无", "活化"},    {"活化->结束","开始活化"}},
    { 1,  "电池失电告警", &g_TelesignalAddr.batteryLossAlarm,            NULL,          {"无", "告警"},    {"告警->复位","无->告警"}},
    { 1,  "开关类型",     &g_TelesignalAddr.swtichclass,                 NULL,          {"断路器","负荷开关"}, {"断路器","负荷开关"}},
    { 1,  "工作模式",     &g_TelesignalAddr.breakContact,                NULL,          {"分段", "联络"},  {"联络->分段","分段->联络"}},
    { 1,  "重合压板/FA",  &g_TelesignalAddr.recloseFAHardStrap,          NULL,          {"退出", "投入"},  {"投->退","退->投"}},
    { 1,  "保护压板",     &g_TelesignalAddr.functionHardStrap,           NULL,          {"退出", "投入"},  {"投->退","退->投"}},
    { 1,  "遥控保护压板", &g_TelesignalAddr.telecontrolProOut,           NULL,          {"投入", "退出"},  {"退->投","投->退"}},
    { 1,  "远方/就地",    &g_TelesignalAddr.remoteEarth,                 NULL,          {"就地", "远方"},  {"远方->就地","就地->远方"}},
    { 0,  "短路故障",     &g_TelesignalAddr.shortCircuitFault,           NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "零序故障",     &g_TelesignalAddr.earthingFault,               NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "保护动作",     &g_TelesignalAddr.protectionAct,               NULL,          {"复位","动作"},   {"复位","动作"}},
    { 0,  "保护闭锁",     &g_TelesignalAddr.protectionLock,              NULL,          {"复位","闭锁"},   {"复位","闭锁"}},
    { 0,  "合闸闭锁",     &g_TelesignalAddr.closingLocked,               NULL,          {"复位","闭锁"},   {"复位","闭锁"}},	
    { 0,  "分闸闭锁",     &g_TelesignalAddr.openingLocked,               NULL,          {"复位","闭锁"},   {"复位","闭锁"}},
    { 0,  "重合动作",     &g_TelesignalAddr.recloseAct,                  NULL,          {"复位","动作"},   {"复位","动作"}},
    { 0,  "重合闭锁",     &g_TelesignalAddr.recloseLock,                 NULL,          {"复位","闭锁"},   {"复位","闭锁"}},
    { 0,  "装置故障",     &g_TelesignalAddr.deviceFault,                 NULL,          {"无","故障"},     {"无","故障"}},
    { 0,  "自检异常",     &g_TelesignalAddr.selfCheckAbnomal,            NULL,          {"无","异常"},     {"无","异常"}},
    { 0,  "通讯",         &g_TelesignalAddr.communication,               NULL,          {"断","通"},       {"断","通"}},	
    { 0,  "电池故障告警", &g_TelesignalAddr.batteryFaultAlarm,           NULL,          {"无","告警"},     {"无","告警"}},
    { 0,  "双点开关分合", &g_TelesignalAddr.doubleSwitch,                NULL,          {"合->分","分->合"}, {"合->分","分->合"}}, 
    { 0,  "控制回路断线", &g_TelesignalAddr.controlLoopDisconnection,    NULL,          {"复位","断线"},   {"复位","断线"}},
    { 0,  "A相过流一段",  &g_TelesignalAddr.overcurrentIa1,              NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "B相过流一段",  &g_TelesignalAddr.overcurrentIb1,              NULL,          {"复位","保护"},   {"复位","保护"}},	
    { 0,  "C相过流一段",  &g_TelesignalAddr.overcurrentIc1,              NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "A相过流二段",  &g_TelesignalAddr.overcurrentIa2,              NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "B相过流二段",  &g_TelesignalAddr.overcurrentIb2,              NULL,          {"复位","保护"},   {"复位","保护"}},	
    { 0,  "C相过流二段",  &g_TelesignalAddr.overcurrentIc2,              NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "A相过流三段",  &g_TelesignalAddr.overcurrentIa3,              NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "B相过流三段",  &g_TelesignalAddr.overcurrentIb3,              NULL,          {"复位","保护"},   {"复位","保护"}},	
    { 0,  "C相过流三段",  &g_TelesignalAddr.overcurrentIc3,              NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "零序过流一段", &g_TelesignalAddr.overcurrentI01,              NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "零序过流二段", &g_TelesignalAddr.overcurrentI02,              NULL,          {"复位","保护"},   {"复位","保护"}},		
    { 0,  "A相后加速",    &g_TelesignalAddr.overcurrentAccIa,            NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "B相后加速",    &g_TelesignalAddr.overcurrentAccIb,            NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "C相后加速",    &g_TelesignalAddr.overcurrentAccIc,            NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "A相反时限",    &g_TelesignalAddr.overcurrentInverseIa,        NULL,          {"复位","保护"},   {"复位","保护"}},  
    { 0,  "B相反时限",    &g_TelesignalAddr.overcurrentInverseIb,        NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "C相反时限",    &g_TelesignalAddr.overcurrentInverseIc,        NULL,          {"复位","保护"},   {"复位","保护"}},	
    { 0,  "一次重合闸",   &g_TelesignalAddr.recloseProtection1,          NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "二次重合闸",   &g_TelesignalAddr.recloseProtection2,          NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "三次重合闸",   &g_TelesignalAddr.recloseProtection3,          NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "四次重合闸",   &g_TelesignalAddr.recloseProtection4,          NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "重合闭锁",     &g_TelesignalAddr.recloseClockProtection,      NULL,          {"复位","闭锁"},   {"复位","闭锁"}},	
    { 0,  "零序一次重合", &g_TelesignalAddr.zeroRecloseProtection1,      NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "零序二次重合", &g_TelesignalAddr.zeroRecloseProtection2,      NULL,          {"复位","保护"},   {"复位","保护"}},			
    { 0,  "二次重合闭锁", &g_TelesignalAddr.clockSecondaryClose,         NULL,          {"复位","闭锁"},   {"复位","闭锁"}},
    { 0,  "失电分闸",     &g_TelesignalAddr.lossTrip,                    NULL,          {"复位","跳闸"},   {"复位","跳闸"}},
    { 0,  "得电合闸",     &g_TelesignalAddr.getClossing,                 NULL,          {"复位","合闸"},   {"复位","合闸"}},
    { 0,  "合闸于故障",   &g_TelesignalAddr.closingFaultEvent,           NULL,          {"无","故障"},     {"无","故障"}},
    { 0,  "保护闭锁",     &g_TelesignalAddr.protectionLocked,            NULL,          {"复位","闭锁"},   {"复位","闭锁"}},	
    { 0,  "双压禁止合闸", &g_TelesignalAddr.doubleProhibitClossing,      NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "单侧失压合闸", &g_TelesignalAddr.singleLossClosingEvent,      NULL,          {"复位","动作"},   {"复位","动作"}},
    { 0,  "残压闭锁合闸", &g_TelesignalAddr.remainClockClossing,         NULL,          {"复位","保护"},   {"复位","保护"}},	
    { 0,  "接地故障跳闸", &g_TelesignalAddr.earthingFaultTripU0,         NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "小信号接地",   &g_TelesignalAddr.smallSignalEarthingTrip,     NULL,          {"复位","跳闸"},   {"复位","跳闸"}},
    { 0,  "遮断电流",     &g_TelesignalAddr.breakingCurrent,             NULL,          {"复位","保护"},   {"复位","保护"}},	
    { 0,  "过负荷",       &g_TelesignalAddr.overloadEvent,               NULL,          {"复位","告警"},   {"复位","告警"}},
    { 0,  "重过载",       &g_TelesignalAddr.haveyOverload,               NULL,          {"复位","告警"},   {"复位","告警"}},
    { 0,  "过压保护",     &g_TelesignalAddr.overVoltageProtection,       NULL,          {"复位","保护"},   {"复位","保护"}},		
    { 0,  "过频保护",     &g_TelesignalAddr.overFrequencyProtection,     NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "低压保护",     &g_TelesignalAddr.downVoltageProtection,       NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "低频保护",     &g_TelesignalAddr.downFrequencyProtection,     NULL,          {"复位","保护"},   {"复位","保护"}},
    { 0,  "Uab越上限",    &g_TelesignalAddr.overLimitUabUp,              NULL,          {"复位","越上限"}, {"复位","越上限"}},
    { 0,  "Uab越下限",    &g_TelesignalAddr.overLimitUabDown,            NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "Ubc越上限",    &g_TelesignalAddr.overLimitUbcUp,              NULL,          {"复位","越上限"}, {"复位","越上限"}},
    { 0,  "Ubc越下限",    &g_TelesignalAddr.overLimitUbcDown,            NULL,          {"复位","越下限"}, {"复位","越下限"}},
    { 0,  "Uca越上限",    &g_TelesignalAddr.overLimitUcaUp,              NULL,          {"复位","越上限"}, {"复位","越上限"}},	
    { 0,  "Uca越下限",    &g_TelesignalAddr.overLimitUcaDown,            NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "UAB越上限",    &g_TelesignalAddr.overLimitUABUp,              NULL,          {"复位","越上限"}, {"复位","越上限"}},
    { 0,  "UAB越下限",    &g_TelesignalAddr.overLimitUABDown,            NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "UBC越上限",    &g_TelesignalAddr.overLimitUBCUp,              NULL,          {"复位","越上限"}, {"复位","越上限"}},
    { 0,  "UBC越下限",    &g_TelesignalAddr.overLimitUBCDown,            NULL,          {"复位","越下限"}, {"复位","越下限"}},		
    { 0,  "U0越上限",     &g_TelesignalAddr.overLimitU0Up,               NULL,          {"复位","越上限"}, {"复位","越上限"}},	
    { 0,  "U0越下限",     &g_TelesignalAddr.overLimitU0Down,             NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "Ia越上限",     &g_TelesignalAddr.overLimitIaUp,               NULL,          {"复位","越上限"}, {"复位","越上限"}},
    { 0,  "Ia越下限",     &g_TelesignalAddr.overLimitIaDown,             NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "Ib越上限",     &g_TelesignalAddr.overLimitIbUp,               NULL,          {"复位","越上限"}, {"复位","越上限"}},
    { 0,  "Ib越下限",     &g_TelesignalAddr.overLimitIbDown,             NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "Ic越上限",     &g_TelesignalAddr.overLimitIcUp,               NULL,          {"复位","越上限"}, {"复位","越上限"}},	
    { 0,  "Ic越下限",     &g_TelesignalAddr.overLimitIcDown,             NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "I0越上限",     &g_TelesignalAddr.overLimitI0Up,               NULL,          {"复位","越上限"}, {"复位","越上限"}},	
    { 0,  "I0越下限",     &g_TelesignalAddr.overLimitI0Down,             NULL,          {"复位","越下限"}, {"复位","越下限"}},		
    { 0,  "DC_U越上限",   &g_TelesignalAddr.overLimitDC_U_Up,            NULL,          {"复位","越上限"}, {"复位","越上限"}},	
    { 0,  "DC_U越下限",   &g_TelesignalAddr.overLimitDC_U_Down,          NULL,          {"复位","越下限"}, {"复位","越下限"}},	
    { 0,  "DC_I越上限",   &g_TelesignalAddr.overLimitDC_I_Up,            NULL,          {"复位","越上限"}, {"复位","越上限"}},	
    { 0,  "DC_I越下限",   &g_TelesignalAddr.overLimitDC_I_Down,          NULL,          {"复位","越下限"}, {"复位","越下限"}},		
    { 0,  "装置掉电",     &g_TelesignalAddr.devicePowerDown,             NULL,          {"复位","掉电"},   {"复位","掉电"}},
};
	
/* 遥测 */
struct tagTelemetryCfg TelemetryCfg[] = 
{
//   使能 菜单编号   名称           上位机名称          点号                                值         单位                 校准系数                             零漂                             死区	
    { 1,  SECONDRY,  "f",           "频率(f)",          &g_TelemetryAddr.F,                 NULL,      "Hz",       &g_CalibrateFactor[CALIFACTOR_F],      &g_Parameter[ZERODRIFT_F],      &g_Parameter[DEADZONE_F]},
    { 1,  SECONDRY,  "Ia",          "相电流(Ia)",       &g_TelemetryAddr.Ia,                NULL,      "A",        &g_CalibrateFactor[CALIFACTOR_Ia],     &g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  SECONDRY,  "Ib",          "相电流(Ib)",       &g_TelemetryAddr.Ib,                NULL,      "A",        &g_CalibrateFactor[CALIFACTOR_Ib],     &g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  SECONDRY,  "Ic",          "相电流(Ic)",       &g_TelemetryAddr.Ic,                NULL,      "A",        &g_CalibrateFactor[CALIFACTOR_Ic],     &g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},	
    { 1,  SECONDRY,  "I0",          "相电流(I0)",       &g_TelemetryAddr.I0,                NULL,      "A",        &g_CalibrateFactor[CALIFACTOR_I0],     &g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},
    { 1,  SECONDRY,  "Uab",         "线电压(Uab)",      &g_TelemetryAddr.Uab,               NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_Uab],    &g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},
    { 1,  SECONDRY,  "Ubc",         "线电压(Ubc)",      &g_TelemetryAddr.Ucb,               NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_Ucb],    &g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  SECONDRY,  "Uac",         "线电压(Uac)",      &g_TelemetryAddr.Uac,               NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_Uac],    &g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  SECONDRY,  "U0",          "电压(U0)",         &g_TelemetryAddr.U0,                NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_U0],     &g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  SECONDRY,  "UAB",         "线电压(UAB)",      &g_TelemetryAddr.UAB,               NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_UAB],    &g_Parameter[ZERODRIFT_UAB],    &g_Parameter[DEADZONE_UAB]},
    { 1,  SECONDRY,  "UBC",         "线电压(UBC)",      &g_TelemetryAddr.UCB,               NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_UCB],    &g_Parameter[ZERODRIFT_UCB],    &g_Parameter[DEADZONE_UCB]},
    { 1,  SECONDRY,  "P",           "有功功率(P)",      &g_TelemetryAddr.P,                 NULL,      "kW",       &g_CalibrateFactor[CALIFACTOR_P],      &g_Parameter[ZERODRIFT_P],      &g_Parameter[DEADZONE_P]},
    { 1,  SECONDRY,  "Q",           "无功功率(Q)",      &g_TelemetryAddr.Q,                 NULL,      "kVar",     &g_CalibrateFactor[CALIFACTOR_Q],      &g_Parameter[ZERODRIFT_Q],      &g_Parameter[DEADZONE_Q]},
    { 1,  SECONDRY,  "S",           "视在功率(S)",      &g_TelemetryAddr.S,                 NULL,      "kVA",      &g_CalibrateFactor[CALIFACTOR_S],      &g_Parameter[ZERODRIFT_S],      &g_Parameter[DEADZONE_S]},
    { 1,  SECONDRY,  "pf",          "功率因数(pf)",     &g_TelemetryAddr.PF,                NULL,      "-",        &g_CalibrateFactor[CALIFACTOR_PF],     &g_Parameter[ZERODRIFT_PF],     &g_Parameter[DEADZONE_PF]},
    { 1,  SECONDRY,  "DC1",         "直流(DC1)",        &g_TelemetryAddr.DC1,               NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_DC1],    &g_Parameter[ZERODRIFT_DC1],    &g_Parameter[DEADZONE_DC1]},
    { 0,  SECONDRY,  "DC2",         "直流(DC2)",        &g_TelemetryAddr.DC2,               NULL,      "V",        &g_CalibrateFactor[CALIFACTOR_DC2],    &g_Parameter[ZERODRIFT_DC2],    &g_Parameter[DEADZONE_DC2]},
    { 1,  SECONDRY,  "T",           "温度(T)",          &g_TelemetryAddr.T,                 NULL,      "°C",       &g_CalibrateFactor[CALIFACTOR_T],      &g_Parameter[ZERODRIFT_T],      &g_Parameter[DEADZONE_T]},

	{ 0,  SECONDRY,  "∠Uab_Ia",    "∠Uab_Ia",         &g_TelemetryAddr.alphy_Uab_Ia,      NULL,      "°",        &g_CalibrateFactor[CALIFACTOR_ALPHA_UabIa],     NULL,                            NULL},
    { 0,  SECONDRY,  "∠Ucb_Ic",    "∠Ucb_Ic",         &g_TelemetryAddr.alphy_Ucb_Ic,      NULL,      "°",        &g_CalibrateFactor[CALIFACTOR_ALPHA_UcbIc],     NULL,                            NULL},	
    { 1,  SECONDRY,  "∠U0_I0",     "∠U0_I0",          &g_TelemetryAddr.alphy_U0_I0,       NULL,      "°",        &g_CalibrateFactor[CALIFACTOR_ALPHA_U0I0],      NULL,                            NULL},
    { 1,  SECONDRY,  "∠Ux_Ux",     "∠Ux_Ux",          &g_TelemetryAddr.alphy_Ux_Ux,       NULL,      "°",        &g_CalibrateFactor[CALIFACTOR_ALPHA_UxUx],      NULL,                            NULL},	
	
    { 1,  ONCE,      "Ia",          "一次Ia",           &g_TelemetryAddr.IaOnce,            NULL,      "A",       NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "Ib",          "一次Ib",           &g_TelemetryAddr.IbOnce,            NULL,      "A",       NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "Ic",          "一次Ic",           &g_TelemetryAddr.IcOnce,            NULL,      "A",       NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "I0",          "一次I0",           &g_TelemetryAddr.I0Once,            NULL,      "A",       NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "Uab",         "一次Uab",          &g_TelemetryAddr.UabOnce,           NULL,      "kV",      NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "Ubc",         "一次Ubc",          &g_TelemetryAddr.UbcOnce,           NULL,      "kV",      NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "Uca",         "一次Uca",          &g_TelemetryAddr.UcaOnce,           NULL,      "kV",      NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "U0",          "一次U0",           &g_TelemetryAddr.U0Once,            NULL,      "kV",      NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "UAB",         "一次UAB",          &g_TelemetryAddr.UABOnce,           NULL,      "kV",      NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "UBC",         "一次UBC",          &g_TelemetryAddr.UBCOnce,           NULL,      "kV",      NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "P",           "一次P",            &g_TelemetryAddr.POnce,             NULL,      "kW",      NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "Q",           "一次Q",            &g_TelemetryAddr.QOnce,             NULL,      "kVar",    NULL,                                  NULL,                           NULL},
    { 1,  ONCE,      "S",           "一次S",            &g_TelemetryAddr.SOnce,             NULL,      "kVA",     NULL,                                  NULL,                           NULL},		

    { 1,  HARMONIC,  "三次Uab",     "三次谐波Uab",      &g_TelemetryAddr.thirdharmonicUab,  NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_Uab],    &g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},	
    { 1,  HARMONIC,  "三次Ubc",     "三次谐波Ubc",      &g_TelemetryAddr.thirdharmonicUbc,  NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_Ucb],    &g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  HARMONIC,  "三次Uca",     "三次谐波Uca",      &g_TelemetryAddr.thirdharmonicUca,  NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_Uac],    &g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  HARMONIC,  "三次U0",      "三次谐波U0",       &g_TelemetryAddr.thirdharmonicU0,   NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_U0],     &g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  HARMONIC,  "三次Ia",      "三次谐波Ia",       &g_TelemetryAddr.thirdharmonicIa,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_Ia],     &g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  HARMONIC,  "三次Ib",      "三次谐波Ib",       &g_TelemetryAddr.thirdharmonicIb,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_Ib],     &g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  HARMONIC,  "三次Ic",      "三次谐波Ic",       &g_TelemetryAddr.thirdharmonicIc,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_Ic],     &g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},
    { 1,  HARMONIC,  "三次I0",      "三次谐波I0",       &g_TelemetryAddr.thirdharmonicI0,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_I0],     &g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},	
    { 1,  HARMONIC,  "五次Uab",     "五次谐波Uab",      &g_TelemetryAddr.fifthharmonicUab,  NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_Uab],    &g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},	
    { 1,  HARMONIC,  "五次Ubc",     "五次谐波Ubc",      &g_TelemetryAddr.fifthharmonicUbc,  NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_Ucb],    &g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  HARMONIC,  "五次Uca",     "五次谐波Uca",      &g_TelemetryAddr.fifthharmonicUca,  NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_Uac],    &g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  HARMONIC,  "五次U0",      "五次谐波U0",       &g_TelemetryAddr.fifthharmonicU0,   NULL,      "V",       &g_CalibrateFactor[CALIFACTOR_U0],     &g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  HARMONIC,  "五次Ia",      "五次谐波Ia",       &g_TelemetryAddr.fifthharmonicIa,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_Ia],     &g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  HARMONIC,  "五次Ib",      "五次谐波Ib",       &g_TelemetryAddr.fifthharmonicIb,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_Ib],     &g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  HARMONIC,  "五次Ic",      "五次谐波Ic",       &g_TelemetryAddr.fifthharmonicIc,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_Ic],     &g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},
    { 1,  HARMONIC,  "五次I0",      "五次谐波I0",       &g_TelemetryAddr.fifthharmonicI0,   NULL,      "A",       &g_CalibrateFactor[CALIFACTOR_I0],     &g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},		
	
};	

/* 遥控 */
struct tagTelecontrolCfg TelecontrolCfg[] =
{
    {1, "远方遥控",   13,  {"分闸选择失败", "分闸选择成功", "分闸执行失败", "分闸执行成功", "合闸选择失败", "合闸选择成功", "合闸执行失败", "合闸执行成功", "遥控取消","合环选择失败", "合环选择成功", "合环执行失败", "合环执行成功"}},
    {1, "远方活化",    2,  {"失败", "成功"}},	
	{1, "远方复位",    2,  {"失败", "成功"}},
    {1, "远方清除记录",2,  {"失败", "成功"}},
    {1, "远方保护压板",13, {"退出选择失败", "退出选择成功", "退出执行失败", "退出执行成功", "投入选择失败", "投入选择成功", "投入执行失败", "投入执行成功", "遥控取消","", "", "", ""}},	
    {0, "本地操作",   13,  {"分闸选择失败", "分闸选择成功", "分闸执行失败", "分闸执行成功", "合闸选择失败", "合闸选择成功", "合闸执行失败", "合闸执行成功", "遥控取消","合环选择失败", "合环选择成功", "合环执行失败", "合环执行成功"}},
	{0, "本地复位",    2,  {"失败", "成功"}},
    {0, "本地清除记录",2,  {"失败", "成功"}},
    {0, "手持操作",   13,  {"分闸选择失败", "分闸选择成功", "分闸执行失败", "分闸执行成功", "合闸选择失败", "合闸选择成功", "合闸执行失败", "合闸执行成功", "遥控取消","合环选择失败", "合环选择成功", "合环执行失败", "合环执行成功"}},	
};

/* 固有参数 */
struct tagInherentParaCfg InherentParaCfg[] = 
{
    {1, "终端类型",             g_InherentPara.terminalType},  
    {1, "终端操作系统",         g_InherentPara.terminalOpreatSystem}, 
    {1, "终端制造商",           g_InherentPara.terminalManufacturer}, 	
    {1, "终端硬件版本",         g_InherentPara.terminalHardwareVersion},  
    {1, "终端软件版本",         g_InherentPara.terminalSoftwareVersion}, 
    {1, "软件版本校验码",       g_InherentPara.terminalSoftwareVersionCheckCode}, 	
    {1, "通讯规约类型",         g_InherentPara.terminalCommunicationProtocolType},  
    {1, "终端出厂型号",         g_InherentPara.terminalFactoryModel}, 
    {1, "终端ID号",             g_InherentPara.terminalID}, 		
    {1, "终端网卡MAC地址",      g_InherentPara.terminalMAC}, 
    {1, "终端产品序列号",       g_InherentPara.terminalProductSerialNumber}, 	
};
	
/* 校准系数 */
struct tagCalibrateFactor CalibrateFactorCfg[] = 
{
//   使能          名称                  校准值	          点号                             系数值                            系数上限值       系数下限值      系数缺省值
    { 1,    "频率(f)校准系数",           50.00f,   &g_TelemetryAddr.F,                &g_CalibrateFactor[CALIFACTOR_F],          1.2f,            0.8f,           1.0f},
    { 1,    "A相电流(Ia)校准系数",       5.00f,    &g_TelemetryAddr.Ia,               &g_CalibrateFactor[CALIFACTOR_Ia],         0.005f,         0.0035f,         0.00432f},
    { 1,    "B相电流(Ib)校准系数",       5.00f,    &g_TelemetryAddr.Ib,               &g_CalibrateFactor[CALIFACTOR_Ib],         0.005f,         0.0035f,         0.00432},
    { 1,    "C相电流(Ic)校准系数",       5.00f,    &g_TelemetryAddr.Ic,               &g_CalibrateFactor[CALIFACTOR_Ic],         0.005f,         0.0035f,         0.00432f},
  #ifdef SYNTHESIS_I0_ENABLE
    { 1,    "零序电流(I0)校准系数",      1.00f,    &g_TelemetryAddr.I0,               &g_CalibrateFactor[CALIFACTOR_I0],         0.005f,         0.0035f,         0.00432f},
  #else		
    { 1,    "零序电流(I0)校准系数",      1.00f,    &g_TelemetryAddr.I0,               &g_CalibrateFactor[CALIFACTOR_I0],         0.001f,         0.0004f,         0.000866f}, 
  #endif		
    { 1,    "线电压(Uab)校准系数",       100.00f,  &g_TelemetryAddr.Uab,              &g_CalibrateFactor[CALIFACTOR_Uab],        0.02f,          0.0065f,         0.0095f},
    { 1,    "线电压(Ucb)校准系数",       100.00f,  &g_TelemetryAddr.Ucb,              &g_CalibrateFactor[CALIFACTOR_Ucb],        0.02f,          0.0065f,         0.0095f},
    { 1,    "线电压(Uac)校准系数",       100.00f,  &g_TelemetryAddr.Uac,              &g_CalibrateFactor[CALIFACTOR_Uac],        0.02f,          0.0065f,         0.0095f},
    { 1,    "线电压(U0)校准系数",        50.00f,   &g_TelemetryAddr.U0,               &g_CalibrateFactor[CALIFACTOR_U0],         0.02f,          0.0065f,         0.0095f},
    { 1,    "线电压(UAB)校准系数",       100.00f,  &g_TelemetryAddr.UAB,              &g_CalibrateFactor[CALIFACTOR_UAB],        0.02f,          0.0065f,         0.0095f},
    { 1,    "线电压(UCB)校准系数",       100.00f,  &g_TelemetryAddr.UCB,              &g_CalibrateFactor[CALIFACTOR_UCB],        0.02f,          0.0065f,         0.0095f},

    { 1,    "直流电压1校准系数",         24,       &g_TelemetryAddr.DC1,              &g_CalibrateFactor[CALIFACTOR_DC1],        0.05f,          0.0001f,         0.0013745f},
    { 1,    "直流电压2校准系数",         24,       &g_TelemetryAddr.DC2,              &g_CalibrateFactor[CALIFACTOR_DC2],        0.05f,          0.0001f,         0.0013745f},
    { 1,    "温度校准系数",              0,        &g_TelemetryAddr.T,                &g_CalibrateFactor[CALIFACTOR_T],          2.0f,           0.0001f,         1.0f},
	
    { 1,    "Uab_Ia夹角校准系数",        0,        &g_TelemetryAddr.alphy_Uab_Ia,     &g_CalibrateFactor[CALIFACTOR_ALPHA_UabIa], 0.5f,           0.29f,           0.01f},
    { 1,    "Ucb_Ic夹角校准系数",        0,        &g_TelemetryAddr.alphy_Ucb_Ic,     &g_CalibrateFactor[CALIFACTOR_ALPHA_UcbIc], 0.5f,           0.29f,           0.01f},
    { 1,    "U0_I0夹角校准系数",         0,        &g_TelemetryAddr.alphy_U0_I0,      &g_CalibrateFactor[CALIFACTOR_ALPHA_U0I0],  0.5f,           0.29f,           0.01f},
    { 1,    "Ux_Ux夹角校准系数",         0,        &g_TelemetryAddr.alphy_Ux_Ux,      &g_CalibrateFactor[CALIFACTOR_ALPHA_UxUx],  0.5f,           0.29f,           0.01f},		
};


/* 定值0区 */
struct tagValueParaCfg ParameterCfg[] = 
{
//   使能  菜单编号          名称                             值                         单位      上限        下限   缺省值    数据类型  小数点位数       显示内容           备注  
    { 1,  ME_BASIC_SET,   "操作机构",        &g_Parameter[OPERATING_MECHANISM],          "-",       1,           0,     0,         2,         0,         {"弹簧", "永磁"    },   " "   },    
    { 1,  ME_BASIC_SET,   "开关类型",        &g_Parameter[SWITCH_TYPE],                  "-",       1,           0,     0,         2,         0,         {"断路器", "负荷开关"},   " "   }, 
    { 1,  ME_BASIC_SET,   "断路器模式",      &g_Parameter[BREAK_WORK_MODE],              "-",       4,           0,     1,         5,         0,         {"无","常规保护","电压时间型","电压电流型","电流计数型"},   " "   },	
    { 1,  ME_BASIC_SET,   "负荷开关模式",    &g_Parameter[LOAD_WORK_MODE],               "-",       4,           0,     0,         5,         0,         {"无","常规保护","电压时间型","电压电流型","分界"},   " "   },
    { 1,  ME_BASIC_SET,   "负荷保护电压",    &g_Parameter[CFG_PRO_VOL_N],                "-",       1,           0,     1,         2,         0,         {"UAB", "UCB"},   " "   },     
    { 1,  ME_BASIC_SET,   "联络开关识别",    &g_Parameter[CONNECT_SWITCH_RECOGNIZE],     "-",       1,           0,     0,         2,         0,         {"手动", "自动"    },   " "   },	
    { 1,  ME_BASIC_SET,   "永磁输出翻转",    &g_Parameter[OUTPUT_OVERTURN],              "-",       1,           0,     0,         2,         0,         {"不翻转", "翻转"  },   " "   }, 
    { 1,  ME_BASIC_SET,   "合闸欠压定值",    &g_Parameter[CAP_UNDERVOLTAGE_CLOSING],     "V",     250.00f,     0.00f,   176,       0,         3,         {"-",  "-"         },   " "   },	
    { 1,  ME_BASIC_SET,   "分闸欠压定值",    &g_Parameter[CAP_UNDERVOLTAGE_OPENING],     "V",     250.00f,     0.00f,   176,       0,         3,         {"-",  "-"         },   " "   }, 
    { 1,  ME_BASIC_SET,   "电容欠压系数",    &g_Parameter[CAP_UNDERVOLTAGE_FACTOR],      "-",      2.00f,      1.00f,   1.2f,      0,         3,         {"-",  "-"         },   " "   },	    
    { 1,  ME_BASIC_SET,   "功率电压ab",      &g_Parameter[CFG_POW_VOL_AB],               "-",       1,           0,     0,        2,         0,         {"Uab", "UAB"},   " "   },   
    { 1,  ME_BASIC_SET,   "功率电压cb",      &g_Parameter[CFG_POW_VOL_CB],               "-",       1,           0,     1,        2,         0,         {"Ucb", "UCB"},   " "   }, 
    { 1,  ME_BASIC_SET,   "一次变比_U",      &g_Parameter[RATIO_U_ONE_TURN],             "V",    100000.00f,   0.00f,   10000.00f, 0,         3,         {"-",  "-"         },   " "    }, 
    { 1,  ME_BASIC_SET,   "二次变比_U",      &g_Parameter[RATIO_U_SECONDARY],            "V",     1000.00f,   0.00f,    220.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_BASIC_SET,   "一次变比_U0",     &g_Parameter[RATIO_U0_ONE_TURN],            "V",     10000.00f,   0.00f,   1,         0,         3,         {"-",  "-"         },   " "    }, 
    { 1,  ME_BASIC_SET,   "二次变比_U0",     &g_Parameter[RATIO_U0_SECONDARY],           "V",     1000.00f,   0.00f,    1,         0,         3,         {"-",  "-"         },   " "    },
    { 1,  ME_BASIC_SET,   "一次变比_I",      &g_Parameter[RATIO_I_ONE_TURN],             "A",    100000.00f,   0.00f,   600,       0,         3,         {"-",  "-"         },   " "    }, 
    { 1,  ME_BASIC_SET,   "二次变比_I",      &g_Parameter[RATIO_I_SECONDARY],            "A",     1000.00f,   0.00f,    5,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_BASIC_SET,   "一次变比_I0",     &g_Parameter[RATIO_I0_ONE_TURN],            "A",     10000.00f,   0.00f,   20,        0,         3,         {"-",  "-"         },   " "    }, 
    { 1,  ME_BASIC_SET,   "二次变比_I0",     &g_Parameter[RATIO_I0_SECONDARY],           "A",     10000.00f,   0.00f,   1,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_BASIC_SET,   "开入防抖时间",    &g_Parameter[DI_SHAKING_TIME],              "ms",     99999,        0,     20.0f,     0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_BASIC_SET,   "合闸脉宽时间",    &g_Parameter[CLOSING_PULSE_TIME],           "ms",     99999,        0,     60,        0,         3,         {"-",  "-"         },   " "    },
    { 1,  ME_BASIC_SET,   "分闸脉宽时间",    &g_Parameter[OPENING_PULSE_TIME],           "ms",     99999,        0,     60,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_BASIC_SET,   "反校时间",        &g_Parameter[REVERSE_TIME],                 "ms",     99999,        0,     10000.0,   0,         0,         {"-",  "-"         },   " "    },	
    { 1,  ME_BASIC_SET,   "电源告警取反",    &g_Parameter[POWERLOSS_NEGATE],             "-",       1,           0,     0,         2,         0,         {"无", "取反"},   " "   },	  
 
    { 1,  ME_ZERODRIFT,   "频率零漂",        &g_Parameter[ZERODRIFT_F],                 "Hz",     99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "A相电流零漂",     &g_Parameter[ZERODRIFT_Ia],                "A",      99.99f,     0.00f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "B相电流零漂",     &g_Parameter[ZERODRIFT_Ib],                "A",      99.99f,     0.00f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "C相电流零漂",     &g_Parameter[ZERODRIFT_Ic],                "A",      99.99f,     0.00f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "零序电流零漂",    &g_Parameter[ZERODRIFT_I0],                "A",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },
    { 1,  ME_ZERODRIFT,   "Uab零漂",         &g_Parameter[ZERODRIFT_Uab],               "V",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "Ucb零漂",         &g_Parameter[ZERODRIFT_Ucb],               "V",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },		
    { 1,  ME_ZERODRIFT,   "Uac零漂",         &g_Parameter[ZERODRIFT_Uac],               "V",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "U0零漂",          &g_Parameter[ZERODRIFT_U0],                "V",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },
    { 1,  ME_ZERODRIFT,   "UAB零漂",         &g_Parameter[ZERODRIFT_UAB],               "V",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "UCB零漂",         &g_Parameter[ZERODRIFT_UCB],               "V",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "有功功率零漂",    &g_Parameter[ZERODRIFT_P],                 "kW",     99.99f,     0.00f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "无功功率零漂",    &g_Parameter[ZERODRIFT_Q],                 "kVar",   99.99f,     0.00f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "视在功率零漂",    &g_Parameter[ZERODRIFT_S],                 "kVA",    99.99f,     0.00f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },		
    { 1,  ME_ZERODRIFT,   "功率因数零漂",    &g_Parameter[ZERODRIFT_PF],                "-",      99.99f,     0.00f,   0.001f,      0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "直流1零漂",       &g_Parameter[ZERODRIFT_DC1],               "V",      99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "直流2零漂",       &g_Parameter[ZERODRIFT_DC2],               "V",      99.99f,     0.00f,   9.9f,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_ZERODRIFT,   "温度零漂",        &g_Parameter[ZERODRIFT_T],                 "°C",     99.99f,     0.00f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
	
    { 1,  ME_DEADEZONE,   "频率死区",        &g_Parameter[DEADZONE_F],                   "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "A相电流死区",     &g_Parameter[DEADZONE_Ia],                  "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "B相电流死区",     &g_Parameter[DEADZONE_Ib],                  "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "C相电流死区",     &g_Parameter[DEADZONE_Ic],                  "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "零序电流死区",    &g_Parameter[DEADZONE_I0],                  "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },
    { 1,  ME_DEADEZONE,   "Uab死区",         &g_Parameter[DEADZONE_Uab],                 "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "Ucb死区",         &g_Parameter[DEADZONE_Ucb],                 "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },		
    { 1,  ME_DEADEZONE,   "Uac死区",         &g_Parameter[DEADZONE_Uac],                 "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "U0死区",          &g_Parameter[DEADZONE_U0],                  "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },
    { 1,  ME_DEADEZONE,   "UAB死区",         &g_Parameter[DEADZONE_UAB],                 "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "UCB死区",         &g_Parameter[DEADZONE_UCB],                 "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "有功功率死区",    &g_Parameter[DEADZONE_P],                   "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "无功功率死区",    &g_Parameter[DEADZONE_Q],                   "%",      99.99f,     0.01f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "视在功率死区",    &g_Parameter[DEADZONE_S],                   "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },		
    { 1,  ME_DEADEZONE,   "功率因数死区",    &g_Parameter[DEADZONE_PF],                  "%",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "直流1死区",       &g_Parameter[DEADZONE_DC1],                 "%",      99.99f,     0.00f,   99.99f,     0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "直流2死区",       &g_Parameter[DEADZONE_DC2],                 "%",      99.99f,     0.00f,   99.99f,     0,         3,         {"-",  "-"         },   " "    },	
    { 1,  ME_DEADEZONE,   "温度死区",        &g_Parameter[DEADZONE_T],                   "%",      99.99f,     0.00f,   99.99f,     0,         3,         {"-",  "-"         },   " "    },
    
    { 1,  ME_UART_COM,    "串口号",          &g_Parameter[UART_PORT],                    "-",       1,           0,     0,          2,         0,         {"串口中", "串口右"},   " "   }, 
    { 1,  ME_UART_COM,    "波特率",          &g_Parameter[UART_BAUDRATE],                "-",       4,           0,     2,          5,         0,         {"2400", "4800", "9600", "38400", "11520"},   " "   }, 
    { 1,  ME_UART_COM,    "数据位",          &g_Parameter[UART_WORDLENGTH],              "-",       9.00f,     5.00f,    8.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_UART_COM,    "停止位",          &g_Parameter[UART_STOPBITS],                "-",       3.00f,     0.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                    
    { 1,  ME_UART_COM,    "校验位",          &g_Parameter[UART_PARITY],                  "-",       2,           0,     0,          3,         0,         {"无校验", "奇校验", "偶校验"},   " "   },                      
    { 1,  ME_UART_COM,    "101模式",         &g_Parameter[UART_BALANMODE],               "-",       1,           0,     1,          2,         0,         {"非平衡", "平衡"},   " "   },                 
    { 1,  ME_UART_COM,    "从站地址",        &g_Parameter[UART_SOURCEADDR],              "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                 
    { 1,  ME_UART_COM,    "地址长度",        &g_Parameter[UART_LINKADDRSIZE],            "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },                    
    { 1,  ME_UART_COM,    "原因长度",        &g_Parameter[UART_ASDUCOTSIZE],             "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },              
    { 1,  ME_UART_COM,    "ASDU地址",        &g_Parameter[UART_ASDUADDR],                "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                
    { 1,  ME_UART_COM,    "ASDU长度",        &g_Parameter[UART_ASDUADDRSIZE],            "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },                         
    
    { 1,  ME_NET_COM,     "上IP_1",         &g_Parameter[NET_IP1_0],                     "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { 1,  ME_NET_COM,     "上IP_2",         &g_Parameter[NET_IP1_1],                     "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "上IP_3",         &g_Parameter[NET_IP1_2],                     "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "上IP_4",         &g_Parameter[NET_IP1_3],                     "-",     255.00f,     0.00f,  100.00f,     0,         0,         {"-",  "-"         },   " "    },    
    { 1,  ME_NET_COM,     "下IP_1",         &g_Parameter[NET_IP2_0],                     "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { 1,  ME_NET_COM,     "下IP_2",         &g_Parameter[NET_IP2_1],                     "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "下IP_3",         &g_Parameter[NET_IP2_2],                     "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "下IP_4",         &g_Parameter[NET_IP2_3],                     "-",     255.00f,     0.00f,  120.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { 1,  ME_NET_COM,     "子网_1",         &g_Parameter[NET_NETMASK_0],                 "-",     255.00f,     0.00f,  255.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { 1,  ME_NET_COM,     "子网_2",         &g_Parameter[NET_NETMASK_1],                 "-",     255.00f,     0.00f,  255.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "子网_3",         &g_Parameter[NET_NETMASK_2],                 "-",     255.00f,     0.00f,  255.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "子网_4",         &g_Parameter[NET_NETMASK_3],                 "-",     255.00f,     0.00f,    0.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "网关_1",         &g_Parameter[NET_GATEWAY_0],                 "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { 1,  ME_NET_COM,     "网关_2",         &g_Parameter[NET_GATEWAY_1],                 "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "网关_3",         &g_Parameter[NET_GATEWAY_2],                 "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "网关_4",         &g_Parameter[NET_GATEWAY_3],                 "-",     255.00f,     0.00f,  254.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "DNS_1",          &g_Parameter[NET_DNS_0],                     "-",     255.00f,     0.00f,  114.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { 1,  ME_NET_COM,     "DNS_2",          &g_Parameter[NET_DNS_1],                     "-",     255.00f,     0.00f,  114.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "DNS_3",          &g_Parameter[NET_DNS_2],                     "-",     255.00f,     0.00f,  114.00f,     0,         0,         {"-",  "-"         },   " "    },
    { 1,  ME_NET_COM,     "DNS_4",          &g_Parameter[NET_DNS_3],                     "-",     255.00f,     0.00f,  114.00f,     0,         0,         {"-",  "-"         },   " "    },                           
    { 1,  ME_NET_COM,     "从站地址",       &g_Parameter[NET_SOURCEADDR],                "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                
    { 1,  ME_NET_COM,     "ASDU地址",       &g_Parameter[NET_ASDUADDR],                  "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },   
}; 

/* 定值一区 */
struct tagValueParaCfg FixedValueCfg1[] = 
{
//   使能  菜单编号       名称                            值                           单位       上限        下限   缺省值     数据类型   小数点位数       显示内容           备注
    { 1,  INTERGHASE,   "过流一段投退",    &g_FixedValue1[OVER_CURRENT_SWITCH1],          "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  INTERGHASE,   "过流一段电流",    &g_FixedValue1[OVER_CURRENT_VALUE1],           "A",     100.00f,     0.00f,   10,         0,         3,         {"-",  "-"         },   " "   }, 
    { 1,  INTERGHASE,   "过流一段时间",    &g_FixedValue1[OVER_CURRENT_TIME1],            "S",     100.00f,     0.00f,   0,          0,         3,         {"-",  "-"         },   " "   },	
    { 1,  INTERGHASE,   "过流二段投退",    &g_FixedValue1[OVER_CURRENT_SWITCH2],          "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  INTERGHASE,   "过流二段电流",    &g_FixedValue1[OVER_CURRENT_VALUE2],           "A",     100.00f,     0.00f,   8,          0,         3,         {"-",  "-"         },   " "   }, 
    { 1,  INTERGHASE,   "过流二段时间",    &g_FixedValue1[OVER_CURRENT_TIME2],            "S",     100.00f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "   },	
    { 1,  INTERGHASE,   "过流三段投退",    &g_FixedValue1[OVER_CURRENT_SWITCH3],          "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  INTERGHASE,   "过流三段电流",    &g_FixedValue1[OVER_CURRENT_VALUE3],           "A",     100.00f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "   }, 
    { 1,  INTERGHASE,   "过流三段时间",    &g_FixedValue1[OVER_CURRENT_TIME3],            "S",     100.00f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "   },	
    { 1,  ZERO_SEQUE,   "零序一段投退",    &g_FixedValue1[OVER_CURRENTI0_SWITCH1],        "-",       1,           0,     0,          3,         0,         {"退", "投", "告警"},   " "   },    
    { 1,  ZERO_SEQUE,   "零序一段电流",    &g_FixedValue1[OVER_CURRENTI0_VALUE1],         "A",     100.00f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "   }, 
    { 1,  ZERO_SEQUE,   "零序一段时间",    &g_FixedValue1[OVER_CURRENTI0_TIME1],          "S",     100.00f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "   },	    
    { 1,  ZERO_SEQUE,   "零序二段投退",    &g_FixedValue1[OVER_CURRENTI0_SWITCH2],        " ",       1,           0,     3,          3,         0,         {"退", "投", "告警"},   " "    },    
    { 1,  ZERO_SEQUE,   "零序二段电流",    &g_FixedValue1[OVER_CURRENTI0_VALUE2],         "A",     100.00f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    }, 
    { 1,  ZERO_SEQUE,   "零序二段时间",    &g_FixedValue1[OVER_CURRENTI0_TIME2],          "S",     100.00f,     0.00f,   0,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "越限告警投退",    &g_FixedValue1[OVERLIMIT_SWITCH],              "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },
    { 1,  LIMITATION,   "越限上限系数",    &g_FixedValue1[UPLIMIT_FACTOR],                "-",     1.00f,       0.00f,   0.8f,       0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "越限下限系数",    &g_FixedValue1[DOWNLIMIT_FACTOR],              "-",     2.00f,       1.00f,   1.2,        0,         3,         {"-",  "-"         },   " "    },
    { 1,  LIMITATION,   "越限延时",        &g_FixedValue1[OVERLIMIT_TIME],                "S",     999.99f,       0,     3,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "Uab越限投退",     &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uab],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },
    { 1,  LIMITATION,   "Ubc越限投退",     &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uab],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },
    { 1,  LIMITATION,   "Uac越限投退",     &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uca],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "UAB越限投退",     &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uab],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },
    { 1,  LIMITATION,   "UBC越限投退",     &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_UBC],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "电压上限定值",    &g_FixedValue1[UPLIMIT_VOLTAGE_U],             "V",     999.99f,     0.00f,   110,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "电压下限定值",    &g_FixedValue1[DOWNLIMIT_VOLTAGE_U],           "V",     999.99f,     0.00f,   20,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "Ia越限投退",      &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IA],     "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },
    { 1,  LIMITATION,   "Ib越限投退",      &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IB],     "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "Ic越限投退",      &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IC],     "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "电流上限定值",    &g_FixedValue1[UPLIMIT_CURRENT_I],             "A",     999.99f,     0.00f,   6,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "电流下限定值",    &g_FixedValue1[DOWNLIMIT_CURRENT_I],           "A",     999.99f,     0.00f,   0.5,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "U0越限投退",      &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_U0],     "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "U0上限定值",      &g_FixedValue1[UPLIMIT_VOLTAGE_U0],            "V",     999.99f,     0.00f,   100,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "U0下限定值",      &g_FixedValue1[DOWNLIMIT_VOLTAGE_U0],          "V",     999.99f,     0.00f,   20,         0,         3,         {"-",  "-"         },   " "    },
    { 1,  LIMITATION,   "I0越限投退",      &g_FixedValue1[OVERLIMIT_ALARM_SWITCH_I0],     "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "I0上限定值",      &g_FixedValue1[UPLIMIT_VOLTAGE_I0],            "A",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "I0下限定值",      &g_FixedValue1[DOWNLIMIT_VOLTAGE_I0],          "A",     999.99f,     0.00f,   0.2f,       0,         3,         {"-",  "-"         },   " "    },		
    { 1,  LIMITATION,   "U_DC越限投退",    &g_FixedValue1[OVERLIMIT_ALARMSWITCH_DC_U],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "U_DC上限定值",    &g_FixedValue1[UPLIMIT_DC_VOLTAGE_U],          "V",     999.99f,     0.00f,   30,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "U_DC下限定值",    &g_FixedValue1[DOWNLIMIT_DC_VOLTAGE_U],        "V",     999.99f,     0.00f,   18,         0,         3,         {"-",  "-"         },   " "    },
    { 1,  LIMITATION,   "I_DC越限投退",    &g_FixedValue1[OVERLIMIT_ALARMSWITCH_DC_I],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  LIMITATION,   "I_DC上限定值",    &g_FixedValue1[UPLIMIT_DC_CURRENT_I],          "A",     999.99f,     0.00f,   0,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMITATION,   "I_DC下限定值",    &g_FixedValue1[DOWNLIMIT_DC_CURRENT_I],        "A",     999.99f,     0.00f,   0,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  HEAVY_LOAD,   "重过载投退",      &g_FixedValue1[HEAVY_OVERLOAD_SWITCH],         "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  HEAVY_LOAD,   "重过载定值",      &g_FixedValue1[HEAVY_OVERLOAD_VALUE],          "A",     999.99f,     0.00f,   10,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  HEAVY_LOAD,   "重过载系数",      &g_FixedValue1[HEAVY_OVERLOAD_FACTOR],         "-",      1.00f,      0.01f,   0.8f,       0,         3,         {"-",  "-"         },   " "    },	
    { 1,  HEAVY_LOAD,   "重过载延时",      &g_FixedValue1[HEAVY_OVERLOAD_TIME],           "S",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },		
    { 1,  OVERLOAD,     "过负荷投退",      &g_FixedValue1[OVERLOAD_SWITCH],               "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  OVERLOAD,     "过负荷定值",      &g_FixedValue1[OVERLOAD_VALUE],                "A",     999.99f,     0.00f,   8,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  OVERLOAD,     "过负荷系数",      &g_FixedValue1[OVERLOAD_FACTOR],               "-",      1.00f,      0.01f,   0.8,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  OVERLOAD,     "过负荷延时",      &g_FixedValue1[OVERLOAD_TIME],                 "S",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  BATTERY_SET,  "低压告警投退",    &g_FixedValue1[BATTERY_LOWVOLTAGE_ALARM_SWITCH],"-",       1,          0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  BATTERY_SET,  "电池低压定值",    &g_FixedValue1[BATTERY_LOWVOLTAGE_VALUE],      "A",     999.99f,     0.00f,   18,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  BATTERY_SET,  "电池低压系数",    &g_FixedValue1[BATTERY_LOWVOLTAGE_FACTOR],     "-",      2.00f,      1.01f,   1.2f,       0,         3,         {"-",  "-"         },   " "    },	
    { 1,  BATTERY_SET,  "电池低压延时",    &g_FixedValue1[BATTERY_LOWVOLTAGE_TIME],       "S",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },		
    { 1,  BATTERY_SET,  "电池活化投退",    &g_FixedValue1[BATTERY_ACTIVE_SWITCH],         "-",        1,          0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  BATTERY_SET,  "电池活化周期",    &g_FixedValue1[BATTERY_ACTIVE_CYCLE],          "天",    99999.99f,   0.00f,   0,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  BATTERY_SET,  "活化持续时间",    &g_FixedValue1[BATTERY_ACTIVE_TIME],           "h",       23,          0,     0,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  BATTERY_SET,  "活化故障电压",    &g_FixedValue1[BATTERY_ACTIVE_FAULT_VOLTAGE],  "V",     999.99f,     0.00f,   0,          0,         3,         {"-",  "-"         },   " "    },
    { 1,  BATTERY_SET,  "活化故障延时",    &g_FixedValue1[BATTERY_ACTIVE_FAULT_TIME],     "S",     999.99f,     0.00f,   0,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  AUTO_RESET,   "自动复归投退",    &g_FixedValue1[AUTOMATIC_RESET_SWITCH],        "-",        1,          0,     0,          2,         0,         {"退", "投"        },   " "    },	
    { 1,  AUTO_RESET,   "自动复归延时",    &g_FixedValue1[AUTOMATIC_RESET_TIME],          "S",     99999.99f,   0.00f,   99,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "过电压投退",      &g_FixedValue1[OVERVOLTAGE_SWITCH],            "-",       1,           0,     0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { 1,  LIMIT_V_F,    "过电压定值",      &g_FixedValue1[OVERVOLTAGE_VALUE],             "V",     9999.99f,    0.00f,   110,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "过电压延时",      &g_FixedValue1[OVERVOLTAGE_TIME],              "S",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "过电压系数",      &g_FixedValue1[OVERVOLTAGE_FACTOR],            "-",      1.00f,      0.00f,   0.8f,       0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "过频投退",        &g_FixedValue1[OVERFREQUENCY_SWITCH],          "-",       1,           0,     0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { 1,  LIMIT_V_F,    "过频定值",        &g_FixedValue1[OVERFREQUENCY_VALUE],           "Hz",     99.99f,     0.00f,   60,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "过频延时",        &g_FixedValue1[OVERFREQUENCY_TIME],            "S",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "过频系数",        &g_FixedValue1[OVERFREQUENCY_FACTOR],          "-",      1.00f,      0.00f,   0.8f,       0,         3,         {"-",  "-"         },   " "    },		
    { 1,  LIMIT_V_F,    "低电压投退",      &g_FixedValue1[DOWNVOLTAGE_SWITCH],            "-",       1,           0,     0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { 1,  LIMIT_V_F,    "低电压定值",      &g_FixedValue1[DOWNVOLTAGE_VALUE],             "V",     99999.99f,   0.00f,   30,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "低电压延时",      &g_FixedValue1[DOWNVOLTAGE_TIME],              "S",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "低电压系数",      &g_FixedValue1[DOWNVOLTAGE_FACTOR],            "-",      2.00f,      1.00f,   1.2,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "低频投退",        &g_FixedValue1[DOWNFREQUENCY_SWITCH],          "-",       1,           0,     0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { 1,  LIMIT_V_F,    "低频定值",        &g_FixedValue1[DOWNFREQUENCY_VALUE],           "Hz",     99.99f,     0.00f,   45,         0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "低频延时",        &g_FixedValue1[DOWNFREQUENCY_TIME],            "S",     999.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LIMIT_V_F,    "低频系数",        &g_FixedValue1[DOWNFREQUENCY_FACTOR],          "-",      2.00f,      1.00f,   1.2f,       0,         3,         {"-",  "-"         },   " "    },		
    { 1,  LOOP_CLOSE,   "合环投退",        &g_FixedValue1[CLOSING_LOOP_SWITCH],           "-",       1,           0,     0,          2,         0,         {"退", "投",       },   " "    },	
    { 1,  LOOP_CLOSE,   "两侧压差",        &g_FixedValue1[VOLTAGE_DIFFERENCE],            "V",      30.00f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LOOP_CLOSE,   "相角差",          &g_FixedValue1[PHASEANGLE_DIFFERENCE],         "°",      10.00f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },
    { 1,  LOOP_CLOSE,   "频率差",          &g_FixedValue1[FREQUENCY_DIFFERENCE],          "Hz",     99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },	
    { 1,  FAULT_SWITCH, "控制回路异常",    &g_FixedValue1[CONTROL_LOOP_ABNOMAL_ENABLE],   "-",       1,           0,     0,          2,         0,         {"退", "投",       },   " "    },		
    { 1,  OTHER_PROTEC, "反时限投退",      &g_FixedValue1[INVERSE_SWITCH],                "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  OTHER_PROTEC, "反时限定值",      &g_FixedValue1[INVERSE_CURRENT_VALUE],         "A",      99.99f,     0.00f,   10,         0,         3,         {"-",  "-"         },   " "   }, 
    { 1,  OTHER_PROTEC, "反时限延时",      &g_FixedValue1[INVERSE_TIME],                  "S",      99.99f,     0.00f,   0,          0,         3,         {"-",  "-"         },   " "   },	
    { 1,  OTHER_PROTEC, "后加速投退",      &g_FixedValue1[IACC_SWITCH],                   "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  OTHER_PROTEC, "后加速定值",      &g_FixedValue1[IACC_CURRENT_VALUE],            "A",      99.99f,     0.00f,   6,          0,         3,         {"-",  "-"         },   " "   }, 
    { 1,  OTHER_PROTEC, "后加速延时",      &g_FixedValue1[IACC_TIME],                     "S",      99.99f,     0.00f,   1,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "涌流抑制投退",    &g_FixedValue1[INRUSH_SWITCH],                 "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  OTHER_PROTEC, "涌流抑制延时",    &g_FixedValue1[INRUSH_TIME],                   "S",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "   },	
    { 1,  OTHER_PROTEC, "二次重合闭锁",    &g_FixedValue1[SECONDARY_RECLOSE_LOCK_SWITCH], "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  OTHER_PROTEC, "重合闭锁时间",    &g_FixedValue1[SECONDARY_RECLOSE_LOCK_TIME],   "S",      99.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "相间重合次数",    &g_FixedValue1[RECLOSE_TIMES_I],               "次",      4,           0,     2,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "零序重合次数",    &g_FixedValue1[RECLOSE_TIMES_I0],              "次",      2,           0,     1,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "一次重合时间",    &g_FixedValue1[RECLOSE_TIMER1],                "S",      99.99f,     0.00f,   2,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "二次重合时间",    &g_FixedValue1[RECLOSE_TIMER2],                "S",      99.99f,     0.00f,   10,         0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "三次重合时间",    &g_FixedValue1[RECLOSE_TIMER3],                "S",      99.99f,     0.00f,   15,         0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "四次重合时间",    &g_FixedValue1[RECLOSE_TIMER4],                "S",      99.99f,     0.00f,   20,         0,         3,         {"-",  "-"         },   " "   },	
    { 1,  OTHER_PROTEC, "重合复位时间",    &g_FixedValue1[RE_RESET_TIME],                 "S",      99.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  OTHER_PROTEC, "重合充电时间",    &g_FixedValue1[RE_CHARGE_TIME],                "S",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  LOGICAL_FUN,  "有流定值",        &g_FixedValue1[CURRENT_VALUE],                 "A",      99.99f,     0.00f,   0.2f,       0,         3,         {"-",  "-"         },   " "   },
    { 1,  LOGICAL_FUN,  "有压定值",        &g_FixedValue1[VOLTAGE_VALUE],                 "V",     9999.99f,    0.00f,   176,        0,         3,         {"-",  "-"         },   " "   },	
    { 1,  LOGICAL_FUN,  "无压定值",        &g_FixedValue1[NONVOLTAGE_VALUE],              "V",     9999.99f,    0.00f,   80,         0,         3,         {"-",  "-"         },   " "   },
    { 1,  LOGICAL_FUN,  "Y时限",           &g_FixedValue1[FAULT_CHECK_Y_TIME],            "S",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "   },
    { 1,  LOGICAL_FUN,  "闭锁分闸时间",    &g_FixedValue1[SHORT_LOCKED_OPENING_TIME],     "S",      99.99f,     0.00f,   10,         0,         3,         {"-",  "-"         },   " "   },
    { 1,  LOGICAL_FUN,  "失电分闸投退",    &g_FixedValue1[LOSS_ELECTRICITY_SWITCH],       "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  LOGICAL_FUN,  "失电分闸时间",    &g_FixedValue1[LOSS_OPENING_TIME],             "S",      99.99f,     0.00f,   3,          0,         3,         {"-",  "-"         },   " "   },		
    { 1,  LOGICAL_FUN,  "得电合闸投退",    &g_FixedValue1[GET_VOLTAGE_CLOSSING_SWITCH],   "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  LOGICAL_FUN,  "得电合闸时间",    &g_FixedValue1[GET_VOLTAGE_CLOSSING_X_TIME],   "S",      99.99f,     0.00f,   7,          0,         3,         {"-",  "-"         },   " "   },	
    { 1,  LOGICAL_FUN,  "单侧失压合闸",    &g_FixedValue1[SINGLE_LOSS_VOLTAGE_SWITCH],    "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  LOGICAL_FUN,  "单侧失压时间",    &g_FixedValue1[SINGLE_LOSS_VOLTAGE_XLTIME],    "S",      99.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "   },	
    { 1,  LOGICAL_FUN,  "双侧有压投退",    &g_FixedValue1[DOUBLE_VOLTAGE_SWITCH],         "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },  
    { 1,  LOGICAL_FUN,  "残压脉冲投退",    &g_FixedValue1[REMAIN_VOLTAGE_SWITCH],         "-",       1,           0,     0,          2,         0,         {"退", "投"        },   " "   },    
    { 1,  LOGICAL_FUN,  "残压整定电压",    &g_FixedValue1[REMAIN_VOLTAGE_VALUE],          "V",      99.99f,     0.00f,   30,         0,         3,         {"-",  "-"         },   " "   },	
    { 1,  LOGICAL_FUN,  "U0保护投退",      &g_FixedValue1[VOLTAGE_U0_SWITCH],             "-",       1,           0,     0,          2,         0,         {"退", "投",       },   " "    },	
    { 1,  LOGICAL_FUN,  "U0保护电压",      &g_FixedValue1[VOLTAGE_U0_VALUE],              "V",      99.99f,     0.00f,   100,        0,         3,         {"-",  "-"         },   " "    },	
    { 1,  LOGICAL_FUN,  "U0保护延时",      &g_FixedValue1[VOLTAGE_U0_TIME],               "S",      99.99f,     0.00f,   1,          0,         3,         {"-",  "-"         },   " "    },
    { 1,  LOGICAL_FUN,  "非遮断投退",      &g_FixedValue1[BREAKING_CURRENT_SWITCH],       "-",       1,           0,     0,          2,         0,         {"退", "投",       },   " "    },	
    { 1,  LOGICAL_FUN,  "非遮断电流",      &g_FixedValue1[BREAKING_CURRENT_VALUE],        "A",     999.99f,     0.00f,   5,          0,         3,         {"-",  "-"         },   " "    },		
}; 

/* 定值二区 */
struct tagValueParaCfg FixedValueCfg2[sizeof(FixedValueCfg1) / sizeof(FixedValueCfg1[0])];

<<<<<<< HEAD
=======
//配置信息
ConfigurationSetDatabaseToJson SetDatabaseCfg[] = {
//    名称                    数据长度            数据
//串口配置
    {.name = "串口配置",          .dataLen = 1,       .value = {1.00f}   },
    {.name = "波特率",            .dataLen = 1,       .value = {0.0f}   },
    {.name = "数据位",            .dataLen = 1,       .value = {0.0f}   },
    {.name = "停止位",            .dataLen = 1,       .value = {0.0f}   },
    {.name = "校验",              .dataLen = 1,       .value = {0.0f}   },
//串口设置
    {.name = "通讯模式",          .dataLen = 1,       .value = {0.0f}   },
    {.name = "从站地址",          .dataLen = 1,       .value = {0.0f}   },
    {.name = "从站地址长度",      .dataLen = 1,       .value = {0.0f}   },
    {.name = "传送原因字节数",    .dataLen = 1,       .value = {0.0f}   },
    {.name = "ASDU地址",         .dataLen = 1,       .value = {0.0f}   },
    {.name = "ASDU地址长度",     .dataLen = 1,       .value = {0.0f}   },
//网口配置
    {.name = "上行IP",           .dataLen = 4,       .value = {0.0f}   },
    {.name = "下行IP",           .dataLen = 4,       .value = {0.0f}   },
    {.name = "子网掩码",         .dataLen = 4,       .value = {0.0f}   },
    {.name = "网关",             .dataLen = 4,       .value = {0.0f}   },
    {.name = "DNS",             .dataLen = 4,       .value = {0.0f}   },
//网口设置
    {.name = "从站地址",         .dataLen = 1,       .value = {0.0f}   },
    {.name = "ASDU地址",         .dataLen = 1,       .value = {0.0f}   },
//YX设置
    {.name = "YX设置总数",       .dataLen = 1,       .value = {0.0f}   },
    {.name = "YX设置",           .dataLen = 200,     .value = {0.0f}   },
//YC设置
    {.name = "YC值",            .dataLen = 100,      .value = {0.0f}   },
    {.name = "属性",            .dataLen = 100,      .value = {0.0f}   },
    {.name = "倍率",            .dataLen = 100,      .value = {0.0f}   },
//YK设置
    {.name = "YK值",           .dataLen = 1,         .value = {0.0f}   },
    {.name = "属性",           .dataLen = 1,         .value = {0.0f}   },
    //ID设置 
    {.name = "ID",             .dataLen = 12,       .value = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}   },

};



>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290
unsigned int g_FixedValueCfg1_Len = sizeof(FixedValueCfg1) / sizeof(FixedValueCfg1[0]);
unsigned int g_FixedValueCfg2_Len = sizeof(FixedValueCfg2) / sizeof(FixedValueCfg2[0]);
unsigned int g_ParameterCfg_Len = sizeof(ParameterCfg) / sizeof(ParameterCfg[0]);
unsigned int g_CalibrateFactorCfg_Len = sizeof(CalibrateFactorCfg) / sizeof(CalibrateFactorCfg[0]);
unsigned int g_TelemetryCfg_Len = sizeof(TelemetryCfg) / sizeof(TelemetryCfg[0]);
unsigned int g_TelesignalCfg_Len = sizeof(TelesignalCfg) / sizeof(TelesignalCfg[0]);
<<<<<<< HEAD
unsigned int g_TelecontrolCfg_Len = sizeof(TelecontrolCfg) / sizeof(TelecontrolCfg[0]);
unsigned int g_InherentParaCfg_Len = sizeof(InherentParaCfg) / sizeof(InherentParaCfg[0]);
=======
unsigned int g_SetDatabaseCfg_Len = sizeof(SetDatabaseCfg) / sizeof(SetDatabaseCfg[0]);

>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290
/* END OF FILE ---------------------------------------------------------------*/


