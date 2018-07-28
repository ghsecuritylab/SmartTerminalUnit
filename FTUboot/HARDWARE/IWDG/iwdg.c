#include "iwdg.h" 

IWDG_HandleTypeDef IWDG_Handler; //�������Ź����

/**
  * @brief:  ��ʼ���������Ź� ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
  * @param:  prer:��Ƶ��:IWDG_PRESCALER_4~IWDG_PRESCALER_256
  * @param:  rlr:�Զ���װ��ֵ,0~0XFFF.
  * @return: None
  */
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=prer;	//����IWDG��Ƶϵ��
    IWDG_Handler.Init.Reload=rlr;		//��װ��ֵ
    HAL_IWDG_Init(&IWDG_Handler);		//��ʼ��IWDG  
	
    __HAL_IWDG_START(&IWDG_Handler);		//�����������Ź�
}
    
/**
  * @brief: ���Ź�ʱ�ӷ�ת
  * @param:  None
  * @return: None
  */
void WDG_FEED(void)
{
   //HAL_IWDG_Refresh(&IWDG_Handler); 	//ι��
}

