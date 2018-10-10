/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		db.c
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
/*Include files -----------------------------------------------*/
#include <absacc.h>
#include "db.h"
#include "stm32f10x_gpio.h"
#include "string.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "diskio.h"
#include "sd_diskio.h"
#include "ctrl.h"



/*Defines -----------------------------------------------------*/



/*Extern data & funtion----------------------------------------*/

/*Global data -------------------------------------------------*/
FATFS fs;
ChargeCfgData_t stChargeCfgData;
CHAR acUITDataPath[16]; 
U8 abyDBBuf[1024*3];
CHAR acTempPath[16];		/* 临时路径 */
U16 wdCfgDataNum = CONFIG_DATA_NUM;
U16 wdCurCfgDataIndex = 0;
BOOL bCfgDataExist = FALSE;
U16 wdCurUITFileInex = 0;
U16 wdUITFileNum = 0;
U16 dwdUITFileTotalNum = 0;
U32 dwdUITCnt = 0;
U32 dwdChargeBeginTm = 0;
FIL fp;
BOOL bCfgValid[CONFIG_DATA_NUM];

/*datas & funcitons define area -------------------------------*/
/* castChgCfgData 默认的几组充电配置参数，其中电流电压以0.01A，0.01V为单位，时间以分钟为单位 */
const ChargeCfgData_t castChgCfgData[6] =
{
	{ 75,500,0,75,75,75,798,1146,1470,1560,1380,120,540,0,300,720,240,60000 ,
		{'D', 'I', 'S', '_', 'I', 'U','I','A'}},
	{ 75,500,0,85,85,75,798,1146,1470,1590,1338,120,540,0,300,720,180,60000,
		{'W', 'E', 'T', '\0', '\0', '\0','\0','\0'}},
	{ 75,500,0,75,75,75,798,1146,1440,1560,1380,120,540,0,300,720,0,60000,
		{'G', 'E', 'L', '_', 'I', 'U','U','A'}},
	{ 75,500,0,75,75,75,798,1146,1440,1560,1380,120,540,0,300,720,240,60000,
		{'G', 'E', 'L', '_', 'I', 'U','I','A'}},
	{ 75,500,0,75,75,75,798,1146,1470,1560,1380,120,540,0,300,720,0,60000,
		{'D', 'I', 'S', '_', 'I', 'U','U','A'}},
	{ 75,500,300,75,75,75,798,1146,1470,1560,1380,120,540,180,180,720,0,60000,
		{'T', 'N', '_', 'I', 'U', 'U','A','\0'}},
};


const U16 cbyFlashData[1024] __at (FLASH_CALIB_ADDR) = 
{
	CALIB_DEFAULT_VOLT_VAL, ~CALIB_DEFAULT_VOLT_VAL, CALIB_DEFAULT_CUR_VAL, ~CALIB_DEFAULT_CUR_VAL
};
//u8 a[10] __attribute__ ((at(0x2000002c)));


/*Functions ---------------------------------------------------*/
void mem_cpy (void* dst, const void* src, UINT cnt) 
{
	BYTE *d = (BYTE*)dst;
	const BYTE *s = (const BYTE*)src;

	while (cnt--)
	{
		*d++ = *s++;
	}
}

void mem_set (void* dst, BYTE val, UINT cnt) 
{
	BYTE *d = (BYTE*)dst;

	while (cnt--)
	{
		*d++ = val;
	}
}


/****************************************************************
* Function:		GetChkSum
* Description:	计算获取校验和
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
U16 GetChkSum(U8 *pData, U16 Len)
{
	U16 wdChkSum = 0;
	
	while(Len)
	{
		wdChkSum += *pData;
		pData++;
		Len--;
	}

	wdChkSum = ~wdChkSum;

	return wdChkSum;
}

/****************************************************************
* Function:		GetUITFileName
* Description:	根据文件索引值获取充电数据文件名称
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void GetUITFileName(U16 wdIndex, CHAR *pcName)
{
	pcName[0] = wdIndex/1000%10 + '0';
	pcName[1] = wdIndex/100%10 + '0';
	pcName[2] = wdIndex/10%10 + '0';
	pcName[3] = wdIndex%10 + '0';
	pcName[4] = '.';
	pcName[5] = 'u';
	pcName[6] = 'i';
	pcName[7] = 't';
	pcName[8] = 0;
	
}


/****************************************************************
* Function:		DBInit
* Description:	数据管理部分初始化
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBInit(void)
{
	char path[16];
	INT ret;
	FRESULT fret;
	UINT Count;
	BOOL bFindCfg = FALSE;
	ConfigSetFile_t *pCfgSetFile = NULL;
	ConfigFile_t *pCfgFile = NULL;
	ConfigData_t *pCfgData = NULL;
	UITDataSetFile_t *pUITDataSetFile = NULL;
	DWORD dwdTmp;
	U16 wdTmp;
	static BOOL bFSInit = FALSE;

	if (!bFSInit)
	{
		ret = FATFS_LinkDriver(&SD_Driver,path);
		if (ret)
		{
			return ret;
		}
		ret = disk_initialize(0);
		if (ret)
		{
			return ret;
		}
		ret = f_mount(&fs,"0:/",1);
		if (ret)
		{
			return ret;
		}

		bFSInit = TRUE;
	}

	/* 读取当前曲线配置信息 */
	fret = f_open(&fp,FILE_NAME_CONFIG_SET,FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		return DB_ERR_NO_CFG_SET_FILE;
	}

	fret = f_read(&fp, abyDBBuf, SIZE_CONFIG_SET_FILE, &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_SET_FILE;
	}
	f_close(&fp);

	if (Count != SIZE_CONFIG_SET_FILE)
	{
		return DB_ERR_READ_CFG_SET_FILE;
	}

	pCfgSetFile = (ConfigSetFile_t *)abyDBBuf;
	if ((pCfgSetFile->FileType != FILE_TYPE_CONFIG_SET) 
		|| (pCfgSetFile->SelCfgIndex != (U16)(~pCfgSetFile->NotSelCfgIndex)))
	{
		return DB_ERR_CFG_SET_FILE_DMG;
	}

	wdCurCfgDataIndex = pCfgSetFile->SelCfgIndex;
	//wdCurCfgDataIndex = 0;
	IWDG_ReloadCounter();
	fret = f_open(&fp,FILE_NAME_CONFIG_DATA,FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		return DB_ERR_NO_CFG_DATA_FILE;
	}

	fret = f_read(&fp, abyDBBuf, SIZE_CONFIG_FILE_HEADER, &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}

	if (Count != SIZE_CONFIG_FILE_HEADER)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_SET_FILE;
	}

	pCfgFile = (ConfigFile_t *)abyDBBuf;

	if ((pCfgFile->FileType != FILE_TYPE_CONFIG_DATA) 
		|| (pCfgFile->DataNum != (U16)(~(pCfgFile->NotDataNum))))
	{
		f_close(&fp);
		return DB_ERR_CFG_DATA_FILE_DMG;
	}

	wdCfgDataNum = pCfgFile->DataNum;

	dwdTmp = wdCurCfgDataIndex;
	dwdTmp *= SIZE_CONFIG_DATA;
	dwdTmp += SIZE_CONFIG_FILE_HEADER;

	if (dwdTmp + SIZE_CONFIG_DATA > fp.fsize)
	{
		f_close(&fp);
		return DB_ERR_CFG_DATA_FILE_DMG;
	}
	
	fret=f_lseek(&fp,dwdTmp);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_CFG_DATA_FILE_DMG;
	}

	fret = f_read(&fp, abyDBBuf, SIZE_CONFIG_DATA, &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}

	if (Count != SIZE_CONFIG_DATA)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_SET_FILE;
	}

	pCfgData = (ConfigData_t *)abyDBBuf;

	if (pCfgData->bValid != CONFIG_DATA_VALID)
	{
		f_close(&fp);
		return DB_ERR_CFG_DATA_INVALID;
	}

	wdTmp = GetChkSum((U8 *)pCfgData, SIZE_CONFIG_DATA-sizeof(U16));
	
	if (wdTmp != pCfgData->ChkSum)
	{
		return DB_ERR_CFG_DATA_FILE_DMG;
	}
	
	bCfgDataExist = TRUE;
	wdTmp = byCurType + 1;

	stChargeCfgData.I1 = wdTmp*pCfgData->I1;
	stChargeCfgData.I2 = wdTmp*pCfgData->I2;
	stChargeCfgData.I3 = wdTmp*pCfgData->I3;
	stChargeCfgData.i4 = wdTmp*pCfgData->i4;
	stChargeCfgData.I5 = wdTmp*pCfgData->I5;
	stChargeCfgData.I6 = wdTmp*pCfgData->I6;

	wdTmp = byVoltType + 1;
	stChargeCfgData.u0 = wdTmp*pCfgData->u0;
	stChargeCfgData.u1 = wdTmp*pCfgData->u1;
	stChargeCfgData.Ucv = wdTmp*pCfgData->Ucv;
	stChargeCfgData.U5 = wdTmp*pCfgData->U5;
	stChargeCfgData.U6 = wdTmp*pCfgData->U6;

	stChargeCfgData.To = pCfgData->To;
	stChargeCfgData.T1 = pCfgData->T1;
	stChargeCfgData.T2 = pCfgData->T2;
	stChargeCfgData.T3 = pCfgData->T3;
	stChargeCfgData.T4 = pCfgData->T4;
	stChargeCfgData.T5 = pCfgData->T5;
	stChargeCfgData.T6 = pCfgData->T6;

	mem_cpy(stChargeCfgData.acName, pCfgData->acName, CONFIG_NAME_LEN);
	
	f_close(&fp);

	/* 读取充电数据记录情况 */
	wdTmp = 1;
	IWDG_ReloadCounter();
	fret = f_open(&fp,FILE_NAME_UIT_DATA_SET,FA_OPEN_EXISTING | FA_READ | FA_WRITE);
	if (fret)
	{
		/* 如果文件不存在，说明目前还没有保存过数据 */
		wdTmp = 0;
		//return DB_ERR_NO_ERROR;
	}

	if (wdTmp)
	{
		fret = f_read(&fp, abyDBBuf, SIZE_UIT_SET_FILE, &Count);
		if (fret)
		{
			//f_close(&fp);
			//return DB_ERR_READ_UIT_DATA_SET_FILE;
			wdTmp = 0;
		}
	}

	if (wdTmp)
	{
		if (Count != SIZE_UIT_SET_FILE)
		{
			//f_close(&fp);
			//return DB_ERR_READ_CFG_SET_FILE;
			wdTmp = 0;
		}
	}

	if (wdTmp)
	{
		pUITDataSetFile = (UITDataSetFile_t *)abyDBBuf;
		if (pUITDataSetFile->FileType != FILE_TYPE_UIT_DATA_SET)
		{
			wdTmp = 0;
		}
		
	}

	if (wdTmp)
	{
		if (pUITDataSetFile->CurIndex == (U16)(~pUITDataSetFile->NotCurIndex))
		{
			wdCurUITFileInex = pUITDataSetFile->CurIndex;
		}

		if (pUITDataSetFile->Num == (U16)(~pUITDataSetFile->NotNum))
		{
			wdUITFileNum = pUITDataSetFile->Num;
		}

		if (pUITDataSetFile->TotalNum == (U32)(~pUITDataSetFile->NotTotalNum))
		{
			dwdUITFileTotalNum = pUITDataSetFile->TotalNum;
		}

		if ((pUITDataSetFile->CurIndex != (U16)(~pUITDataSetFile->NotCurIndex))
			|| (pUITDataSetFile->Num != (U16)(~pUITDataSetFile->NotNum))
			|| (pUITDataSetFile->TotalNum != (U32)(~pUITDataSetFile->NotTotalNum)))
		{
			wdTmp = 0;
		}
			
	}

	if (!wdTmp)
	{
		f_close(&fp);
		wdCurUITFileInex = 0;
		wdUITFileNum = 0;
		dwdUITFileTotalNum = 0;
		f_unlink(FILE_NAME_UIT_DATA_SET);

		fret = f_open(&fp,FILE_NAME_UIT_DATA_SET, FA_CREATE_NEW | FA_WRITE);
		if (FR_OK != fret)
		{
			return DB_ERR_UIT_DATA_SET_FILE_CREATE;
		}
	
	
		pUITDataSetFile = (UITDataSetFile_t *)abyDBBuf;
		pUITDataSetFile->Version = FILE_VER_UIT_DATA_SET;
		pUITDataSetFile->FileType = FILE_TYPE_UIT_DATA_SET;
		pUITDataSetFile->CurIndex = wdCurUITFileInex;
		pUITDataSetFile->NotCurIndex = ~pUITDataSetFile->CurIndex;
		pUITDataSetFile->Num = wdUITFileNum;
		pUITDataSetFile->NotNum = ~pUITDataSetFile->Num;
		pUITDataSetFile->TotalNum = dwdUITFileTotalNum;
		pUITDataSetFile->NotTotalNum = ~pUITDataSetFile->TotalNum;

		fret = f_write(&fp, abyDBBuf, SIZE_UIT_SET_FILE, &Count);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_SET_FILE_WR;
		}

		if (Count != SIZE_UIT_SET_FILE)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_SET_FILE_WR;
		}
	}

	f_close(&fp);
	
	return DB_ERR_NO_ERROR;
}



/****************************************************************
* Function:		DBSaveUITemp
* Description:	保存电压电流温度值到当前文件
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBSaveUITemp(UIT_t *pUIT, U32 dwCnt)
{
	INT ret;
	FRESULT fret;
	UITDataFileHeader_t stUITDataFileHeader;
	UITData_t stUITData;
	UINT Count;
	U32 dwdTmp;

	fret = f_open(&fp,acUITDataPath,FA_OPEN_EXISTING | FA_WRITE);
	if (FR_OK != fret)
	{
		/* 如果不存在，就新建一个，并保存文件头进去 */
		stUITDataFileHeader.FileType = FILE_TYPE_UIT_DATA;
		stUITDataFileHeader.Version = FILE_VER_UIT_DATA;
		stUITDataFileHeader.TmSpace = SAVE_UIT_TM_SPACE/2;
		stUITDataFileHeader.NotTmSpace = ~stUITDataFileHeader.TmSpace;
		fret = f_open(&fp,acUITDataPath,FA_CREATE_ALWAYS | FA_WRITE);
		if (FR_OK != fret)
		{
			return DB_ERR_UIT_DATA_FILE_CREATE;
		}
		fret = f_write(&fp, &stUITDataFileHeader, SIZE_UIT_FILE_HEADER, &Count);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}

		if (Count != SIZE_UIT_FILE_HEADER)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}		
	}

	stUITData.U = pUIT->U;
	stUITData.I = pUIT->I;
	stUITData.T1 = pUIT->T1;
	stUITData.T2 = pUIT->T2;
	stUITData.Step = pUIT->Step;
	stUITData.ChkSum = (U8)GetChkSum((U8 *)(&stUITData),
		SIZE_UIT_DATA - sizeof(stUITData.ChkSum));

	
	#if 1
	
	if (fp.fsize < SIZE_UIT_FILE_HEADER + dwCnt*SIZE_UIT_DATA)
	{
		dwdTmp = dwCnt*SIZE_UIT_DATA + SIZE_UIT_FILE_HEADER - fp.fsize;
		for (Count = 0; Count < dwdTmp; Count++)
		{
			abyDBBuf[Count] = 0xff;
		}

		fret=f_lseek(&fp,fp.fsize);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}
					
		fret = f_write(&fp, abyDBBuf, dwdTmp, &Count);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}

		if (Count != dwdTmp)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}	
	}
	#else
	if ((fp.fsize - SIZE_UIT_FILE_HEADER)%SIZE_UIT_DATA)
	{
		dwdTmp = SIZE_UIT_DATA - ((fp.fsize - SIZE_UIT_FILE_HEADER)%SIZE_UIT_DATA);
		for (Count = 0; Count < dwdTmp; Count++)
		{
			abyDBBuf[Count] = 0xff;
		}

		fret=f_lseek(&fp,fp.fsize);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}
					
		fret = f_write(&fp, abyDBBuf, dwdTmp, &Count);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}

		if (Count != dwdTmp)
		{
			f_close(&fp);
			return DB_ERR_UIT_DATA_FILE_WR;
		}		
	}
	#endif

	dwdTmp = dwCnt*SIZE_UIT_DATA + SIZE_UIT_FILE_HEADER;
	
	fret=f_lseek(&fp,dwdTmp);
	fret = f_write(&fp, &stUITData, SIZE_UIT_DATA, &Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_FILE_WR;
	}

	if (Count != SIZE_UIT_DATA)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_FILE_WR;
	}
	
	f_close(&fp);
	return DB_ERR_NO_ERROR;
}

/****************************************************************
* Function:		DBLoadUI
* Description:	从文件中读取电压电流值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBLoadUI(U16 wdFileIndex, 
				U32 dwdDataIdxBegin, 
				U32 dwdDataNum, 
				U32 dwdSpace,
				U16 *pUData,
				U16 *pIData)
{
	FRESULT fret;
	UITDataFileHeader_t stUITDataFileHeader;
	UITData_t stUITData;
	UINT Count;
	U32 dwdTmp;
	U32 dwdSpaceBak;
	U32 i;
	U16 *pUDataBak;
	U16 *pIDataBak;
	
	GetUITFileName(wdFileIndex,acTempPath);
	fret = f_open(&fp,acTempPath,FA_OPEN_EXISTING | FA_READ);
	if (FR_OK != fret)
	{
		return DB_ERR_NO_UIT_DATA_FILE;
	}
	
	fret=f_lseek(&fp,0);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}

	fret = f_read(&fp,&stUITDataFileHeader,SIZE_UIT_FILE_HEADER,&Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}
	if (SIZE_UIT_FILE_HEADER != Count)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}

	if (stUITDataFileHeader.FileType != FILE_TYPE_UIT_DATA)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_FILE_DMG;
	}

	pUDataBak = pUData;
	pIDataBak = pIData;

	dwdSpaceBak = dwdSpace*SIZE_UIT_DATA;
	for (i = 0, dwdTmp = (dwdDataIdxBegin*SIZE_UIT_DATA + SIZE_UIT_FILE_HEADER); 
		i < dwdDataNum; 
		i++, dwdTmp += dwdSpaceBak, pUDataBak++, pIDataBak++)
	{

		fret=f_lseek(&fp,dwdTmp);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_READ_UIT_DATA_FILE;
		}

		fret = f_read(&fp,&stUITData,SIZE_UIT_DATA,&Count);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_READ_UIT_DATA_FILE;
		}
		if (SIZE_UIT_DATA != Count)
		{
			f_close(&fp);
			return DB_ERR_READ_UIT_DATA_FILE;
		}

		if (stUITData.ChkSum != 
			(U8)GetChkSum((U8 *)(&stUITData),SIZE_UIT_DATA - sizeof(stUITData.ChkSum)))
		{
			//f_close(&fp);
			//return DB_ERR_UIT_DATA_FILE_DMG;
			/* 如果校验失败 */
			if (0 == i)
			{
				*pUDataBak = 0;
				*pIDataBak = 0;
			}
			else
			{
				*pUDataBak = *(pUDataBak - 1);
				*pIDataBak = *(pIDataBak - 1);
			}
			continue;
		}

		*pUDataBak = stUITData.U;
		*pIDataBak = stUITData.I;
	}
		
	f_close(&fp);
	return DB_ERR_NO_ERROR;
	
}

/****************************************************************
* Function:		DBReadUIFile
* Description:	读取电压电流值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBReadUIFile(U16 wdFileIndex, 
				U8 *pData,
				U32 dwdOffset,
				U32 dwdLen, 
				U32 *pdwdReadLen
				)
{
	FRESULT fret;
	
	GetUITFileName(wdFileIndex,acTempPath);
	fret = f_open(&fp,acTempPath,FA_OPEN_EXISTING | FA_READ);
	if (FR_OK != fret)
	{
		return DB_ERR_NO_UIT_DATA_FILE;
	}
	fret=f_lseek(&fp,dwdOffset);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE_OUT_OF_RANGE;
	}
		
	fret = f_read(&fp,pData,dwdLen,pdwdReadLen);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}
	
	f_close(&fp);
	return DB_ERR_NO_ERROR;
	
}

/****************************************************************
* Function:		DBGetUIFileSize
* Description:	获取充电文件长度
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBGetUIFileSize(U16 wdFileIndex, U32 *pdwdFileSize, U32 *pdwdCnt)
{
	FRESULT fret;
	UITDataFileHeader_t stUITDataFileHeader;
	UINT Count;
	U32 dwdTmp;
	
	GetUITFileName(wdFileIndex,acTempPath);
	fret = f_open(&fp,acTempPath,FA_OPEN_EXISTING | FA_READ);
	if (FR_OK != fret)
	{
		return DB_ERR_NO_UIT_DATA_FILE;
	}

	fret = f_read(&fp,&stUITDataFileHeader,SIZE_UIT_FILE_HEADER,&Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}
	if (SIZE_UIT_FILE_HEADER != Count)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}

	if (stUITDataFileHeader.FileType != FILE_TYPE_UIT_DATA)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_FILE_DMG;
	}

	if (pdwdFileSize)
	{
		*pdwdFileSize = fp.fsize;
	}
	
	dwdTmp = fp.fsize;
	dwdTmp -= SIZE_UIT_FILE_HEADER;
	dwdTmp /= SIZE_UIT_DATA;
	if (pdwdCnt)
	{
		*pdwdCnt = dwdTmp;
	}
		
	f_close(&fp);
	return DB_ERR_NO_ERROR;
	
}


/****************************************************************
* Function:		DBLoadUITemp
* Description:	从文件中读取电压电流温度值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBLoadUITemp(U16 wdFileIndex, U32 dwdDataIndex, UIT_t *pUIT)
{
	FRESULT fret;
	UITDataFileHeader_t stUITDataFileHeader;
	UITData_t stUITData;
	UINT Count;
	U32 dwdTmp;
	
	GetUITFileName(wdFileIndex,acTempPath);
	fret = f_open(&fp,acTempPath,FA_OPEN_EXISTING | FA_READ);
	if (FR_OK != fret)
	{
		return DB_ERR_NO_UIT_DATA_FILE;
	}

	fret = f_read(&fp,&stUITDataFileHeader,SIZE_UIT_FILE_HEADER,&Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}
	if (SIZE_UIT_FILE_HEADER != Count)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}

	if (stUITDataFileHeader.FileType != FILE_TYPE_UIT_DATA)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_FILE_DMG;
	}

	dwdTmp = dwdDataIndex;
	dwdTmp *= SIZE_UIT_DATA;
	dwdTmp += SIZE_UIT_FILE_HEADER;
	
	fret=f_lseek(&fp,dwdTmp);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}

	fret = f_read(&fp,&stUITData,SIZE_UIT_DATA,&Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}
	if (SIZE_UIT_DATA != Count)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}

	if (stUITData.ChkSum != 
		(U8)GetChkSum((U8 *)(&stUITData),SIZE_UIT_DATA - sizeof(stUITData.ChkSum)))
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_FILE_DMG;
	}

	pUIT->U = stUITData.U;
	pUIT->I = stUITData.I;
	pUIT->T1= stUITData.T1;
	pUIT->T2= stUITData.T2;
	
	f_close(&fp);
	return DB_ERR_NO_ERROR;
	
}

/****************************************************************
* Function:		DBGetCT
* Description:	根据特定充电文件计算电量
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBGetCT(U16 wdFileIndex, U32 *pdwdCapacity, U32 *pdwdTmLen)
{
	FRESULT fret;
	UITDataFileHeader_t stUITDataFileHeader;
	UINT Count;
	U32 dwdTmp;
	U16 wdTmSpace;
	U32 i;
	U32 Offset;
	U8 tmpBuf[12];
	U16 wdI;
	U32 dwdC;
	uint64_t llTmp;
	
	
	GetUITFileName(wdFileIndex,acTempPath);
	fret = f_open(&fp,acTempPath,FA_OPEN_EXISTING | FA_READ);
	if (FR_OK != fret)
	{
		return DB_ERR_NO_UIT_DATA_FILE;
	}

	fret = f_read(&fp,&stUITDataFileHeader,SIZE_UIT_FILE_HEADER,&Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}
	if (SIZE_UIT_FILE_HEADER != Count)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_FILE;
	}

	wdTmSpace = stUITDataFileHeader.TmSpace;

	if (stUITDataFileHeader.FileType != FILE_TYPE_UIT_DATA)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_FILE_DMG;
	}
	
	dwdTmp = fp.fsize;
	dwdTmp -= SIZE_UIT_FILE_HEADER;
	dwdTmp /= SIZE_UIT_DATA;
	if (pdwdTmLen)
	{
		*pdwdTmLen = (dwdTmp - 1)*wdTmSpace;
	}

	for (i = 0, Offset = SIZE_UIT_FILE_HEADER + 2, llTmp = 0; 
		i < dwdTmp; 
		i++, Offset += sizeof (UITData_t))
	{
		fret=f_lseek(&fp,Offset);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_READ_UIT_DATA_FILE;
		}
	
		fret = f_read(&fp, tmpBuf, 2, &Count);
		if (FR_OK != fret)
		{
			f_close(&fp);
			return DB_ERR_READ_UIT_DATA_FILE;
		}
		if (2 != Count)
		{
			f_close(&fp);
			return DB_ERR_READ_UIT_DATA_FILE;
		}
		wdI = *((U16 *)tmpBuf);

		if ((0 == i) || (i == (dwdTmp - 1)))
		{
			llTmp += wdI/2;
		}
		else
		{
			llTmp += wdI;
		}

		if (i%1000)
		{
			IWDG_ReloadCounter();
		}
		
	}

	llTmp *= wdTmSpace;
	llTmp /= 3600*10;
	dwdC = (U32)llTmp;

	if (pdwdCapacity)
	{
		*pdwdCapacity = dwdC;
	}
	
	f_close(&fp);
	
}


/****************************************************************
* Function:		DBSaveUITSet
* Description:	保存充电数据数量及当前计数值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBSaveUITSet(U16 Index, U16 Num, U32 TotalNum)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	UITDataSetFile_t stUITDataSet;

	stUITDataSet.Version = FILE_VER_UIT_DATA_SET;
	stUITDataSet.FileType = FILE_TYPE_UIT_DATA_SET;
	stUITDataSet.CurIndex = Index;
	stUITDataSet.NotCurIndex = ~Index;
	stUITDataSet.Num = Num;
	stUITDataSet.NotNum = ~Num;
	stUITDataSet.TotalNum = TotalNum;
	stUITDataSet.NotTotalNum = ~TotalNum;
	
	f_unlink(FILE_NAME_TMP);

	fret = f_open(&fp,FILE_NAME_TMP,FA_CREATE_ALWAYS | FA_WRITE);
	if (FR_OK != fret)
	{
		return DB_ERR_UIT_DATA_SET_FILE_CREATE;
	}
	
	fret = f_write(&fp, &stUITDataSet, SIZE_UIT_SET_FILE, &Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_SET_FILE_WR;
	}

	if (Count != SIZE_UIT_SET_FILE)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_SET_FILE_WR;
	}

	f_close(&fp);

	f_unlink(FILE_NAME_UIT_DATA_SET);
	f_rename(FILE_NAME_TMP,FILE_NAME_UIT_DATA_SET);

	return DB_ERR_NO_ERROR;
	
}

/****************************************************************
* Function:		DBLoadUITSet
* Description:	获取充电文件当前计数值及总数
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBLoadUITSet(U16 *pIndex, U16 *pNum, U32 *pTotalNum)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	UITDataSetFile_t *pstUITDataSet = NULL;
	
	fret = f_open(&fp,FILE_NAME_UIT_DATA_SET,FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		/* 如果文件不存在，说明目前还没有保存过数据 */
		return DB_ERR_NO_UIT_DATA_SET_FILE;
	}

	fret = f_read(&fp, abyDBBuf, SIZE_UIT_SET_FILE, &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_SET_FILE;
	}

	if (Count != SIZE_UIT_SET_FILE)
	{
		f_close(&fp);
		return DB_ERR_READ_UIT_DATA_SET_FILE;
	}
	pstUITDataSet = (UITDataSetFile_t *)abyDBBuf;
	if (pstUITDataSet->FileType != FILE_TYPE_UIT_DATA_SET)
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_SET_FILE_DMG;
	}

	if ((pstUITDataSet->CurIndex != (U16)(~pstUITDataSet->NotCurIndex))
		|| (pstUITDataSet->Num != (U16)(~pstUITDataSet->NotNum))
		|| (pstUITDataSet->TotalNum != (U32)(~pstUITDataSet->NotTotalNum)))
	{
		f_close(&fp);
		return DB_ERR_UIT_DATA_SET_FILE_DMG;
	}

	*pIndex = pstUITDataSet->CurIndex;
	*pNum = pstUITDataSet->Num;
	*pTotalNum = pstUITDataSet->TotalNum;

	f_close(&fp);
	return DB_ERR_NO_ERROR;
}


/****************************************************************
* Function:		DBSaveCfg
* Description:	保存配置曲线值到文件
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBSaveCfg(U16 wdIndex, ConfigData_t* pConfigData)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	ConfigFile_t *pstCfgFile = NULL;

	if (wdIndex >= CONFIG_DATA_NUM)
	{
		return DB_ERR_CFG_INDEX_OUT_RANGE;
	}

	pstCfgFile = (ConfigFile_t *)abyDBBuf;

	fret = f_open(&fp,FILE_NAME_CONFIG_DATA, FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		/* 如果文件不存在，说明目前还没有保存过数据 */
		//return DB_ERR_NO_CFG_DATA_FILE;
		mem_set(pstCfgFile, 0xff, sizeof(ConfigFile_t));
		pstCfgFile->FileType = FILE_TYPE_CONFIG_DATA;
		pstCfgFile->Version = FILE_VER_CONFIG_DATA;
		pstCfgFile->DataNum = CONFIG_DATA_NUM;
		pstCfgFile->NotDataNum = ~pstCfgFile->DataNum;
	}
	else
	{

		fret = f_read(&fp, abyDBBuf, SIZE_CONFIG_FILE, &Count);
		f_close(&fp);
		if (fret)
		{
			return DB_ERR_READ_CFG_DATA_FILE;
		}

		if (Count != SIZE_CONFIG_FILE)
		{
			return DB_ERR_READ_CFG_DATA_FILE;
		}
		
	}
	
	memcpy(&pstCfgFile->Data[wdIndex], pConfigData, SIZE_CONFIG_DATA);
	
	
	f_unlink(FILE_NAME_TMP);
	
	fret = f_open(&fp, FILE_NAME_TMP, FA_CREATE_ALWAYS | FA_WRITE);
	if (FR_OK != fret)
	{
		return DB_ERR_CFG_DATA_FILE_CREATE;
	}

	fret = f_write(&fp, abyDBBuf, SIZE_CONFIG_FILE, &Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_CFG_DATA_FILE_WR;
	}

	if (Count != SIZE_CONFIG_FILE)
	{
		f_close(&fp);
		return DB_ERR_CFG_DATA_FILE_WR;
	}

	f_close(&fp);

	f_unlink(FILE_NAME_CONFIG_DATA);
	f_rename(FILE_NAME_TMP,FILE_NAME_CONFIG_DATA);

	return DB_ERR_NO_ERROR;
}

/****************************************************************
* Function:		DBLoadCfg
* Description:	从文件加载配置曲线值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBLoadCfg(U16 wdIndex, ConfigData_t* pConfigData)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	ConfigFile_t *pstCfgFile = NULL;

	if (wdIndex >= CONFIG_DATA_NUM)
	{
		return DB_ERR_CFG_INDEX_OUT_RANGE;
	}

	fret = f_open(&fp,FILE_NAME_CONFIG_DATA, FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		/* 如果文件不存在，说明目前还没有保存过数据 */
		return DB_ERR_NO_CFG_DATA_FILE;
	}

	fret = f_read(&fp, abyDBBuf, SIZE_CONFIG_FILE, &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}

	if (Count != SIZE_CONFIG_FILE)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}
	f_close(&fp);

	pstCfgFile = (ConfigFile_t *)abyDBBuf;

	memcpy(pConfigData, &pstCfgFile->Data[wdIndex], SIZE_CONFIG_DATA);
	return 0;
}

/****************************************************************
* Function:		DBLoadCfgEx
* Description:	与DBLoadCfg相比较，差别在于数据被加载的目的地数据结构有所不同
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBLoadCfgEx(U16 wdIndex, ChargeCfgData_t* pConfigData)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	U16 wdTmp;
	ConfigFile_t *pstCfgFile = NULL;
	ConfigData_t *pCfgData = NULL;

	if (wdIndex >= CONFIG_DATA_NUM)
	{
		return DB_ERR_CFG_INDEX_OUT_RANGE;
	}

	fret = f_open(&fp,FILE_NAME_CONFIG_DATA, FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		/* 如果文件不存在，说明目前还没有保存过数据 */
		return DB_ERR_NO_CFG_DATA_FILE;
	}

	fret = f_read(&fp, abyDBBuf, SIZE_CONFIG_FILE, &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}

	if (Count != SIZE_CONFIG_FILE)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}
	f_close(&fp);

	pstCfgFile = (ConfigFile_t *)abyDBBuf;

	//memcpy(pConfigData, &pstCfgFile->Data[wdIndex], SIZE_CONFIG_DATA);
	pCfgData = &(pstCfgFile->Data[wdIndex]);

	wdTmp = GetChkSum((U8 *)pCfgData, SIZE_CONFIG_DATA-sizeof(U16));
	
	if (wdTmp != pCfgData->ChkSum)
	{
		return DB_ERR_CFG_DATA_FILE_DMG;
	}
	
	wdTmp = byCurType + 1;

	pConfigData->I1 = wdTmp*pCfgData->I1;
	pConfigData->I2 = wdTmp*pCfgData->I2;
	pConfigData->I3 = wdTmp*pCfgData->I3;
	pConfigData->i4 = wdTmp*pCfgData->i4;
	pConfigData->I5 = wdTmp*pCfgData->I5;
	pConfigData->I6 = wdTmp*pCfgData->I6;

	wdTmp = byVoltType + 1;
	pConfigData->u0 = wdTmp*pCfgData->u0;
	pConfigData->u1 = wdTmp*pCfgData->u1;
	pConfigData->Ucv = wdTmp*pCfgData->Ucv;
	pConfigData->U5 = wdTmp*pCfgData->U5;
	pConfigData->U6 = wdTmp*pCfgData->U6;

	pConfigData->To = pCfgData->To;
	pConfigData->T1 = pCfgData->T1;
	pConfigData->T2 = pCfgData->T2;
	pConfigData->T3 = pCfgData->T3;
	pConfigData->T4 = pCfgData->T4;
	pConfigData->T5 = pCfgData->T5;
	pConfigData->T6 = pCfgData->T6;

	mem_cpy(pConfigData->acName, pCfgData->acName, CONFIG_NAME_LEN);

	
	return 0;
}


/****************************************************************
* Function:		DBCfgValidCheck
* Description:	检测索引值对应配置曲线是否有效
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBCfgValidCheck(U16 wdIndex, BOOL *pbValid)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	U16 wdValid;
	ConfigFile_t *pstCfgFile = NULL;

	*pbValid = FALSE;
	
	if (wdIndex >= CONFIG_DATA_NUM)
	{
		return DB_ERR_CFG_INDEX_OUT_RANGE;
	}

	fret = f_open(&fp,FILE_NAME_CONFIG_DATA, FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		/* 如果文件不存在，说明目前还没有保存过数据 */
		return DB_ERR_NO_CFG_DATA_FILE;
	}

	if (fp.fsize != SIZE_CONFIG_FILE)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}

	dwdTmp = SIZE_CONFIG_FILE_HEADER + wdIndex*SIZE_CONFIG_DATA + 44;

	fret=f_lseek(&fp,dwdTmp);
	
	
	fret = f_read(&fp, abyDBBuf, sizeof(U16), &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}

	if (Count != sizeof(U16))
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_DATA_FILE;
	}
	f_close(&fp);

	wdValid = *((U16 *)abyDBBuf);

	if (wdValid == 0x55)
	{
		*pbValid = TRUE;
	}

	
	return 0;
}

/****************************************************************
* Function:		DBSaveCfgSet
* Description:	保存当前所用配置曲线索引值到文件中
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBSaveCfgSet(U16 wdSelIndex)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	ConfigSetFile_t stCfgSetFile;

	stCfgSetFile.Version = FILE_VER_CONFIG_SET;
	stCfgSetFile.FileType = FILE_TYPE_CONFIG_SET;
	stCfgSetFile.SelCfgIndex = wdSelIndex;
	stCfgSetFile.NotSelCfgIndex = ~wdSelIndex;
	
	f_unlink(FILE_NAME_TMP);

	fret = f_open(&fp,FILE_NAME_TMP,FA_CREATE_ALWAYS | FA_WRITE);
	if (FR_OK != fret)
	{
		return DB_ERR_CFG_SET_FILE_CREATE;
	}
	
	fret = f_write(&fp, &stCfgSetFile, SIZE_CONFIG_SET_FILE, &Count);
	if (FR_OK != fret)
	{
		f_close(&fp);
		return DB_ERR_CFG_SET_FILE_WR;
	}

	if (Count != SIZE_CONFIG_SET_FILE)
	{
		f_close(&fp);
		return DB_ERR_CFG_SET_FILE_WR;
	}

	f_close(&fp);

	f_unlink(FILE_NAME_CONFIG_SET);
	f_rename(FILE_NAME_TMP,FILE_NAME_CONFIG_SET);

	return DB_ERR_NO_ERROR;
}

/****************************************************************
* Function:		DBLoadCfgSet
* Description:	从文件中加载当前所用配置曲线索引值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBLoadCfgSet(U16 *pwdSelIndex)
{
	FRESULT fret;
	UINT Count;
	U32 dwdTmp;
	ConfigSetFile_t *pstCfgSet = NULL;
	
	fret = f_open(&fp,FILE_NAME_CONFIG_SET,FA_OPEN_EXISTING | FA_READ);
	if (fret)
	{
		/* 如果文件不存在，说明目前还没有保存过数据 */
		return DB_ERR_NO_CFG_SET_FILE;
	}

	fret = f_read(&fp, abyDBBuf, SIZE_CONFIG_SET_FILE, &Count);
	if (fret)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_SET_FILE;
	}

	if (Count != SIZE_CONFIG_SET_FILE)
	{
		f_close(&fp);
		return DB_ERR_READ_CFG_SET_FILE;
	}
	pstCfgSet = (ConfigSetFile_t *)abyDBBuf;
	if (pstCfgSet->FileType != FILE_TYPE_CONFIG_SET)
	{
		f_close(&fp);
		return DB_ERR_CFG_SET_FILE_DMG;
	}

	if (pstCfgSet->SelCfgIndex != (U16)(~pstCfgSet->NotSelCfgIndex))
	{
		f_close(&fp);
		return DB_ERR_CFG_SET_FILE_DMG;
	}

	*pwdSelIndex = pstCfgSet->SelCfgIndex;
	f_close(&fp);
	
	return DB_ERR_NO_ERROR;

}

/****************************************************************
* Function:		DBSaveCalibValue
* Description:	保存电压电流校准值到FLASH中
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBSaveCalibValue(U16 wdVolt, U16 wdCurrent)
{
	FLASH_Status Status;
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	Status = FLASH_ErasePage(FLASH_CALIB_ADDR);
	if (FLASH_COMPLETE == Status)
	{
		Status = FLASH_ProgramHalfWord(FLASH_CALIB_ADDR,wdVolt);
		if (FLASH_COMPLETE == Status)
		{
			Status = FLASH_ProgramHalfWord(FLASH_CALIB_ADDR + 2,~wdVolt);
			if (FLASH_COMPLETE == Status)
			{
				Status = FLASH_ProgramHalfWord(FLASH_CALIB_ADDR + 4,wdCurrent);
				if (FLASH_COMPLETE == Status)
				{
					Status = FLASH_ProgramHalfWord(FLASH_CALIB_ADDR + 6,~wdCurrent);
				}
			}
		}
	}
	

	if (FLASH_COMPLETE == Status)
	{
		return DB_ERR_NO_ERROR;
	}
	else
	{
		return DB_ERR_SAVE_CALIB_VAL;
	}
}

/****************************************************************
* Function:		DBLoadCalibValue
* Description:	从FLASH中加载电压电流校准值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
INT DBLoadCalibValue(U16 *pwdVolt, U16 *pwdCurrent)
{
	U16 wdTmp1;
	U16 wdNotTmp1;
	U16 wdTmp2;
	U16 wdNotTmp2;
	
	wdTmp1 = cbyFlashData[0];
	wdNotTmp1 = cbyFlashData[1];
	wdNotTmp1 = ~wdNotTmp1;
	wdTmp2 = cbyFlashData[2];
	wdNotTmp2 = cbyFlashData[3];
	wdNotTmp2 = ~wdNotTmp2;
	if ((wdTmp1 != wdNotTmp1) || (wdTmp2 != wdNotTmp2))
	{
		return DB_ERR_LOAD_CALIB_VAL;
	}

	if (pwdVolt)
	{
		*pwdVolt = wdTmp1;
	}
	
	if (pwdCurrent)
	{
		*pwdCurrent = wdTmp2;
	}
	
	return DB_ERR_NO_ERROR;
}



/* end of file */


