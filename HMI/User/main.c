
#include "stm32f10x.h"
#include "systemconfig.h"
#include "uartDriver.h"
#include "ledDriver.h"
#include "lkdGuiUse.h"
#include "dlt634_HMImaster_disk.h"
#include "hmi101.h"
#include "switchDriver.h"
#include "keyDriver.h"
#include "GuiDisplay.h"
#include "inoutUser.h"

static void IwdgInit(void);
static void IwdgReloadCounter(void);

int main(void)
{
	CLOSE_ALL_INT();
	SystemconfigInit();
	LedDriverInit();
	KeyDriverInit();
	SwitchDriverInit();
	uartInit();
	LcdInit();
	ZiKuInit();
	OPEN_ALL_INT();
	GUIStartInterface();
	DLT634_HMI_MASTER_INIT(0);
	IwdgInit();
	while(1)                            
	{
		ScanSwitchStatus();
		ScanKeyStatus();
		Hmi101Main();
		InOutUserMain();
		IwdgReloadCounter();
	}
}

/**
  *@brief ���Ź���ʼ��
  *@param  None
  *@retval None
  */
static void IwdgInit(void)
{
	uint16_t reload; /* ����ֵ ,���ܴ���0x0FFF*/
	
	/* 
	 *reload = (LSi / ( 4*2^IWDG_Prescaler )) * time
	 * LSi ���Ź�ʱ�� 40KHZ
	 * reload ����ֵ
	 * IWDG_Prescaler ��Ƶ
	 * time �趨ʱ�� s
	*/
	reload = (uint16_t)1250*2.5;//2.5S 
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(reload);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

/**
  *@brief ι��
  *@param  None
  *@retval None
  */
static void IwdgReloadCounter(void)
{
	IWDG_ReloadCounter(); /* ι�� */
}
/* END */
