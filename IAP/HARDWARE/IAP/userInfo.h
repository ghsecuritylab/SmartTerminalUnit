
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


enum Cmd03MasterOffset{//查询
	M03_INFOBYTESIZE_L,//长度
	M03_INFOBYTESIZE_H,
	M03_CMD,//命令号
	M03_REG_ADDRNUM,//寄存器地址数量为1时M03_REG1_SERIALNUM有效
	M03_REG_SERIALNUM,//寄存器地址1开始的连续个数
	M03_REG_EACHSIZE,
	M03_REG1_ADDRL,//寄存器地址1
	M03_REG1_ADDRH,
};

enum Cmd03SlaveOffset{
	S03_INFOBYTESIZE_L,//长度
	S03_INFOBYTESIZE_H,
	S03_CMD,//命令号
	S03_REG_ADDRNUM,//寄存器地址数量
	S03_REG_SERIALNUM,//寄存器地址1开始的连续个数
	S03_REG_EACHSIZE,
	S03_REG1_ADDRL,//寄存器地址1
	S03_REG1_ADDRH,
	S03_REG1_CONTENT,
};

/* 主机读取从机文件 */
enum Cmd04MasterOffset{
	M04_INFOBYTESIZE_L,//长度
	M04_INFOBYTESIZE_H,
	M04_CMD,//命令号
	M04_FILENUMBER,//文件名代号
	M04_FILELENGTH,//文件名长度
	M04_FILENAME,//文件内容
};

enum Cmd04SlaveAckFlag{
	S04_NOTHISFILE,//没有此文件
	S04_REFUSESEND,//拒绝发送此文件
	S04_REVOK,//接收成功，等待召唤文件
};
enum Cmd04SlaveOffset{
	S04_INFOBYTESIZE_L,//长度
	S04_INFOBYTESIZE_H,
	S04_CMD,//命令号
	S04_FILENUMBER,//文件名代号
	S04_ACKFLAG,//应答号
	S04_FILELENGTH_LL,
	S04_FILELENGTH_LH,
	S04_FILELENGTH_HL,
	S04_FILELENGTH_HH,
	S04_FILENAMELENGTH,
	S04_FILENAME,
};

enum Cmd05MasterOffset{
	M05_INFOBYTESIZE_L,//长度
	M05_INFOBYTESIZE_H,
	M05_CMD,//命令号
	M05_FILENUMBER,//文件名代号
	M05_FILEOFFSET_LL,//文件偏移
	M05_FILEOFFSET_LH,
	M05_FILEOFFSET_HL,
	M05_FILEOFFSET_HH,
	M05_CONTENTLEN_L,//内容长度
	M05_CONTENTLEN_H,
};

enum Cmd05SlaveAckFlag{
	S05_REVERROR,//
	S05_REVOK,//
	S05_FINENED,
};
enum Cmd05SlaveOffset{
	S05_INFOBYTESIZE_L,//长度
	S05_INFOBYTESIZE_H,
	S05_CMD,//命令号
	S05_FILENUMBER,//文件名代号
	S05_FILEACKFLAG,//应答号	
//	S05_FILEOFFSET_LL,//文件偏移
//	S05_FILEOFFSET_LH,
//	S05_FILEOFFSET_HL,
//	S05_FILEOFFSET_HH,
	S05_CONTENTLEN_L,//内容长度
	S05_CONTENTLEN_H,
	S05_FILECONTENT,//文件内容
};


/* 主机发送文件给从机 */
enum Cmd06MasterOffset{
	M06_INFOBYTESIZE_L,//长度
	M06_INFOBYTESIZE_H,
	M06_CMD,//命令号
	M06_FILENUMBER,//文件名代号
	M06_FILELENGTH_LL,//文件大小
	M06_FILELENGTH_LH,
	M06_FILELENGTH_HL,
	M06_FILELENGTH_HH,
	M06_FILELENGTH,//文件名长度
	M06_FILENAME,//文件内容
};

enum Cmd06SlaveAckFlag{
	S06_REFUSESEND,//拒绝接收此文件
	S06_REVOK,//接收成功，等待召唤文件
};
enum Cmd06SlaveOffset{
	S06_INFOBYTESIZE_L,//长度
	S06_INFOBYTESIZE_H,
	S06_CMD,//命令号
	S06_FILENUMBER,//文件名代号
	S06_ACKFLAG,//应答号
};

enum Cmd07MasterStatus{
	M07_TRANSFERRING,
	M07_FILEEND,//文件发送结束
};
enum Cmd07MasterOffset{
	M07_INFOBYTESIZE_L,//长度
	M07_INFOBYTESIZE_H,
	M07_CMD,//命令号
	M07_FILENUMBER,//文件名代号
	M07_FILEFLAG,
	M07_FILEOFFSET_LL,//文件偏移
	M07_FILEOFFSET_LH,
	M07_FILEOFFSET_HL,
	M07_FILEOFFSET_HH,
	M07_CONTENTLEN_L,//内容长度
	M07_CONTENTLEN_H,
	M07_FILECONTENT,//文件内容
};

enum Cmd07SlaveAckFlag{
	S07_REVERROR,//
	S07_REVOK,//
	S07_REVEND,
};
enum Cmd07SlaveOffset{
	S07_INFOBYTESIZE_L,//长度
	S07_INFOBYTESIZE_H,
	S07_CMD,//命令号
	S07_FILENUMBER,//文件名代号
	S07_FILEACKFLAG,//应答号	
	S07_FILEOFFSET_LL,//文件偏移
	S07_FILEOFFSET_LH,
	S07_FILEOFFSET_HL,
	S07_FILEOFFSET_HH,
	S07_CONTENTLEN_L,//内容长度
	S07_CONTENTLEN_H,
};

enum Cmd10MasterOffset{//修改
	M10_INFOBYTESIZE_L,//长度
	M10_INFOBYTESIZE_H,
	M10_CMD,//命令号
	M10_REG_ADDRNUM,//寄存器地址数量为1时M03_REG1_SERIALNUM有效
	M10_REG_SERIALNUM,//寄存器地址1开始的连续个数
	M10_REG_EACHSIZE,
	M10_REG1_ADDRL,//寄存器地址1
	M10_REG1_ADDRH,
	M10_REG1_CONTENT,
};

enum Cmd10SlaveAckFlag{
	S10_CMDERROR,//
	S10_MODFIYOK,//
	S10_REFUSEMODFIY,
};
enum Cmd10SlaveOffset{
	S10_INFOBYTESIZE_L,//长度
	S10_INFOBYTESIZE_H,
	S10_CMD,//命令号
	S10_ACKFLAG,//命令号
};

ReturnStatus ApplyForUser(UserInfo *user);
UserErrorStatus GetMasterUserInfoStatus(UserInfo *user);
UserErrorStatus GetSlaveUserInfoStatus(UserInfo *user);

#endif /* __USERINFO_H */

/* END */
