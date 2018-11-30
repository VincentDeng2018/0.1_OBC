/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		lcd.h
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

#ifndef __CTRL_H
#define	__CTRL_H

#include "def.h"
#include "stm3210x_eval.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define SAVE_UIT_TM_SPACE		10		/* every 5 seconds */
#define VOLT_FILT_TM			6
#define CURR_FILT_TM			6
#define JUDGE_WAIT_TM			10     /* 5 seconds */
#define TEM1_HI			    80
#define TEM1_LO			    60


#define DEBUG_CHARGE			0

enum CtrlState_e
{
	CTRL_STATE_IDLE = 0,
	CTRL_STATE_STAND_BY,
	CTRL_STATE_SELF_TEST,
	CTRL_STATE_TEST,
	CTRL_STATE_CHARGE,
	CTRL_STATE_ERROR
};

enum ChgStep_e
{

	CHARGE_STEP_PREV = 0,
	CHARGE_STEP_CC1,
	CHARGE_STEP_CC2,
	CHARGE_STEP_CV,
	CHARGE_STEP_ABSO,
	CHARGE_STEP_FLOAT
};

 enum Error_e
{
	ERROR_NO_ERROR = 0,
	ERROR_SELF_TEST,
	ERROR_TEMP,
	ERROR_BT_LOW,
	ERROR_BT_REV,
	ERROR_CFG,
	ERROR_SAVE_DATA,
	ERROR_ERROR = 0xff
};
 
extern BOOL bChgDisable;
extern U8 byState;
extern BOOL byFlashFlag;
extern U8 byVoltType;
extern U8 byCurType;
extern U16 wdWaitPCTimeLen;  /* 开始运行后的30秒等待与PC连接，过时不侯 */
extern BOOL bConnectWithPC;
extern U32 dwdChargeBeginTm;
extern uint64_t dwdCapacity;   /* 容量 */
extern U16 wdBtVoltage;
extern U16 wdCurrent;
extern U8 byChgStep;
extern U8 byErr;
extern CHAR cTemp1;
extern CHAR cTemp2;
extern U8 byWaitTm;


extern const WORD testIData[1000];
extern const WORD testUData[1000];



void PowerOnProc(void);
void Ctrl_Init(void);
void KeyScan(void);
void KeyFilt(void);
void KeyProc(void);
void GetInputState(void);
void Ctrl_ADC(void);
void TimerRun(void);
void ResetTimer(void);
U32 GetTimer(void);
void LoadRefresh(void);
void LEDDisp(void);
void Ctrl(void);

#ifdef __cplusplus
}
#endif

#endif
/* end of file */

