/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		db.h
 Author:		WangFeng
 Version:		1.00
 Date:			2014-11-8
 Description:	
 
 Others:		
 
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 History
 ----------------------------------------------------------------
 Index:			1
 Date:			2014-11-8
 Author:		WangFeng
 Modification:	Create this file
****************************************************************/

#ifndef __DB_H
#define	__DB_H

#include "def.h"
#include "stm3210x_eval.h"


#ifdef __cplusplus
	extern "C" {
#endif

#define FLASH_CALIB_ADDR		0x08016000
#define CALIB_DEFAULT_VOLT_VAL	(2353*4095/3300)
#define CALIB_DEFAULT_CUR_VAL   (13*4095/33)

#define CONFIG_DATA_NUM			50
#define CHARGE_DATA_NUM			10000
#define CONFIG_NAME_LEN			8

#define FILE_TYPE_CONFIG_DATA	0x11
#define FILE_TYPE_CONFIG_SET	0x12
#define FILE_TYPE_UIT_DATA		0x13
#define FILE_TYPE_UIT_DATA_SET	0x14

#define CONFIG_DATA_VALID		0x55

#define FILE_VER_CONFIG_DATA	0x0100
#define FILE_VER_CONFIG_SET		0x0100
#define FILE_VER_UIT_DATA		0x0100
#define FILE_VER_UIT_DATA_SET	0x0100



__packed typedef struct ConfigData_s
{
	U16 I1;
	U16 I2;
	U16 I3;
	U16 i4;
	U16 I5;
	U16 I6;
	U16 u0;
	U16 u1;
	U16 Ucv;
	U16 U5;
	U16 U6;
	U16 T1;
	U16 T2;
	U16 T3;
	U16 T4;
	U16 To;
	U16 T5;
	U16 T6;
	CHAR acName[CONFIG_NAME_LEN];
	U16 bValid;		/* 有效: 0x55 */
	U16 ChkSum;
} ConfigData_t;

typedef struct ChargeCfgData_s
{
	U16 I1;
	U16 I2;
	U16 I3;
	U16 i4;
	U16 I5;
	U16 I6;
	U16 u0;
	U16 u1;
	U16 Ucv;
	U16 U5;
	U16 U6;
	U16 T1;
	U16 T2;
	U16 T3;
	U16 T4;
	U16 To;
	U16 T5;
	U16 T6;
	CHAR acName[CONFIG_NAME_LEN];
	
} ChargeCfgData_t;


__packed typedef struct ConfigFile_s
{
	U16 Version;   		/* 当前版本 V1.0: 0x0100 */
	U16 FileType; 		/* 0x11 */
	U16 DataNum;   		/* 文件包含的配置数据组数 */
	U16 NotDataNum;     /* 文件包含的配置数据组数取反值 */
	ConfigData_t Data[CONFIG_DATA_NUM];
} ConfigFile_t;

__packed typedef struct ConfigSetFile_s
{
	U16 Version;     	/* 当前版本 V1.0: 0x0100 */
	U16 FileType;    	/* 0x12 */
	U16 SelCfgIndex; 	/* 当前选用的配置序号 */
	U16 NotSelCfgIndex; /* SelCfgIndex取反: NotSelCfgIndex = ~SelCfgIndex */
} ConfigSetFile_t;

__packed typedef struct UITData_s
{
	U16 U;     		/* 电压值 */
	U16 I;    		/* 电流值 */
	CHAR T1; 		/* 温度1 */
	CHAR T2; 		/* 温度2 */
	U8 Step;		/* 充电阶段 */
	U8 ChkSum; 	/* 校验和 */
} UITData_t;

__packed typedef struct UITDataFileHeader_s
{
	U16 Version;     /* 当前版本 V1.0: 0x0100 */
	U16 FileType;    /* 0x13 */
	U16 TmSpace;	 /* 数据记录间隔时间 */
	U16 NotTmSpace;	/* TmSpace 取反值 */
} UITDataFileHeader_t;


__packed typedef struct UITDataSetFile_s
{
	U16 Version;     /* 当前版本 V1.0: 0x0100 */
	U16 FileType;    /* 0x14 */
	U16 CurIndex;    /* 最新数据文件索引值 */
	U16 NotCurIndex; /* CurIndex取反: NotCurIndex = ~CurIndex */
	U16 Num;		 /* 文件数目 */
	U16 NotNum;		 /* Num取反: NotNum = ~Num */
	U32 TotalNum;	 /* 累记写入过的文件数目 */
	U32 NotTotalNum; /* TotalNum取反: NotTotalNum = ~TotalNum */
} UITDataSetFile_t;

typedef struct UIT_s
{
	U16 U;     		/* 电压值 */
	U16 I;    		/* 电流值 */
	CHAR T1; 		/* 温度1 */
	CHAR T2; 		/* 温度2 */
	U8 Step;		/* 充电阶段 */
} UIT_t;



#define SIZE_CONFIG_DATA			sizeof(ConfigData_t)
#define SIZE_CONFIG_FILE_HEADER		(4*sizeof(U16))
#define SIZE_CONFIG_FILE			sizeof(ConfigFile_t)
#define SIZE_CONFIG_SET_FILE		sizeof(ConfigSetFile_t)
#define SIZE_UIT_DATA				sizeof(UITData_t)
#define SIZE_UIT_FILE_HEADER		sizeof(UITDataFileHeader_t)
#define SIZE_UIT_SET_FILE			sizeof(UITDataSetFile_t)

#define FILE_NAME_CONFIG_DATA		"0:/cfg.dat"
#define FILE_NAME_CONFIG_SET		"0:/cfg.set"
#define FILE_NAME_UIT_DATA_SET		"0:/uit.set"

#define FILE_NAME_CONFIG_DATA_BAK	"0:/cfg.dat.bak"
#define FILE_NAME_CONFIG_SET_BAK	"0:/cfg.set.bak"
#define FILE_NAME_UIT_DATA_SET_BAK	"0:/uit.set.bak"
#define FILE_NAME_TMP				"0:/tmp"


enum DBError_e
{
	DB_ERR_NO_ERROR	= 0,
	DB_ERR_NO_CFG_DATA_FILE,
	DB_ERR_NO_CFG_SET_FILE,
	DB_ERR_NO_UIT_DATA_SET_FILE,
	DB_ERR_NO_UIT_DATA_FILE,

	DB_ERR_READ_CFG_DATA_FILE,
	DB_ERR_READ_CFG_SET_FILE,
	DB_ERR_READ_UIT_DATA_SET_FILE,
	DB_ERR_READ_UIT_DATA_FILE,
	DB_ERR_READ_UIT_DATA_FILE_OUT_OF_RANGE,

	DB_ERR_CFG_DATA_FILE_DMG,
	DB_ERR_CFG_SET_FILE_DMG,
	DB_ERR_UIT_DATA_SET_FILE_DMG,
	DB_ERR_UIT_DATA_FILE_DMG,

	DB_ERR_CFG_DATA_INVALID,
	DB_ERR_CFG_SET_INVALID,
	DB_ERR_UIT_DATA_SET_INVALID,

	DB_ERR_CFG_DATA_FILE_CREATE,
	DB_ERR_CFG_SET_FILE_CREATE,
	DB_ERR_UIT_DATA_SET_FILE_CREATE,
	DB_ERR_UIT_DATA_FILE_CREATE,

	DB_ERR_CFG_DATA_FILE_WR,
	DB_ERR_CFG_SET_FILE_WR,
	DB_ERR_UIT_DATA_SET_FILE_WR,
	DB_ERR_UIT_DATA_FILE_WR,

	DB_ERR_CFG_INDEX_OUT_RANGE,
	DB_ERR_UIT_FILE_INDEX_OUT_RANGE,

	DB_ERR_LOAD_CALIB_VAL,
	DB_ERR_SAVE_CALIB_VAL

};

extern CHAR acUITDataPath[16]; 
extern U16 wdCfgDataNum;
extern U16 wdCurCfgDataIndex;
extern BOOL bCfgDataExist;
extern U16 wdCurUITFileInex;
extern U16 wdUITFileNum;
extern U16 dwdUITFileTotalNum;
extern U32 dwdUITCnt;
extern ChargeCfgData_t stChargeCfgData;
extern const ChargeCfgData_t castChgCfgData[6];


void mem_cpy (void* dst, const void* src, UINT cnt); 
void mem_set (void* dst, BYTE val, UINT cnt); 

void GetUITFileName(U16 wdIndex, CHAR *pcName);
INT DBInit(void);
INT DBSaveUITemp(UIT_t *pUIT, U32 dwCnt);
INT DBGetUIFileSize(U16 wdFileIndex, U32 *pdwdFileSize, U32 *pdwdCnt);
INT DBLoadUI(U16 wdFileIndex, 
				U32 dwdDataIdxBegin, 
				U32 dwdDataNum, 
				U32 dwdSpace,
				U16 *pUData,
				U16 *pIData);
INT DBReadUIFile(U16 wdFileIndex, 
				U8 *pData,
				U32 dwdOffset,
				U32 dwdLen, 
				U32 *pdwdReadLen
				);
INT DBLoadUITemp(U16 wdFileIndex, U32 dwdDataIndex, UIT_t *pUIT);
INT DBSaveUITSet(U16 Index, U16 Num, U32 TotalNum);
INT DBLoadUITSet(U16 *pIndex, U16 *pNum, U32 *pTotalNum);
INT DBSaveCfg(U16 wdIndex, ConfigData_t* pConfigData);
INT DBLoadCfg(U16 wdIndex, ConfigData_t* pConfigData);
INT DBSaveCfgSet(U16 wdSelIndex);
INT DBLoadCfgSet(U16 *pwdSelIndex);
INT DBCfgValidCheck(U16 wdIndex, BOOL *pbValid);
U16 GetChkSum(U8 *pData, U16 Len);
INT DBGetCT(U16 wdFileIndex, U32 *pdwdCapacity, U32 *pdwdTmLen);
INT DBLoadCfgEx(U16 wdIndex, ChargeCfgData_t* pConfigData);
INT DBSaveCalibValue(U16 wdVolt, U16 wdCurrent);
INT DBLoadCalibValue(U16 *pwdVolt, U16 *pwdCurrent);


#ifdef __cplusplus
}
#endif

#endif
/* end of file */

