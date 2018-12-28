/******************************************************************************************************
*            SYS_StateMachine.h
******************************************************************************************************
******************************************************************************************************
*
* THIS INFORMATION IS PROPRIETARY TO BYD Corporation
*
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: SYS_StateMachine.h
*
*    DESCRIPTION: Header file for system State-Machine module
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/4/22 15:25:14
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version
******************************************************************************************************/


/**********************************************************************************************
* Includes
**********************************************************************************************/
#ifndef _SYS_STATEMACHINE_H
#define _SYS_STATEMACHINE_H


/* Chip lib include */
/* user include */
#include <stdint.h>

/*********************************************************************************
* Macros
*********************************************************************************/


/*********************************************************************************
* Data and Structure
*********************************************************************************/
typedef enum
{
    SYS_INITIAL = 0x0,  /* 0 */
    SYS_IDLE,          /* 1 */
    SYS_CHARGING,       /* 2 */
    SYS_SELF_TEST,    /* 3 */
    SYS_FCT    /* 4 */
}enSystemState;


typedef enum
{
    CHG_PRECHARGE = 0x0,  // 0
    CHG_CC1,              // 1
    CHG_CC2,              // 2
    CHG_CV,               // 3
    CHG_PULL,             // 4
    CHG_FLOAT,            // 5
}enChargePhase_t;

typedef struct
{
    /* system level state machine */
    enSystemState CurrentState;
    enSystemState PreState;
    uint32_t TimeSincePowerOn_s;
    uint32_t QuitChgReason;
    uint32_t TimerCnt_ms;
    
    /* charger state machine */
    enChargePhase_t CurrentChgState;
    enChargePhase_t PreChgState;
    uint32_t ChgTimerCnt_ms;
    uint32_t ChgCcCvCnt_ms;
    uint8_t  ChgToIdleCnt;
    uint8_t  ChgPhaseFinish;
    uint32_t ChargeCap_mAh;
    uint32_t ChargeCapTemp;
    uint8_t  ChargeSOC; /* calc by charger */
    
}SystemStateInfo_t;

extern SystemStateInfo_t SystemStateInfo;

typedef struct
{
    uint16_t PWM_U_Duty_x10; /* e.g.-->20% duty then this value is 20 */
    uint16_t PWM_I_Duty_x10;
    int16_t  U_FilterCnt;
    int16_t  I_FilterCnt;
    
    uint16_t PowerOnCmd: 1;
    uint16_t MosOnCmd:   1;
    uint16_t RelayOnCmd: 1;
    uint16_t FanOnCmd: 1;

    uint16_t StopTimerAcc: 1;
    uint16_t PWM_I_RampDone:1;
    uint16_t PWM_U_RampDone:1;
    uint16_t Reserved: 9;
}ChargeCtrl_t;

extern ChargeCtrl_t stChargeCtrlNow;
extern ChargeCtrl_t stChargeCtrlPre;

/*********************************************************************************
* Global DECLARATIONS
*********************************************************************************/
extern void SYS_RunStateMachine(uint16_t callPeriod_ms);
extern void SYS_Initial(void);
extern enSystemState SYS_GetSystemstate(void);

#endif