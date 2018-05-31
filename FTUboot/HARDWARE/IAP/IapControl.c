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
	if((appxaddr & 0x2FFE0000) == 0x20000000 ||\
		(appxaddr & 0x1FFE0000) == 0x10000000){
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
	FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,tempreadbuff, PRO_STATE_ALLBYTE);
	Delay_ms(50);
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,\
		tempreadbuff, PRO_STATE_ALLBYTE);
	if(tempflag == TRUE){
		pRunState.firstPower = tempreadbuff[PRO_FIRSTPOWER];
		pRunState.mBoradState = tempreadbuff[PRO_MAINBOARD_STATE];
		pRunState.bootCount = tempreadbuff[PRO_BOOTCOUNT];
		DebugPrintf("��ȡ����״̬�ɹ���");
		DebugPrintf("firstPower:0x%02X  ",pRunState.firstPower);
		DebugPrintf("mBoradState:0x%02X  ",pRunState.mBoradState);
		DebugPrintf("bootCount:%d\r\n",pRunState.bootCount);
		return 0;
	}
	DebugPrintf("��ȡ����״̬����!\r\n");
	return 1;	
}

/**
  *@brief ��ȡ�ļ�ϵͳ��Ϣ
  *@param  None
  *@retval None
  */
void GetFatfsInfo(void)
{
	DWORD fre_clust, fre_sect, tot_sect;
	FATFS *fs;
	uint8_t res = 0;
	res = f_getfree("0:", &fre_clust, &fs);
	if(res == 0){
		tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
		DebugPrintf("%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", tot_sect / 2, fre_sect / 2);
	}
}

/**
  * @Description: IAP��ʼ��
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
		if(res == 0){
			f_setlabel((const TCHAR *)"1:A");	//����Flash���̵�����Ϊ
		}
	}
	GetFatfsInfo();
}

void FatfsReFormat(void)
{
	uint8_t res = 0;
	DebugPrintf("�������¸�ʽ��\r\n");
	res = f_mkfs("0:",0,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
	if(res == 0){
		f_setlabel((const TCHAR *)"1:A");	//����Flash���̵�����Ϊ
	}
	Delay_ms(200);
	GetFatfsInfo();
}
/**
  *@brief �ļ�ϵͳж��
  *@param  None
  *@retval None
  */
void FatfsUninstall(void)
{
	f_mount(0,"0:",1);//����FLASH.
}

/**
  *@brief Iap���Ƴ�ʼ��
  *@param  None
  *@retval None
  */
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
	if(pRunState.firstPower != NOFIRSTPOWER){//��һ���ϵ�
		DebugPrintf("��һ���ϵ�\r\n");
		IapLoadApp(MAINPRO_BEGINADDR);
	}
	else if(pRunState.mBoradState == MAINPRO_RUNTURE){
		DebugPrintf("��������\r\n");
		IapLoadApp(MAINPRO_BEGINADDR);
	}
	else if(pRunState.mBoradState == MAINPRO_UPDATA){
		DebugPrintf("�����и���\r\n");
		if(UpadaProgramCheck(MAIN_SDRAM_STARTADDR + 4) == 0){
			uint8_t tempVaule = IS_RUNOK_FLAG;
			FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_MAINBOARD_STATE,NULL,&tempVaule,1);
			if(UpdataMainProgram() == 0){//�������
				IapLoadApp(MAINPRO_BEGINADDR);
			}
		}
		else{
			DebugPrintf("�����д�,ֱ����ת\r\n");
			IapLoadApp(MAINPRO_BEGINADDR);
		}
	}
	else{//�������
		if(MainProgramBack() == 0){//�������
			IapLoadApp(MAINPRO_BEGINADDR);
		}
	}
	/* ������ִ���,����,ֱ����ת */
	uint8_t tempVaule = !IS_FIRSTRUN_FLAG;
	FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_FIRSTPOWER,NULL,&tempVaule,1);
	Delay_ms(500);
	NVIC_SystemReset();//����
}

void VersionInfo(void)
{
	DebugPrintf("\r\n= = = = = = = = = = = = = = = = = = = = = = = =\r\n");
	DebugPrintf("FtuBootVersion:%s\r\n",FTUBOOT_VERSIONCODE);
}

/* END */
