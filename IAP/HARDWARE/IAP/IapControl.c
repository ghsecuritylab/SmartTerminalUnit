/**
  * @file   IapControl.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/20
  * @brief  IapBoot��ת����
  */
#include "systemconfig.h"
#include "IapControl.h"
#include "drv_fm25vxx.h"
#include "hmiboot.h"
#include "MainBoardBoot.h"
#include "MainFlashOperate.h"
#include "ff.h"

/* �߼����̹�����. */
static FATFS fs[_VOLUMES];
/* �������� */
enum IapControlStep controlStep;

/* ��������״̬ */
static struct ProgramRunState pRunState;

/**
  *@brief  ������°����
  *@param  proBeginAddr ��ų�������׵�ַ
  *@retval 0 �ɹ� 1ʧ��
  */
uint8_t UpadaProgramCheck(uint32_t proBeginAddr)
{
	uint32_t appxaddr;
	appxaddr = *(volatile uint32_t *)proBeginAddr;
	DebugPrintf("��ַ���: 0x%08X \r\n",appxaddr);
	if((appxaddr & 0x2FFE0000) == 0x20000000){
		return 0;
	}
	return 1;
}
void ModfiyPowerOnCount(void)
{
	uint8_t powerOnIs;
	uint8_t tempflag;
	
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR + PRO_BOOTCOUNT, NULL, &powerOnIs, 1);
	if(tempflag == TRUE){
		powerOnIs ++;
		FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_BOOTCOUNT,NULL,&powerOnIs,1);
		DebugPrintf("��������: %d \r\n",powerOnIs);
	}
}
/**
  *@brief  ��ȡ����״̬��־
  *@param  None
  *@retval 0 �ɹ� 1ʧ��
  */
uint8_t ReadProgramState(void)
{
	uint8_t tempreadbuff[PRO_STATE_ALLBYTE];
	uint8_t tempflag;
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,\
		tempreadbuff, PRO_STATE_ALLBYTE);//������һ��
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,\
		tempreadbuff, PRO_STATE_ALLBYTE);
	if(tempflag == TRUE){
		pRunState.firstPower = tempreadbuff[PRO_FIRSTPOWER];
		pRunState.hBoradState = tempreadbuff[PRO_HMIBOARD_STATE];
		pRunState.mBoradState = tempreadbuff[PRO_MAINBOARD_STATE];
		pRunState.bootCount = tempreadbuff[PRO_BOOTCOUNT];
		DebugPrintf("��ȡ����״̬�ɹ���");
		DebugPrintf("firstPower:0x%02X  ",pRunState.firstPower);
		DebugPrintf("hBoradState:0x%02X  ",pRunState.hBoradState);
		DebugPrintf("mBoradState:0x%02X  ",pRunState.mBoradState);
		DebugPrintf("bootCount:%d\r\n",pRunState.bootCount);
		return 0;
	}
	DebugPrintf("��ȡ����״̬����!\r\n");
	return 1;	
}

/**
  * @Description: �ļ�ϵͳ��ʼ��
  * @param:  void
  * @return: void
  * @updata: 
  */
void FatfsInit(void)
{
	uint8_t res=0;
	res=f_mount(&fs[0],"0:",1);//����FLASH.
	if(res != 0)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		DebugPrintf("���̸�ʽ��\r\n");
		res = f_mkfs("0:",0,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res == 0)
		{
			f_setlabel((const TCHAR *)"1:A");	//����Flash���̵�����
		}
		else{
			Delay_ms(1000);
		} 
	}
}

/**
  * @Description: ж���ļ�ϵͳ
  * @param:  None
  * @return: None
  */
void FatfsUninstall(void)
{
	f_mount(NULL,"0:",1);
}

void IapControlInit(void)
{
	uint8_t i;
	for(i = 0; i < 10; i++){
		if(ReadProgramState() != 0){
			if(i >= 9){
				//������״̬����
			}
			Delay_ms(100);
		}
		else{
			//״̬����
			break;
		}
	}
	ModfiyPowerOnCount();
}

/**
  *@brief  IAP����������
  *@param  None
  *@retval None
  */
void IapControlMain(void)
{
	static uint8_t hmistepFlag;
	static uint8_t jumpflag;
	if(controlStep == CON_INIT){
		hmistepFlag = 0;
		if(pRunState.firstPower != NOFIRSTPOWER){//��һ���ϵ�
			DebugPrintf("��һ���ϵ�\r\n");
			controlStep = CON_TOJUMP;
		}
		else{//�Ƚ���������
			if(pRunState.hBoradState == HMIPRO_RUNTURE){//��������
				DebugPrintf("�����������\r\n");
				controlStep = CON_HMIPRO_RUNOK;
			}
			else if(pRunState.hBoradState == HMIPRO_UPDATA){//����
				hmistepFlag = 0;
				if(UpadaProgramCheck(HMIPROGRAMSIZEADDR + 4) == 0){
					if(pRunState.mBoradState == MAINPRO_UPDATA){
						if(UpadaProgramCheck(MAIN_SDRAM_STARTADDR + 4) == 0){
							DebugPrintf("��忪ʼ����\r\n");
							controlStep = CON_HMIPRO_UPDATA;
						}
						else{
							controlStep = CON_MAINPRO_UPDATAPROERROE;
						}
					}
					else{
						DebugPrintf("��忪ʼ����\r\n");
						controlStep = CON_HMIPRO_UPDATA;
					}
				}
				else{
					controlStep = CON_HMIPRO_UPDATAPROERROE;
				}
			}
			else if(pRunState.hBoradState == HMIPRO_SAVE){//����
				DebugPrintf("����������\r\n");
				hmistepFlag = 0;
				controlStep = CON_HMIPRO_SAVE;
			}
			else{
				DebugPrintf("���������\r\n");
				hmistepFlag = 0;
				controlStep = CON_HMIPRO_BACK;
			}
		}
	}
	if(controlStep == CON_HMIPRO_FINISH){//�������
		if(pRunState.mBoradState == MAINPRO_RUNTURE){
			controlStep = CON_MAINPRO_RUNOK;
		}
		else if(pRunState.mBoradState == MAINPRO_UPDATA){
			if(UpadaProgramCheck(MAIN_SDRAM_STARTADDR + 4) == 0){
				controlStep = CON_MAINPRO_UPDATA;
			}
			else{
				controlStep = CON_MAINPRO_UPDATAPROERROE;
			}
		}
		else if(pRunState.mBoradState == MAINPRO_SAVE){
			controlStep = CON_MAINPRO_SAVE;
		}
		else{//�������
			controlStep = CON_MAINPRO_BACK;
		}
	}
	/* -------���----------- */
	if(controlStep == CON_HMIPRO_RUNOK){//
		controlStep = CON_HMIPRO_FINISH;
	}
	if(controlStep == CON_HMIPRO_UPDATA){//������
		if(HmiProgramUpadta(&hmistepFlag) == 0){//�������,1����ִ��
			controlStep = CON_HMIPRO_FINISH;
		}
	}
	if(controlStep == CON_HMIPRO_SAVE){//��屣��
		if(SaveHmiProgram(&hmistepFlag) == 0){//�������
			controlStep = CON_HMIPRO_FINISH;
		}
	}

	if(controlStep == CON_HMIPRO_BACK){//������
		if(HmiProgramBack(&hmistepFlag) == 0){//�������
			controlStep = CON_HMIPRO_FINISH;
		}	
	}
	/* -------����----------- */
	if(controlStep == CON_MAINPRO_RUNOK){//������������
		controlStep = CON_TOJUMP;
	}
	if(controlStep == CON_MAINPRO_UPDATA){//�������
		if(UpdataMainProgram() == 0){//�������
			controlStep = CON_TOJUMP;
		}
		else{//����ʧ��,��������
			controlStep = CON_MAINPRO_BACK;
		}
	}
	if(controlStep == CON_MAINPRO_SAVE){//���屣��
		if(SaveMainProgram() == 0){//�������
			controlStep = CON_TOJUMP;
		}
		else{
			controlStep = CON_TOJUMP;
		}
	}
	if(controlStep == CON_MAINPRO_BACK){//�������
		if(MainProgramBack() == 0){//�������
			controlStep = CON_TOJUMP;
		}
		else{
			controlStep = CON_TOJUMP;
		}
	}
	if(controlStep == CON_HMIPRO_UPDATAPROERROE){//��⵽�����³����д�
		DebugPrintf("HMI����������,��������ȷ������");
		controlStep = CON_TOJUMP;
	}
	if(controlStep == CON_MAINPRO_UPDATAPROERROE){//��⵽������³����д�
		DebugPrintf("��������������,��������ȷ������");
		controlStep = CON_TOJUMP;
	}
	/* -------��ת----------- */
	if(controlStep == CON_TOJUMP){//������壬ֱ����ת
		DebugPrintf("��ʼ��ת\r\n");
		if(jumpflag ++ < 3){
			HmiProgramJump();
		}
		else{
			IapLoadApp(MAINPRO_BEGINADDR);
			while(1);
		}
		//NVIC_SystemReset();//����
	}
	if(controlStep < CON_HMIPRO_FINISH){
		IapOutTimCheck();
	}
}

void SetControlStep(enum IapControlStep step)
{
	controlStep = step;
}
