/**
  * @file   mainBoardBoot.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/12
  * @brief  ����boot
  */
#include "systemconfig.h"
#include "MainBoardBoot.h"
#include "MainFlashOperate.h"
#include "ff.h"
#include "IapControl.h"
#include "drv_fm25vxx.h"

static FIL mainfile;
static uint8_t *mainName="m.bin";
static uint8_t writefilebuff[4096];

/**
  *@brief  �����������
  *@param  None
  *@retval 0 �ɹ� 1ʧ��
  */
uint8_t SaveMainProgram(void)
{
	uint32_t res;
	uint32_t i;
	uint16_t reallen;
	uint32_t tempSize;
	FatfsReFormat();
	res = f_open(&mainfile,(TCHAR *)mainName, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(res == FR_OK){
		DebugPrintf("���������,�������\r\n");
		for(i = MAINPRO_BEGINADDR; i < MAINPRO_ENDADDR; i += WRITEFILEBUFF){
			reallen = ReadProgram(i, writefilebuff,WRITEFILEBUFF);
			if(reallen){
				res = f_write(&mainfile,writefilebuff,reallen,(UINT *)&tempSize);
				if(res != FR_OK){
					f_close(&mainfile);
					DebugPrintf("WriteFile����2\r\n");
					return 2;
				}
				if(tempSize != reallen){
					DebugPrintf("WriteFile����3\r\n");
					f_close(&mainfile);	
					return 3;
				}
			}
			else{
				f_close(&mainfile);
				DebugPrintf("WriteFile����4\r\n");
				return 4;
			}
		}
	}
	else{
		DebugPrintf("���ļ�����\r\n");
		return 5;
	}
	DebugPrintf("�����ļ��ɹ�\r\n");
	f_close(&mainfile);
	return 0;
}

/**
  *@brief �����������
  *@param  None
  *@retval 0 �ɹ� 1ʧ��
  */
uint8_t UpdataMainProgram(void)
{
	uint32_t i;
	uint32_t fileSize,res;
	uint8_t tempVaule = 0xFF;
    DebugPrintf("�����������,ֱ�Ӹ��³���\r\n");
//	for(i = 0; i < 3; i++){
//		if(SaveMainProgram() == 0){
//			break;
//		}
//		if(i == 2){
//			DebugPrintf("�����ļ�ʧ��\r\n");
//			return 1;
//		}
//	}
	for(i = 0; i < 3; i++){
		tempVaule = IS_ROLLBACK_FLAG;
		FM25VxxWriteData(PRO_MAINBOARD_STATE + PRO_MAINBOARD_STATE,NULL,&tempVaule,1);
        
		if(EreaseProgram() == 0){
			break;
		}
		if(i == 2){
			DebugPrintf("����ʧ��\r\n");
			return 2;
		}
	}
	fileSize = *((uint32_t *)(MAIN_SDRAM_STARTADDR));
	res = WriteProgram(MAINPRO_BEGINADDR,\
		(uint8_t *)(MAIN_SDRAM_STARTADDR + 4),fileSize);
	if(res != 0){
		DebugPrintf("д�������\r\n");
		return 3;
	}
	DebugPrintf("���³���ɹ�\r\n");
	return 0;
}

/**
  *@brief  �����������
  *@param  None
  *@retval 0 �ɹ� 1ʧ��
  */
uint8_t MainProgramBack(void)
{
	uint32_t res;
	uint32_t i,k;
	uint32_t tempSize = 0,realLen;
	uint32_t fileSize;
	for(i = 0; i < 3; i++){
		res = f_open(&mainfile,(TCHAR *)mainName, FA_READ);
		if(res != FR_OK){
			f_close(&mainfile);
		}
		else{
			break;
		}
		if(i == 2){
			DebugPrintf("���ļ�ʧ��\r\n");
			return 1;
		}
	}
	for(i = 0; i < 3; i++){
		if(EreaseProgram() == 0){
			break;
		}
		if(i == 2){
			DebugPrintf("����ʧ��\r\n");
			return 2;
		}
	}
	fileSize = f_size(&mainfile);
	for(k = MAINPRO_BEGINADDR; k < MAINPRO_ENDADDR; k += WRITEFILEBUFF){
		res = f_read(&mainfile,writefilebuff,WRITEFILEBUFF,(UINT*)&realLen);
		if(res == FR_OK){
			res = WriteProgram(k,writefilebuff,realLen);
			if(res != 0){
				DebugPrintf("д�������\r\n");
				return 3;
			}
			DebugPrintf("д��ַ_0x%08X,size:%d\r\n",k,realLen);
			tempSize += realLen;
			if(realLen < WRITEFILEBUFF && tempSize < fileSize){
				DebugPrintf("д�������,realLen������\r\n");
				return 4;
			}
			if(tempSize >= fileSize){
				break;
			}
		}
		else{
			DebugPrintf("���ļ�����\r\n");
			return 5;
		}
	}
	DebugPrintf("���˳ɹ�\r\n");
	return 0;
}

void MtestCreatFile(void)
{
	uint8_t testbuff[4] = {1,2,3,4};
	uint8_t res;
	uint32_t realWrite;
	res = f_open(&mainfile,(TCHAR *)mainName, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(res != FR_OK){
		return ;
	}
	res = f_write(&mainfile,testbuff,4,(UINT *)&realWrite);
	if(res == FR_OK){
		f_close(&mainfile);
	}
}
