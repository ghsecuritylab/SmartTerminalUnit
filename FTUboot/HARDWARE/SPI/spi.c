
#include "spi.h"
	
SPI_HandleTypeDef SPI4_Handler;  //SPI���

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI5�ĳ�ʼ��
void SPI4_Init(void)
{
    SPI4_Handler.Instance=SPI4;                         //SP4
    SPI4_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI4_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI4_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI4_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI4_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI4_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI4_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI4_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI4_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI4_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI4_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI4_Handler);
    
    __HAL_SPI_ENABLE(&SPI4_Handler);                    //ʹ��SPI4
    SPI4_ReadWriteByte(0Xff);                           //��������
}

//SPI5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();       //ʹ��GPIOFʱ��
    __HAL_RCC_SPI4_CLK_ENABLE();        //ʹ��SPI5ʱ��
    
    //PF7,8,9
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
    GPIO_Initure.Alternate=GPIO_AF5_SPI4;           //����ΪSPI4
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}

//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ45Mhz��
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI4_Handler);            //�ر�SPI
    SPI4_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI4_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI4_Handler);             //ʹ��SPI
    
}

//SPI5 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI4_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI4_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}

/**
  * @Description: Spiʹ��ɱ��
  * @param:  None
  * @return: None
  */
void SpiUseKill(void)
{
	HAL_SPI_DeInit(&SPI4_Handler);
	__HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_SPI4_CLK_DISABLE();
}

/* END */
