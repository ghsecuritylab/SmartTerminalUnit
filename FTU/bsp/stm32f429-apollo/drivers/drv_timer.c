/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      timer.c
  * @brief:     The driver of timer.
  * @version:   V1.0.1
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @update:    [2017-09-05][Lexun][make the code cleanup]
  */


/* INCLUDE FILES--------------------------------------------------------------*/
#include "drv_timer.h"
#include "drv_di.h"
#include "drv_do.h"
#include "string.h"
#include "common_data.h"
#include "drv_wdg.h"
#include ".\MultiThread\multi_thread.h"


#if RT_USING_HWTIMER

/* PUBLIC VARIABLES ----------------------------------------------------------*/
struct TagFreGather g_FreGather[FRE_NUM]; // 频率采集

//static SystemCounter SysCounter;

static TIM_HandleTypeDef TIM6_Handler,TIM7_Handler; // 基本定时器定义
static TIM_HandleTypeDef TIM3_Handler; // 输入捕获

static TIM_HandleTypeDef TIM5_Handler; // 定时器5PWM句柄
static TIM_OC_InitTypeDef TIM5_CH1Handler; // 定时器5通道1句柄


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : TIM6 hardware initialization. 
  * @brief ：Timer overflow time calculation method: Tout=((arr+1)*(psc+1))/Ft us.
  * @param : [arr] Automatic reloading value.
  * @param : [psc] Pre clock frequency.
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
void TIM6_Init(uint16_t arr, uint16_t psc)
{
    TIM6_Handler.Instance=TIM6;
    TIM6_Handler.Init.Prescaler=psc;
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM6_Handler.Init.Period=arr;
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; // 时钟分频因子
    HAL_TIM_Base_Init(&TIM6_Handler);

    HAL_TIM_Base_Start_IT(&TIM6_Handler);
}

/**
  * @brief : hardware 1MS TIMER initialization. 
  * @param : None.
  * @return: None.
  */
int rt_hw_tim1ms_init(void)
{
    TIM6_Init(45000-1, 2-1);
    return(0);
}
INIT_ENV_EXPORT(rt_hw_tim1ms_init);


/**
  * @brief: TIM7硬件初始化. 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
  * @param:  arr-自动重装值.
  * @param:  psc-时钟预分频数.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void TIM7_Init(uint16_t arr,uint16_t psc)
{
    TIM7_Handler.Instance=TIM7;
    TIM7_Handler.Init.Prescaler=psc;
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM7_Handler.Init.Period=arr;
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; // 时钟分频因子
    HAL_TIM_Base_Init(&TIM7_Handler);

    HAL_TIM_Base_Start_IT(&TIM7_Handler);
}

/**
  * @brief: TIM10 PWM部分初始化.
  * @param:  arr-自动重装值.
  * @param:  psc-时钟预分频数.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void TIM5_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM5_Handler.Instance=TIM5;
    TIM5_Handler.Init.Prescaler=psc;
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM5_Handler.Init.Period=arr;
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM5_Handler);
    TIM5_CH1Handler.OCMode=TIM_OCMODE_PWM1;
    TIM5_CH1Handler.Pulse=1000;
    TIM5_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&TIM5_Handler,&TIM5_CH1Handler,TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&TIM5_Handler,TIM_CHANNEL_1);
}

/**
  * @brief: 定时器底层驱动，时钟使能，引脚配置，此函数会被HAL_TIM_PWM_Init()调用.
  * @param:  htim-定时器句柄.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_Initure.Pin=GPIO_PIN_10;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate= GPIO_AF2_TIM5;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
}

/* 输入捕获-------------------------------------------------------------------*/
/**
  * @brief: TIM3_CH2硬件初始化. 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
  * @param:  arr-自动重装值.(TIM2,TIM5是32位的!!)
  * @param:  psc-时钟预分频数.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void TIM3_CH1_Cap_Init(uint32_t arr,uint16_t psc)
{
    TIM_IC_InitTypeDef TIM3_CH1Config;

    TIM3_Handler.Instance=TIM3;
    TIM3_Handler.Init.Prescaler=psc;
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM3_Handler.Init.Period=arr;
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM3_Handler);

    TIM3_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获，捕获极性
    TIM3_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;
    TIM3_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频，每个上升沿都会捕获
    TIM3_CH1Config.ICFilter=0;
    HAL_TIM_IC_ConfigChannel(&TIM3_Handler,&TIM3_CH1Config,TIM_CHANNEL_1);

    HAL_TIM_IC_Start_IT(&TIM3_Handler,TIM_CHANNEL_1);   // 开启TIM5的捕获通道1，并且开启捕获中断
    __HAL_TIM_ENABLE_IT(&TIM3_Handler,TIM_IT_UPDATE); // 使能更新中断,不使用更新中断，因为定时器预装值已经足够大了
    // 所谓更新中断就是普通的中断处理函数
}

int rt_hw_frequency_timer_int(void)
{
    TIM3_CH1_Cap_Init(0xffffffff, 90 - 1);
	
    return (0);
}
INIT_BOARD_EXPORT(rt_hw_frequency_timer_int);

int FLASH_OB_Init(void)
{
    uint8_t level = OB_BOR_LEVEL1;
    uint32_t i;
    
    FLASH_OBProgramInitTypeDef OBInit;
    __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
    HAL_FLASHEx_OBGetConfig(&OBInit);
    if(OBInit.BORLevel != level)
    {
        OBInit.OptionType = OPTIONBYTE_BOR;
        OBInit.BORLevel = level;
        FLASH_WaitForLastOperation(0xffff);
        HAL_FLASH_Unlock();
        HAL_FLASH_OB_Unlock();
        i=0xffff;
        while(--i){};
        HAL_FLASHEx_OBProgram(&OBInit);
        i=0xffff;
        while(--i){};
        HAL_FLASH_OB_Launch();
        i=0xffff;
        while(--i){};
        HAL_FLASH_OB_Lock();
        HAL_FLASH_Lock();
        FLASH_WaitForLastOperation(0xffff);
    }
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    return (0);
}
INIT_BOARD_EXPORT(FLASH_OB_Init);

/**
  * @brief: 输入捕获定时器底层驱动，时钟使能，引脚配置，此函数会被HAL_TIM_IC_Init()调用.
  * @param:  htim-定时器句柄.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;

    if (htim->Instance== TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_Initure.Pin=GPIO_PIN_6;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;
        GPIO_Initure.Pull=GPIO_PULLUP ;
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;
        GPIO_Initure.Alternate=GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

        HAL_NVIC_SetPriority(TIM3_IRQn, (INT_TIM3_PRIO >> 4) & 0x0F, INT_TIM3_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

/**
  * @brief: 定时器底层驱动，开启时钟，设置中断优先级.
  * @param:  htim-定时器句柄.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, (INT_TIM6_PRIO>>4)&0x0F, INT_TIM6_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }

//	  if (htim->Instance == TIM7)
//	  {
//		    __HAL_RCC_TIM7_CLK_ENABLE();
//		    HAL_NVIC_SetPriority(TIM7_IRQn, (INT_TIM7_PRIO >> 4) & 0xFF, INT_TIM7_PRIO & 0xFF);
//		    HAL_NVIC_EnableIRQ(TIM7_IRQn);
//	  }
}

/**
  * @brief: 定时器中断服务函数.
  * @param:  无.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void TIM3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
	
    HAL_TIM_IRQHandler(&TIM3_Handler);
	
    /* leave interrupt */
    rt_interrupt_leave();
}

void TIM6_DAC_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
	
    HAL_TIM_IRQHandler(&TIM6_Handler);
	
    /* leave interrupt */
    rt_interrupt_leave();
}

void TIM7_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
	
    HAL_TIM_IRQHandler(&TIM7_Handler);
	
    /* leave interrupt */
    rt_interrupt_leave();
}

/**
  * @brief: 回调函数，定时器中断服务函数调用.
  * @param:  htim-定时器句柄.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static rt_uint32_t cnt;
	static rt_uint32_t s_run_cnt = 0;
    
    if (htim == &TIM6_Handler)
    {   
        g_SystemTime.msecond++;

		rt_hw_di_check_task(1);
		
		/* 线圈储能操作 */ 
		rt_hw_do_operate(ADDR_LOGIC_ACT, DO_COIL_ENERGY_STORAGE);
		
		/* 分闸收回 */
		rt_hw_do_operate(ADDR_LOGIC_ACT, DO_OPEN_RECOVERY); 
				  
		/* 合闸收回 */
		rt_hw_do_operate(ADDR_LOGIC_ACT, DO_CLOSE_RECOVERY); 	
		
		if (s_run_cnt < 5000)
		{
			s_run_cnt++;
		    rt_hw_wdg_deal_task();
		}
        
        if ((cnt % WDG_MS) == 0)
        { 			
            if ((g_ThreadRunSta & 0x19B) == 0x19B)
            {
                rt_hw_wdg_deal_task();      
            }
	        else
	        {
	            s_run_cnt++;
		
		        if (s_run_cnt < 5010)
		        {
                    rt_hw_wdg_deal_task();  		    
		        }
                else
                {
                    s_run_cnt = s_run_cnt;
                }
	        }
			
	        g_ThreadRunSta = 0;			
        }

      #if RT_USING_PROTECT        
        rt_sem_release(&protect_sem);
      #endif /* RT_USING_PROTECT */

      #if RT_USING_WATCH        
        rt_sem_release(&watch_sem); 
      #endif /* RT_USING_WATCH */
        
        if ((cnt % DLT_101SLAVE_MS) == 0)
        { 
          #if RT_USING_SLAVE101 
            rt_event_send(&slave101_event, EVENT_RUN);  
          #endif /* RT_USING_SLAVE101 */
        } 
        
        if ((cnt % DLT_104SLAVE_MS) == 0)
        {
          #if RT_USING_SLAVE104 
            rt_event_send(&slave104_event, EVENT_RUN);
          #endif /* RT_USING_SLAVE104 */
            
          #if RT_USING_W5500            
            rt_sem_release(&w5500_sem);     
          #endif /* RT_USING_W5500 */            
        }
        
        ++cnt;
    }
}
/**
  * @brief: 频率采集处理
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void FreGatherHandle(uint8_t pdrv)
{
    uint8_t i;
    uint32_t fre = 0;

    switch (g_FreGather[pdrv].sta)
    {
    case FRISTTIME:
        memset(&g_FreGather[pdrv], 0, sizeof(struct TagFreGather));
        g_FreGather[pdrv].lastValue = HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_1);
        g_FreGather[pdrv].sta = PREPARE;
        break;

    case PREPARE:
        g_FreGather[pdrv].curValue = HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_1);
        if (g_FreGather[pdrv].curValue > g_FreGather[pdrv].lastValue) // 一个周期内
        {
            g_FreGather[pdrv].data[g_FreGather[pdrv].count] = g_FreGather[pdrv].curValue - g_FreGather[pdrv].lastValue;
        }
        else // 跨周期
        {
            g_FreGather[pdrv].data[g_FreGather[pdrv].count] = 0xffff - g_FreGather[pdrv].lastValue + g_FreGather[pdrv].curValue;
        }
        g_FreGather[pdrv].count = (g_FreGather[pdrv].count + 1) % (FRE_GATHER_NUM);
        g_FreGather[pdrv].num < FRE_GATHER_NUM ? g_FreGather[pdrv].num++ : (g_FreGather[pdrv].sta = VALID);
        g_FreGather[pdrv].lastValue = g_FreGather[pdrv].curValue;
        break;

    case VALID:
        g_FreGather[pdrv].curValue = HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_1);

        if (g_FreGather[pdrv].curValue > g_FreGather[pdrv].lastValue) // 一个周期内
        {
            g_FreGather[pdrv].data[g_FreGather[pdrv].count] = g_FreGather[pdrv].curValue - g_FreGather[pdrv].lastValue;
        }
        else // 跨周期
        {
            g_FreGather[pdrv].data[g_FreGather[pdrv].count] = 0xffff - g_FreGather[pdrv].lastValue + g_FreGather[pdrv].curValue;
        }
        g_FreGather[pdrv].count = (g_FreGather[pdrv].count + 1) % (FRE_GATHER_NUM);
        g_FreGather[pdrv].lastValue = g_FreGather[pdrv].curValue;

        for (i = 0; i < FRE_GATHER_NUM; i++)
        {
            fre += g_FreGather[pdrv].data[i];
        }

        g_FreGather[pdrv].freValue = ((float)(FRE_GATHER_NUM * 1000000) / (float)fre);
        break;
    }
}

/**
  * @brief: 获取频率
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void GetFrequency(uint8_t pdrv)
{
//	static uint32_t arr;
	uint32_t arr = 0, ccr1 = 0;
	static uint32_t arr_backup = 0;
	static uint32_t s_counter;	
	
//    rt_enter_critical(); // 进入临界区

    g_TelemetryDB[g_TelemetryAddr.F] = g_FreGather[pdrv].freValue; // 写入数据缓冲区

    if (g_CalibrateFactor[CALIFACTOR_F] != 0)
    {
        g_TelemetryDB[g_TelemetryAddr.F] *= g_CalibrateFactor[CALIFACTOR_F];
    }
    else
    {
        g_TelemetryDB[g_TelemetryAddr.F] += 0.001f;
    }

    if (g_TelemetryDB[g_TelemetryAddr.F] < g_Parameter[ZERODRIFT_F])
    {
        g_TelemetryDB[g_TelemetryAddr.F] = 0;
    }
	
	g_FreGather[pdrv].freValueProtect = g_TelemetryDB[g_TelemetryAddr.F];
	
    if (g_TelemetryDB[g_TelemetryAddr.Uab] <= 1)
    {
        TIM5->ARR = 6250 - 1; // 若Uab未接入，设定采集PWM为初始值
		g_FreGather[pdrv].freValueProtect = 0;
		
        if (g_TelemetryDB[g_TelemetryAddr.UCB] <= 1)
        {
            g_TelemetryDB[g_TelemetryAddr.F] = 0;
        }
        else
        {
            g_TelemetryDB[g_TelemetryAddr.F] = 50.0f;
        }
    }
    else
    {	
        if (g_TelemetryDB[g_TelemetryAddr.F] < 44.99f || g_TelemetryDB[g_TelemetryAddr.F] > 55.01f)
        {
            g_TelemetryDB[g_TelemetryAddr.F] = 50.0f;
            arr = 6250 - 1; // 变频采集交流量
			ccr1 = 1000;
        }
		else
		{
			arr = (uint32_t)((float)312500.0f / (float)g_TelemetryDB[g_TelemetryAddr.F]) - 1; // 变频采集交流量
			ccr1 = arr / 2 ;
		}
		
		if (++s_counter >= 30)
		{
			s_counter = 0;
		}
		else
		{
			return;
		}
		
		if (arr != arr_backup && (arr > 2000 && arr < 8000))
		{
			TIM5->ARR = arr; // 频率超限，设定采集PWM为初始值
			TIM5->CCR1 = ccr1;
			TIM5->CNT = 0;
		}
		
        arr_backup = arr;
    }
//    rt_exit_critical(); // 退出临界区
}

/**
  * @brief: 定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用.
  * @param:  htim-定时器句柄.
  * @return: 无
  * @updata: [2017-08-07] [更改人姓名][变更描述]
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance== TIM3)
    {
        FreGatherHandle(0);
    }

    if (htim->Instance== TIM2)
    {

    }
}
#endif /* RT_USING_HWTIMER */


#if RT_USING_SFTIMER

/**
  * @description: 定时器回调函数
  * @param:  parameter-入口参数.
  * @return: 无
  * @updata: [2017-10-26] [更改人姓名][变更描述]
  */
static void tmrout1ms_callbak(void* parameter)
{
}

/**
  * @description: 软件定时器初始化
  * @param:  timer-定时器量级.
  * @return: 无
  * @updata: [2017-10-26] [更改人姓名][变更描述]
  */
void rt_ostimer_init(rt_uint8_t timer)
{
    rt_timer_t tmr;
    
    switch (timer)
    {
        case TMR_1MS:   
            tmr = rt_timer_create(RT_SOFT_TIMER_1MS_NAME, 
                                  tmrout1ms_callbak, 
                                  RT_NULL, 
                                  1, // 定时长度 1 OS_TICK
                                  RT_TIMER_FLAG_PERIODIC); // 周期性定时器
            /* 启动定时器 */
            if (tmr != RT_NULL)
            {
                rt_timer_start(tmr);
              
                TIMER_PRINTF("1ms timer started \n"); 
            }         
            break;
						
        case TMR_2MS: 
            break;

        default:             
            break;        
    }
}


#endif /* RT_USING_SFTIMER */


/* END OF FILE ---------------------------------------------------------------*/
