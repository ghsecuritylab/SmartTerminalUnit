/**
  ******************************************************************************
  * @file    systemconfig.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   M3��Systick��ʱ��������Ϊ�����ļ� ����
  ******************************************************************************
  */
#include "systemconfig.h"

/* 1usʱ��Ľ����� */
static uint32_t Fac_us;	
static uint32_t timeCount;

/**
  *@brief ʱ�ӳ�ʼ��
  *@param  
  *@retval None
  */
void Stm32_Clock_Init(uint32_t plln,uint32_t pllm,uint32_t pllp,uint32_t pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //ʹ��PWRʱ��
    
    //������������������õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���
    //ʱʹ�����빦��ʵ��ƽ�⣬�˹���ֻ��STM32F42xx��STM32F43xx�����У�
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//���õ�ѹ�������ѹ����1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //��HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//��PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; //��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.  
    RCC_OscInitStructure.PLL.PLLP=pllp; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
	
    if(ret!=HAL_OK) while(1);
    
    ret=HAL_PWREx_EnableOverDrive(); //����Over-Driver����
    if(ret!=HAL_OK) while(1);
    
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2��Ƶϵ��Ϊ2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//ͬʱ����FLASH��ʱ����Ϊ5WS��Ҳ����6��CPU���ڡ�
	
    if(ret!=HAL_OK) while(1);
}


/**
  *@brief ϵͳ���ȼ�����
  *@param  None
  *@retval None
  */
void NVIC_Configuration(void)
{  
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);		
}

void SysTick_Init(int16_t clock_div)
{
	/* SystemCoreClock / 1000   1ms */
	if (SysTick_Config(SystemCoreClock / clock_div)){ 
	/* Capture error */ 
		while (1);
	}
	Fac_us = SystemCoreClock/1000000;/* 1usʱ��Ľ�����,������ʱ���� */
	OPen_SysTick();
}

/**
  *@brief ��ȷ΢����ʱ
  *@param  nus  Ҫ��ʱ��΢����
  *@retval None
  */
void Delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				/* LOAD��ֵ	 */    	 
	ticks=nus*Fac_us; 						/* ��Ҫ�Ľ�����	  */ 		 
	tcnt=0;
	told=SysTick->VAL;        				/* �ս���ʱ�ļ�����ֵ */
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)
				tcnt+=told-tnow;			/* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����. */
			else 
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;						/* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�. */
		}  
	}							    
}

/**
  *@brief ��ȷms��ʱ
  *@param  nms Ҫ��ʱ�ĺ�����
  *@retval None
  */
void Delay_ms(uint16_t nms)
{	    		  
	Delay_us((uint32_t)(nms*1000));
}

void SystemconfigInit(void)
{
	
	NVIC_Configuration();	
	SysTick_Init((int16_t)1000);/* 1ms */
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	timeCount ++;
	LedRun();
}

/**
  *@brief ��ȡ��ʼ��ʱ��
  *@param  None
  *@retval beginTick  ��ʱ��ʼ����
  */
uint32_t GetTimer1Tick(void)
{
	return timeCount;
}

/**
  *@brief ��ȡ���������
  *@param  beginTick  ��ʱ��ʼ������GetTimer1Tick()�������
  *@retval ���������
  */
uint32_t GetTimer1IntervalTick(uint32_t beginTick)
{
	uint32_t temptick;
	temptick = GetTimer1Tick();
	if(beginTick <= temptick){
		return (temptick - beginTick);
	}
	else{
		return ((0xFFFFFFFF - beginTick)+temptick);
	}
}
