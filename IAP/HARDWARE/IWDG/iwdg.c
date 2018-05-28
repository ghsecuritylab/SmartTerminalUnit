#include "iwdg.h" 

IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄

/**
  * @brief:  初始化独立看门狗 时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
  * @param:  prer:分频数:IWDG_PRESCALER_4~IWDG_PRESCALER_256
  * @param:  rlr:自动重装载值,0~0XFFF.
  * @return: None
  */
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=prer;	//设置IWDG分频系数
    IWDG_Handler.Init.Reload=rlr;		//重装载值
    HAL_IWDG_Init(&IWDG_Handler);		//初始化IWDG  
	
    HAL_IWDG_Start(&IWDG_Handler);		//开启独立看门狗
}
    
/**
  * @brief: 看门狗时钟翻转
  * @param:  None
  * @return: None
  */
void WDG_FEED(void)
{
   //HAL_IWDG_Refresh(&IWDG_Handler); 	//喂狗
}

