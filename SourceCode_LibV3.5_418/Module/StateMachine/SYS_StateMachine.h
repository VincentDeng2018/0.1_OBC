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
    CHG_PRECHARGE = 0x0,
    CHG_CC1,
    CHG_CC2,
    CHG_CV,
    CHG_PULL,
    CHG_FLOAT,
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
    
}SystemStateInfo_t;

extern SystemStateInfo_t SystemStateInfo;

typedef struct
{
    uint16_t PWM_U_DutyX1000; /* e.g.-->20% duty then this value is 20 */
    uint16_t PWM_I_DutyX1000;
    int16_t  U_FilterCnt;
    int16_t  I_FilterCnt;
    
    uint16_t PowerOnCmd: 1;
    uint16_t MosOnCmd:   1;
    uint16_t RelayOnCmd: 1;
    uint16_t FanOnCmd: 1;

    uint16_t StopTimerAcc: 1;
    uint16_t Reserved: 11;
}ChargeCtrl_t;

extern ChargeCtrl_t stChargeCtrl;

/*********************************************************************************
* Global DECLARATIONS
*********************************************************************************/
extern void SYS_RunStateMachine(uint16_t callPeriod_ms);
extern void SYS_Initial(void);

#endif