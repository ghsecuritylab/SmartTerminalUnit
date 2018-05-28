
#ifndef __USERINFO_H
#define	__USERINFO_H

#include "stm32f4xx.h"
#include "gwprotocal.h"

#define NULL 0

typedef enum UserInfoErrorCode{
	ISWAITINGACK,
	REVACKOK,
	ISCONNECTFAIL,
	PARAMTERERROR,
	HAVEMASTERCMD,
	NOMASTERCMD,
}UserErrorStatus;


enum Cmd03MasterOffset{//��ѯ
	M03_INFOBYTESIZE_L,//����
	M03_INFOBYTESIZE_H,
	M03_CMD,//�����
	M03_REG_ADDRNUM,//�Ĵ�����ַ����Ϊ1ʱM03_REG1_SERIALNUM��Ч
	M03_REG_SERIALNUM,//�Ĵ�����ַ1��ʼ����������
	M03_REG_EACHSIZE,
	M03_REG1_ADDRL,//�Ĵ�����ַ1
	M03_REG1_ADDRH,
};

enum Cmd03SlaveOffset{
	S03_INFOBYTESIZE_L,//����
	S03_INFOBYTESIZE_H,
	S03_CMD,//�����
	S03_REG_ADDRNUM,//�Ĵ�����ַ����
	S03_REG_SERIALNUM,//�Ĵ�����ַ1��ʼ����������
	S03_REG_EACHSIZE,
	S03_REG1_ADDRL,//�Ĵ�����ַ1
	S03_REG1_ADDRH,
	S03_REG1_CONTENT,
};

/* ������ȡ�ӻ��ļ� */
enum Cmd04MasterOffset{
	M04_INFOBYTESIZE_L,//����
	M04_INFOBYTESIZE_H,
	M04_CMD,//�����
	M04_FILENUMBER,//�ļ�������
	M04_FILELENGTH,//�ļ�������
	M04_FILENAME,//�ļ�����
};

enum Cmd04SlaveAckFlag{
	S04_NOTHISFILE,//û�д��ļ�
	S04_REFUSESEND,//�ܾ����ʹ��ļ�
	S04_REVOK,//���ճɹ����ȴ��ٻ��ļ�
};
enum Cmd04SlaveOffset{
	S04_INFOBYTESIZE_L,//����
	S04_INFOBYTESIZE_H,
	S04_CMD,//�����
	S04_FILENUMBER,//�ļ�������
	S04_ACKFLAG,//Ӧ���
	S04_FILELENGTH_LL,
	S04_FILELENGTH_LH,
	S04_FILELENGTH_HL,
	S04_FILELENGTH_HH,
	S04_FILENAMELENGTH,
	S04_FILENAME,
};

enum Cmd05MasterOffset{
	M05_INFOBYTESIZE_L,//����
	M05_INFOBYTESIZE_H,
	M05_CMD,//�����
	M05_FILENUMBER,//�ļ�������
	M05_FILEOFFSET_LL,//�ļ�ƫ��
	M05_FILEOFFSET_LH,
	M05_FILEOFFSET_HL,
	M05_FILEOFFSET_HH,
	M05_CONTENTLEN_L,//���ݳ���
	M05_CONTENTLEN_H,
};

enum Cmd05SlaveAckFlag{
	S05_REVERROR,//
	S05_REVOK,//
	S05_FINENED,
};
enum Cmd05SlaveOffset{
	S05_INFOBYTESIZE_L,//����
	S05_INFOBYTESIZE_H,
	S05_CMD,//�����
	S05_FILENUMBER,//�ļ�������
	S05_FILEACKFLAG,//Ӧ���	
//	S05_FILEOFFSET_LL,//�ļ�ƫ��
//	S05_FILEOFFSET_LH,
//	S05_FILEOFFSET_HL,
//	S05_FILEOFFSET_HH,
	S05_CONTENTLEN_L,//���ݳ���
	S05_CONTENTLEN_H,
	S05_FILECONTENT,//�ļ�����
};


/* ���������ļ����ӻ� */
enum Cmd06MasterOffset{
	M06_INFOBYTESIZE_L,//����
	M06_INFOBYTESIZE_H,
	M06_CMD,//�����
	M06_FILENUMBER,//�ļ�������
	M06_FILELENGTH_LL,//�ļ���С
	M06_FILELENGTH_LH,
	M06_FILELENGTH_HL,
	M06_FILELENGTH_HH,
	M06_FILELENGTH,//�ļ�������
	M06_FILENAME,//�ļ�����
};

enum Cmd06SlaveAckFlag{
	S06_REFUSESEND,//�ܾ����մ��ļ�
	S06_REVOK,//���ճɹ����ȴ��ٻ��ļ�
};
enum Cmd06SlaveOffset{
	S06_INFOBYTESIZE_L,//����
	S06_INFOBYTESIZE_H,
	S06_CMD,//�����
	S06_FILENUMBER,//�ļ�������
	S06_ACKFLAG,//Ӧ���
};

enum Cmd07MasterStatus{
	M07_TRANSFERRING,
	M07_FILEEND,//�ļ����ͽ���
};
enum Cmd07MasterOffset{
	M07_INFOBYTESIZE_L,//����
	M07_INFOBYTESIZE_H,
	M07_CMD,//�����
	M07_FILENUMBER,//�ļ�������
	M07_FILEFLAG,
	M07_FILEOFFSET_LL,//�ļ�ƫ��
	M07_FILEOFFSET_LH,
	M07_FILEOFFSET_HL,
	M07_FILEOFFSET_HH,
	M07_CONTENTLEN_L,//���ݳ���
	M07_CONTENTLEN_H,
	M07_FILECONTENT,//�ļ�����
};

enum Cmd07SlaveAckFlag{
	S07_REVERROR,//
	S07_REVOK,//
	S07_REVEND,
};
enum Cmd07SlaveOffset{
	S07_INFOBYTESIZE_L,//����
	S07_INFOBYTESIZE_H,
	S07_CMD,//�����
	S07_FILENUMBER,//�ļ�������
	S07_FILEACKFLAG,//Ӧ���	
	S07_FILEOFFSET_LL,//�ļ�ƫ��
	S07_FILEOFFSET_LH,
	S07_FILEOFFSET_HL,
	S07_FILEOFFSET_HH,
	S07_CONTENTLEN_L,//���ݳ���
	S07_CONTENTLEN_H,
};

enum Cmd10MasterOffset{//�޸�
	M10_INFOBYTESIZE_L,//����
	M10_INFOBYTESIZE_H,
	M10_CMD,//�����
	M10_REG_ADDRNUM,//�Ĵ�����ַ����Ϊ1ʱM03_REG1_SERIALNUM��Ч
	M10_REG_SERIALNUM,//�Ĵ�����ַ1��ʼ����������
	M10_REG_EACHSIZE,
	M10_REG1_ADDRL,//�Ĵ�����ַ1
	M10_REG1_ADDRH,
	M10_REG1_CONTENT,
};

enum Cmd10SlaveAckFlag{
	S10_CMDERROR,//
	S10_MODFIYOK,//
	S10_REFUSEMODFIY,
};
enum Cmd10SlaveOffset{
	S10_INFOBYTESIZE_L,//����
	S10_INFOBYTESIZE_H,
	S10_CMD,//�����
	S10_ACKFLAG,//�����
};

ReturnStatus ApplyForUser(UserInfo *user);
UserErrorStatus GetMasterUserInfoStatus(UserInfo *user);
UserErrorStatus GetSlaveUserInfoStatus(UserInfo *user);

#endif /* __USERINFO_H */

/* END */
