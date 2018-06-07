/**
  * @file   ds18B20.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   �¶Ȼ�ȡ�ļ�
  */

#include "ds18b20.h"
#include "systemconfig.h"

static float Temperature;/* ��ȡ�����¶� */


/**
  *@brief ����DS18B20�õ���I/O��
  *@param  None
  *@retval None
  */
static void DS18B20_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 															   
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 
}

/**
  *@brief ʹDS18B20-DATA���ű�Ϊ����ģʽ
  *@param  None
  *@retval None
  */
static void DS18B20_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/**
  *@brief ʹDS18B20-DATA���ű�Ϊ���ģʽ
  *@param  None
  *@retval None
  */
static void DS18B20_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;															   
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/**
  *@brief �������ӻ����͸�λ����
  *@param  None
  *@retval None
  */
static void DS18B20_Rst(void)
{
	/* ��������Ϊ������� */
	DS18B20_Mode_Out_PP();
	DS18B20_DATA_OUT(LOW);
	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	Delay_us(750);
	/* �����ڲ�����λ�źź��轫�������� */
	DS18B20_DATA_OUT(HIGH);
	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
	Delay_us(15);
}

/**
  *@brief ���ӻ����������صĴ�������
  *@param  None
  *@retval 0 �ɹ�  1 ʧ��
  */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* ��������Ϊ�������� */
	DS18B20_Mode_IPU();
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	while( !DS18B20_DATA_IN() && pulse_time < 240 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	if( pulse_time >= 240 )
		return 1;
	else
		return 0;
}

/**
  *@brief ��DS18B20��ȡһ��bit
  *@param  None
  *@retval bit
  */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DATA_OUT(LOW);
	Delay_us(10);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
	//Delay_us(2);
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	Delay_us(45);
	
	return dat;
}

/**
  *@brief ��DS18B20��һ���ֽڣ���λ����
  *@param  None
  *@retval ����
  */
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/**
  *@brief дһ���ֽڵ�DS18B20����λ����
  *@param  ����
  *@retval None
  */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			/* 1us < �����ʱ < 15us */
			Delay_us(8);
			
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			Delay_us(70);
			
			DS18B20_DATA_OUT(HIGH);			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			Delay_us(2);
		}
	}
}

/**
  *@brief DS18B20��ʼ�¶�ת��
  *@param  None
  *@retval None
  */
void DS18B20_Start(void)
{
	uint8_t waitsuccess = 3;
	while(waitsuccess--){
		DS18B20_Rst();	   
		if(DS18B20_Presence()==0){//���ɹ�
			DS18B20_Write_Byte(0XCC);		/* ���� ROM */
			DS18B20_Write_Byte(0X44);		/* ��ʼת�� */
			break;//����ѭ��
		}
	} 	
}

/**
  *@brief 8λCRCУ�����
  *@param  ptr ��У���ַ���
  *@param  len �ַ�������
  *@retval У��ֵ
  */
uint8_t xCal_crc(uint8_t *ptr, uint8_t len)
{
	uint8_t crc;
	uint8_t i;
	crc = 0;
	while (len--)
	{
		crc ^= *ptr++;
		for (i = 0; i < 8; i++)
		{
			if (crc & 0x01)
			{
				crc = (crc >> 1) ^ 0x8C;
			}
			else crc >>= 1;
		}
	}
	return crc;
}
/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
static void DS18B20_Get_Temp(void)
{
	uint8_t i,waitsuccess = 3;
	uint8_t getDsValue[9];
	short s_tem;
	float f_tem;
	while(waitsuccess--){
		DS18B20_Rst();
		if(DS18B20_Presence()==0){
			DS18B20_Write_Byte(0XCC);				/* ���� ROM */
			DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
			for(i=0;i<9;i++){
				getDsValue[i] = DS18B20_Read_Byte();
			}
			if(xCal_crc(getDsValue,8) == getDsValue[8]){
				s_tem = getDsValue[1]<<8;
				s_tem = getDsValue[0] | s_tem;			
				f_tem = s_tem * 0.0625;//�¶�ת����������
				Temperature = f_tem;//���¶ȸ�ȫ�ֱ���	
				break;//����ѭ��
			}
		}		
	}	
}

/**
  *@brief ������
  *@param  None
  *@retval �¶� 
  */
void Ds18B20Main(void)
{
	static uint32_t tickBegin;/* tickʱ�ӿ�ʼֵ */
	
	if(GetTimer1IntervalTick(tickBegin) >= CYCTRANSFORMVAULE){/*�����趨ʱ��*/
		DS18B20_Get_Temp();//��ȡ�¶�
		DS18B20_Start();//�¶�ת��
		tickBegin = GetTimer1Tick();
	}		
}

/**
  *@brief Ds18B20��ʼ��
  *@param  None
  *@retval None
  */
void DS18B20_Init(void)
{
	DS18B20_GPIO_Config();
	Temperature = 0;
	DS18B20_Start();//�¶�ת��
	DS18B20_Get_Temp();//��ȡ�¶�
}

/**
  *@brief ��ȡ�¶ȣ������ⲿ�ļ�����
  *@param  None
  *@retval �¶� 
  */
float GetTemperature(void)
{
	return Temperature;
}

/* END */
