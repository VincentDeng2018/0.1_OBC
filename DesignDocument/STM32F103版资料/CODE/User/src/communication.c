/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		communication.c
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
#include "hw_config.h"

#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_pwr.h"

#include "stm3210x_eval.h"

#include "communication.h"
#include "ctrl.h"
#include "db.h"
#include "display.h"





/*Defines -----------------------------------------------------*/
#define SYNC_VAL			0x5555
#define READ_STATE			0x01	/* 获取充电器状态 */
#define SET_CHG_DISABLE		0x10	/* 设置禁止充电 */
#define SET_CFG_INDEX		0x21	/* 设置当前使用的配置曲线 */
#define WRITE_CFG			0x22	/* 写配置曲线信息到充电器 */
#define READ_CFG			0x23	/* 从充电器读取配置曲线信息 */
#define READ_CHG_FILE_LEN	0x31	/* 读指定充电数据文件长度 */
#define READ_CHG_FILE		0x32	/* 读指定充电数据文件内容 */
#define PACKET_ERR			0x80	/* 通信包有误 */

/* 通信数据头 字节对齐 */
__packed typedef struct ComHeader_s
{
	U16 Sync;
	U8 Type;
	U8 NotType;
	U16 Len;
	U16 NotLen;
} ComHeader_t;

/* 充电器状态  */
__packed typedef struct ComStatusData_s
{
	U8 State;
	U8 bChgDisable;
	U8 ChgStep;
	U8 CfgNum;
	U8 CurCfgNum;
	U16 ChgDataNum;
	U32 ChgTmLen;
	U16 U;
	U16 I;
	CHAR T1;
	CHAR T2;
	U8 ErrCode;
	
} ComStatusData_t;



/*Extern data & funtion----------------------------------------*/
extern __IO uint8_t PrevXferComplete;
extern __IO uint32_t bDeviceState;
extern uint8_t Receive_Buffer_Bak[64];
extern ConfigData_t stSelConfigData;


/*Global data -------------------------------------------------*/
uint8_t bRxdProc = FALSE; /* 是否有待处理的数据包 */
U8 SendBuf[64];
BOOL bCom = FALSE;			/* 是否处于通信状态 */
U8 byComWDGCnt = 0;

/*datas & funcitons define area -------------------------------*/

/*Functions ---------------------------------------------------*/
/****************************************************************
* Function:		CommunicateWDG
* Description:	若一定时间内未接收到来自PC的数据包，认为通信中断
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void CommunicateWDG(void)
{
	if (byComWDGCnt < 0xf0)
	{
		byComWDGCnt++;
	}

	if (byComWDGCnt >= 10)
	{
		bCom = FALSE;
		bChgDisable = FALSE;
		bRxdProc = FALSE;
	}
}
/****************************************************************
* Function:		Communicate
* Description:	根据PC发来的数据作出相应的回复
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void Communicate(void)
{
	ComHeader_t *pComHeader = NULL;
	ComHeader_t *pSendHeader = NULL;
	ComStatusData_t *pStatusData = NULL;
	static BOOL bProc = FALSE;
	U32 dwdTmp;
	U16 *pwdChkSum = NULL;
	U8 *pRcvData = NULL;
	U8 *pSendData = NULL;
	BOOL bPacketErr = FALSE;
	static ConfigData_t stConfigData;
	U16 wdTmp;
	U32 dwdOffset;
	U8 byLen;
	U32 dwdReadLen = 0;
	INT ret;
	
	if (bRxdProc)  
	{
		pComHeader = (ComHeader_t *)Receive_Buffer_Bak;
		pSendHeader = (ComHeader_t *)SendBuf;
		
		if (!bProc)
		{
			if ((SYNC_VAL != pComHeader->Sync) || (pComHeader->Type != (U8)(~pComHeader->NotType))
				|| (pComHeader->Len != (U16)(~pComHeader->NotLen)))
			{
				bPacketErr = TRUE;
			}

			if (!bPacketErr)
			{
				if (pComHeader->Len)
				{
					pwdChkSum = (U16*)(Receive_Buffer_Bak + pComHeader->Len + sizeof(ComHeader_t));
					if (*pwdChkSum != GetChkSum((U8 *)(Receive_Buffer_Bak + sizeof(ComHeader_t)), pComHeader->Len))
					{
						bPacketErr = TRUE;
					}
				}
			}

			
			if (bPacketErr)
			{
				pSendHeader->Type = PACKET_ERR;
				pSendHeader->Len = 0;
			}
			else
			{
				pSendData = SendBuf + sizeof(ComHeader_t);
				pRcvData = Receive_Buffer_Bak + sizeof(ComHeader_t);
				switch (pComHeader->Type)
				{
					case READ_STATE:
						pSendHeader->Type = READ_STATE;
						pSendHeader->Len = sizeof(ComStatusData_t);
						pStatusData = (ComStatusData_t *)pSendData;
						pStatusData->State = byState;
						pStatusData->bChgDisable = bChgDisable;
						pStatusData->ChgStep = byChgStep;
						pStatusData->CfgNum = CONFIG_DATA_NUM;
						pStatusData->CurCfgNum = (U8)wdCurCfgDataIndex + 1; // 20170112 对上层应用序号从1开始
						pStatusData->ChgDataNum = wdUITFileNum;
						dwdTmp = RTC_GetCounter();
						dwdTmp -= dwdChargeBeginTm;
						pStatusData->ChgTmLen = dwdTmp;
						pStatusData->U = wdBtVoltage;
						pStatusData->I = wdCurrent;
						pStatusData->T1 = cTemp1;
						pStatusData->T2 = cTemp2;
						pStatusData->ErrCode = byErr;
						break;
					case SET_CHG_DISABLE:
						pSendHeader->Type = SET_CHG_DISABLE;
						pSendHeader->Len = 1;
						pSendData[0] = 0;
						if (pRcvData[0])
						{
							if (CTRL_STATE_CHARGE == byState)
							{
								pSendData[0] = 1;
							}
							else
							{
								bChgDisable = TRUE;
							}
						}
						else
						{
							bChgDisable = FALSE;
						}
						break;
					case SET_CFG_INDEX:
						pSendHeader->Type = SET_CFG_INDEX;
						pSendHeader->Len = 2;
						pSendData[0] = pRcvData[0];
						if (CTRL_STATE_CHARGE == byState)
						{
							pSendData[1] = 0x82;
						}
						else
						{
							if ((pRcvData[0] < 1) ||  (pRcvData[0] > CONFIG_DATA_NUM))
							{
								pSendData[1] = 0x81;
							}
							else
							{
								wdCurCfgDataIndex = pRcvData[0] - 1;  // 20170112  内部从0记，上层应用从1起计
								if (DBSaveCfgSet(wdCurCfgDataIndex))
								{
									pSendData[1] = 0x81;
								}
								else
								{
									if (DBLoadCfg(wdCurCfgDataIndex,&stConfigData))
									{
										pSendData[1] = 0x81;
									}
									else
									{
										pSendData[1] = 0;

										wdTmp = byCurType + 1;

										stChargeCfgData.I1 = wdTmp*stConfigData.I1;
										stChargeCfgData.I2 = wdTmp*stConfigData.I2;
										stChargeCfgData.I3 = wdTmp*stConfigData.I3;
										stChargeCfgData.i4 = wdTmp*stConfigData.i4;
										stChargeCfgData.I5 = wdTmp*stConfigData.I5;
										stChargeCfgData.I6 = wdTmp*stConfigData.I6;

										wdTmp = byVoltType + 1;
										stChargeCfgData.u0 = wdTmp*stConfigData.u0;
										stChargeCfgData.u1 = wdTmp*stConfigData.u1;
										stChargeCfgData.Ucv = wdTmp*stConfigData.Ucv;
										stChargeCfgData.U5 = wdTmp*stConfigData.U5;
										stChargeCfgData.U6 = wdTmp*stConfigData.U6;

										stChargeCfgData.To = stConfigData.To;
										stChargeCfgData.T1 = stConfigData.T1;
										stChargeCfgData.T2 = stConfigData.T2;
										stChargeCfgData.T3 = stConfigData.T3;
										stChargeCfgData.T4 = stConfigData.T4;
										stChargeCfgData.T5 = stConfigData.T5;
										stChargeCfgData.T6 = stConfigData.T6;

										mem_cpy(stChargeCfgData.acName, stConfigData.acName, CONFIG_NAME_LEN);

										if (MID_CFG_SEL == byMID)
										{
											wdML2CfgOffsetMax = CONFIG_DATA_NUM;
											wdML2CfgOffset = wdCurCfgDataIndex;
										}
	
										
									}
								}
								
							}
						}
						
						break;
					case WRITE_CFG:
						pSendHeader->Type = WRITE_CFG;
						pSendHeader->Len = 2;
						pSendData[0] = pRcvData[0];
						if (CTRL_STATE_CHARGE == byState)
						{
							pSendData[1] = 0x82;
						}
						else
						{
							if ((pRcvData[0] < 1) ||  (pRcvData[0] > CONFIG_DATA_NUM))
							{
								pSendData[1] = 0x81;
							}
							else
							{
								wdTmp = pRcvData[0] - 1;
								
								if (DBSaveCfg(wdTmp,(ConfigData_t*)(pRcvData+1)))
								{
									pSendData[1] = 0x81;
								}
								else
								{
									pSendData[1] = 0;
									
								}
								
							}
						}
						break;
					case READ_CFG:
						pSendHeader->Type = READ_CFG;
						pSendHeader->Len = 50;
						pSendData[0] = pRcvData[0];
						if ((pRcvData[0] < 1) ||  (pRcvData[0] > CONFIG_DATA_NUM))
						{
							pSendData[1] = 0x81;
						}
						else
						{
							wdTmp = pRcvData[0] - 1;
							
							if (DBLoadCfg(wdTmp,(ConfigData_t* )(pSendData+2)))
							{
								pSendData[1] = 0x81;
							}
							else
							{
								pSendData[1] = 0;
								
							}
							
						}
						break;
					case READ_CHG_FILE_LEN:
						pSendHeader->Type = READ_CHG_FILE_LEN;
						pSendHeader->Len = 7;
						
						wdTmp = *((U16 *)pRcvData);
						*((U16 *)pSendData) = wdTmp;
							
						ret = DBGetUIFileSize(wdTmp,(U32 *)(pSendData + 3),NULL);
						if (DB_ERR_NO_UIT_DATA_FILE == ret)
						{
							pSendData[2] = 0x81;
						}
						else if (ret)
						{
							pSendData[2] = 0x82;
						}
						else
						{
							pSendData[2] = 0;
						}
						
						
						break;
					case READ_CHG_FILE:
						pSendHeader->Type = READ_CHG_FILE;
						wdTmp = *((U16 *)pRcvData);
						dwdOffset = *((U32 *)(pRcvData + 2));
						byLen = pRcvData[6];
						
						wdTmp = *((U16 *)pRcvData);
						*((U16 *)pSendData) = wdTmp;
						*((U32 *)(pSendData + 2)) = dwdOffset;

						if (byLen > 46) /* 除去各方面开销,读充电数据文件时每次最多可读取46字节, 否则会超过64字节的最大包长 */
						{
							byLen = 46;
						}
						
						ret = DBReadUIFile(wdTmp, pSendData + 8, dwdOffset,byLen,&dwdReadLen);
						if (DB_ERR_NO_UIT_DATA_FILE == ret)
						{
							pSendData[6] = 0x81;
							pSendHeader->Len = 8;
							pSendData[7] = 0;
						}
						else if (DB_ERR_READ_UIT_DATA_FILE_OUT_OF_RANGE == ret)
						{
							pSendData[6] = 0x83;
							pSendHeader->Len = 8;
							pSendData[7] = 0;
						}
						else if (ret)
						{
							pSendData[6] = 0x82;
							pSendHeader->Len = 8;
							pSendData[7] = 0;
						}
						else
						{
							pSendData[6] = 0;
							pSendHeader->Len = 8 + (U8)dwdReadLen;
							pSendData[7] = (U8)dwdReadLen;
						}
						
						break;
				}
			}
			
			pSendHeader->Sync = SYNC_VAL;
			pSendHeader->NotType = ~pSendHeader->Type;
			pSendHeader->NotLen = ~pSendHeader->Len;

			if (pSendHeader->Len)
			{
				pwdChkSum = (U16*)(SendBuf + pSendHeader->Len + sizeof(ComHeader_t));
				*pwdChkSum = GetChkSum(SendBuf + sizeof(ComHeader_t), pSendHeader->Len);
			}

			bProc = TRUE;
		}

		if ((PrevXferComplete) && (CONFIGURED ==  bDeviceState))
		{
			/* Write the descriptor through the endpoint */
			USB_SIL_Write(EP1_IN, (uint8_t *)SendBuf, 64);
			SetEPTxValid(ENDP1);
			PrevXferComplete = 0;
			bRxdProc = FALSE;
			bProc = FALSE;
			byComWDGCnt = 0;
			bCom = TRUE;
		}		
	}
}


/* end of file */


