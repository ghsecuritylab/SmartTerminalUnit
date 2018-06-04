/**
  * @file   mainFlashOperate.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/22
  * @brief  ����flash����
  */

#include "MainFlashOperate.h"
#include "systemconfig.h"
#include "led.h"
#include "w25qxx.h"
#include "sdram.h"
#include "drv_fm25vxx.h"
#include "uartDriver.h"

static Iapfun jump2app;//��ת����ָ��

/**
  *@brief  ɱ��boot��������
  *@param  addr  ��ַ
  *@param  pBuff  ����ָ��
  *@param  sizeByte  ��С
  *@retval 0�ɹ� 1ʧ��
  */
void ShutdownbootUse(void)
{
	ledUseKill();
	W25qxxUseKill();
	SDRAMUseKill();
	FM25VxxUseKill();
	UartUseKill();
	Close_SysTick();
}

/**
  *@brief  ����flash
  *@param  addr  ��ַ
  *@param  pBuff  ����ָ��
  *@param  sizeByte  ��С
  *@retval 0�ɹ� 1ʧ��
  */
uint8_t EreaseProgram(void)
{
	uint8_t i;
	uint32_t sectorError = 0;
	HAL_StatusTypeDef flashStatus = HAL_OK;
	FLASH_EraseInitTypeDef FlashEraseInit;
	
	FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;//�������ͣ��������� 
	FlashEraseInit.Sector = MAINPRO_BEGINSECTOR;
	FlashEraseInit.NbSectors = 1;
	FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;//��ѹ��Χ��VCC=2.7~3.6V֮��
  HAL_FLASH_Unlock();
	
	for(i=0;i < MAINPRO_SECTORNUM; i++){
		FlashEraseInit.Sector = (MAINPRO_BEGINSECTOR + i);//Ҫ����������
		flashStatus = HAL_FLASHEx_Erase(&FlashEraseInit,&sectorError);//��������
		if(flashStatus == HAL_OK){
			DebugPrintf("����%d�����ɹ�\r\n",FlashEraseInit.Sector);
		}
		else{
			DebugPrintf("����%d����ʧ��\r\n",FlashEraseInit.Sector);
			return 1;
		}
  }
	return 0;
}

/**
  *@brief  дflash
  *@param  addr  ��ַ
  *@param  pBuff  ����ָ��
  *@param  sizeByte  ��С
  *@retval 0�ɹ� 1ʧ��
  */
uint8_t WriteProgram(uint32_t addr,const uint8_t *pBuff,uint32_t sizeByte)
{
	uint32_t i;
	uint32_t value;
	uint32_t tempSize;
	HAL_StatusTypeDef status;
	
	if(addr < MAINPRO_BEGINADDR || addr > MAINPRO_ENDADDR){
		return 0;
	}
	if(addr + sizeByte > MAINPRO_ENDADDR){
		sizeByte = MAINPRO_ENDADDR - addr + 1;
	}
	
	tempSize = sizeByte;
	
	if(tempSize % 4 != 0){
		tempSize = tempSize - tempSize % 4;
	}
	for(i = 0; i < tempSize; i += 4){
		value = pBuff[i]+(pBuff[i+1]<<8)+(pBuff[i+2]<<16)+(pBuff[i+3]<<24);
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addr + i,(uint64_t)value);
		if(status != HAL_OK){
			return 1;
		}
	}
	tempSize = sizeByte - tempSize;
	switch(tempSize){
		case 0:break;
		case 1:value = pBuff[i] | 0x00FFFFFF;break;
		case 2:value = (pBuff[i]+(pBuff[i+1]<<8)) | 0x0000FFFF;break;
		case 3:value = (pBuff[i]+(pBuff[i+1]<<8)+(pBuff[i+2]<<16)) | 0x000000FF;break;
		default: value = 0xFFFFFFFF;break;
	}
	if(tempSize){
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addr + i,(uint64_t)value);
		if(status != HAL_OK){
			return 1;
		}
	}
	return 0;
}
/**
  *@brief  ��flash
  *@param  addr  ��ַ
  *@param  pBuff  ����ָ��
  *@param  sizeByte  ��С
  *@retval None
  */
uint16_t ReadProgram(uint32_t addr, uint8_t *pBuff,uint32_t sizeByte)
{
	uint32_t i;
	uint8_t tempvalue;
	uint32_t tempSize;
	union{
		uint32_t uint32;
		uint8_t uint8[4];
	}value8To32;
	
	if(addr < MAINPRO_BEGINADDR || addr > MAINPRO_ENDADDR){
		return 0;
	}
	if(addr + sizeByte > MAINPRO_ENDADDR){
		sizeByte = MAINPRO_ENDADDR - addr;
	}
	tempvalue = sizeByte % 4;
	tempSize = sizeByte - tempvalue;
	for(i = 0; i < tempSize; i +=4){
		value8To32.uint32 = *(volatile uint32_t *)(addr + i);
		pBuff[i] = value8To32.uint8[0];
		pBuff[i+1] = value8To32.uint8[1];
		pBuff[i+2] = value8To32.uint8[2];
		pBuff[i+3] = value8To32.uint8[3];
	}
	if(tempvalue == 1){
		value8To32.uint32 = *(volatile uint32_t *)(addr + i);
		pBuff[i] = value8To32.uint8[0];
	}
	else if(tempvalue == 2){
		value8To32.uint32 = *(volatile uint32_t *)(addr + i);
		pBuff[i] = value8To32.uint8[0];
		pBuff[i+1] = value8To32.uint8[1];
	}
	else if(tempvalue == 3){
		value8To32.uint32 = *(volatile uint32_t *)(addr + i);
		pBuff[i] = value8To32.uint8[0];
		pBuff[i+1] = value8To32.uint8[1];
		pBuff[i+2] = value8To32.uint8[2];
	}
	return sizeByte;
}

/**
  *@brief  ������ת����
  *@param  appxaddr  Ҫ��ת�ĵ�ַ
  *@retval None
  */
uint32_t IapLoadApp(uint32_t appxaddr)
{
	if(((*(volatile uint32_t *)appxaddr)&0x2FFC0000) == 0x20000000 ||\
		((*(volatile uint32_t *)appxaddr)&0x1FFE0000) == 0x10000000)//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(Iapfun)*(volatile uint32_t *)(appxaddr+4);//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		__set_MSP(*(volatile uint32_t *)appxaddr);//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)

		CLOSE_ALL_INT();
		ShutdownbootUse();
		jump2app();//��ת��APP.
	}
	DebugPrintf("��תʧ��,��ת��ջ:0x%08X\r\n",*(volatile uint32_t *)appxaddr);
	return *(volatile uint32_t *)appxaddr;
}

//uint8_t tpBuff[512];
//void testFlashOperate(void)
//{
//	uint32_t i,j;
//	uint32_t res;
//	for(i=0;i<512;i++){
//		tpBuff[i] = (uint8_t)i;
//	}
//	DebugPrintf("EraseProgram\r\n");
//	EreaseProgram();
//	for(i = MAINPRO_BEGINADDR;i < MAINPRO_ENDADDR;i += 512){
//		res = WriteProgram(i,tpBuff,512);
//		if(res == 0){
//		}
//		else{
//			DebugPrintf("WriteProgramFalse\r\n");
//		}
//	}
//	DebugPrintf("WriteProgramAddr: 0x%08X\r\n",i);
//	DebugPrintf("ReadProgram\r\n");
//	for(i = MAINPRO_BEGINADDR;i < MAINPRO_ENDADDR;i += 512){
//		res = ReadProgram(i,tpBuff,512);
//		if(res){
//			for(j = 0;j < res; j++){
//				DebugPrintf("%02X ",tpBuff[j]);
//			}
//			DebugPrintf("res:%d \r\n",res);
//		}
//		else{
//			DebugPrintf("ReadProgramEnd\r\n");
//		}
//	}
//	DebugPrintf("EraseProgram\r\n");
//	EreaseProgram();
//	DebugPrintf("ReadProgram\r\n");
//	for(i = MAINPRO_BEGINADDR;i < MAINPRO_ENDADDR;i += 512){
//		res = ReadProgram(i,tpBuff,512);
//		if(res){
//			for(j = 0;j < res; j++){
//				DebugPrintf("%02X ",tpBuff[j]);
//			}
//			DebugPrintf("res:%d \r\n",res);
//		}
//		else{
//			DebugPrintf("ReadProgramEnd\r\n");
//		}
//	}
//	DebugPrintf("ReadProgramAddr:0x%08X\r\n",i);
//}
