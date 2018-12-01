/******************************************************************************************************
*            SYS_StateMachine.c
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
*    FILE NAME: SYS_StateMachine.c
*
*    DESCRIPTION: System state-machine module
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/4/22 9:55:00
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version
******************************************************************************************************/


/**********************************************************************************************
* Includes
**********************************************************************************************/
/* Standard lib include */
#include <string.h>

/* Chip lib include */
/* user include */
#include "SYS_StateMachine.h"
#include "SM_Config.h"
#include "SM_CommonApi.h"
#include "db.h"
#include "METER_AdcSample.h"

/********************************************************************************
* Global DECLARATIONS
********************************************************************************/
void SYS_StateMachine(void);

/********************************************************************************
* Global VARIABLES
********************************************************************************/
SystemStateInfo_t SystemStateInfo;
ChargeCtrl_t stChargeCtrl;

/********************************************************************************
* LOCAL FUNCTION PROTOTYPES
********************************************************************************/
static void f_EnterSleepMode(void);
static void f_ExitFromSleepMode(void);
static void f_TransferSystemState(enSystemState newState);
static void f_ClearTempVarsAndInfo(void);
static void CHG_StateMachine(uint16_t callPeriod_ms);
static void f_TransferChgState(enChargePhase_t newState);



/********************************************************************************
* LOCAL VARIABLES
********************************************************************************/

/* When system state machine enter charging state, then call this sub-charge state machine */
static void CHG_StateMachine(uint16_t callPeriod_ms)
{
    uint16_t u16TempVal = 0;
    uint32_t u32TempVal = 0;
    uint32_t u32UTerminal = 0;
    uint32_t u32ITerminal = 0;

    stChargeCtrl.RelayOnCmd = 1;
    stChargeCtrl.FanOnCmd = 0;
    stChargeCtrl.PowerOnCmd = 1;

    switch(SystemStateInfo.CurrentChgState)
    {
        /* only arrive here when power on or software reset */
        case CHG_PRECHARGE:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_PRECHARGE\r\n");
            }
            SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;

            stChargeCtrl.PWM_U_DutyX1000 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I1 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];

            /* Delay several time, make sure overshoot disappear */
            if (SystemStateInfo.ChgTimerCnt_ms > T_DELAY_DETECTION_MS)
            {
                if (stAdcMeters.U_ChgOutput_mV >= (stChargeCfgData.u1 * 10))
                {
                    stChargeCtrl.U_FilterCnt++;
                }
                else
                {
                    stChargeCtrl.U_FilterCnt = 0;
                }

                /* goto next phase when timeout or voltage is reach exit value */
                if (   (stChargeCtrl.U_FilterCnt >= VOLT_FILT_TM)
                    || (SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T1)))
                {
                    f_TransferChgState(CHG_CC1);
                }
            }
            else if(SystemStateInfo.ChgTimerCnt_ms > 1000)
            {
                stChargeCtrl.MosOnCmd = 1;
            }
            break;
        }

        case CHG_CC1:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_CC1\r\n");
            }

            /* Generate PWM and other command */
            stChargeCtrl.FanOnCmd = 1;
            stChargeCtrl.PWM_U_DutyX1000 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I2 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];

            /* when AC not good or temperature high, then I down to 50%
               Also this condition don't count to charger timer */
            if (stSystemStateBits.AcLow || stSystemStateBits.BattOT)
            {
                stChargeCtrl.PWM_I_DutyX1000 = stChargeCtrl.PWM_I_DutyX1000 >> 1;
                stChargeCtrl.StopTimerAcc = TRUE;
            }
            else
            {
                stChargeCtrl.StopTimerAcc = FALSE;
            }

            if(!stChargeCtrl.StopTimerAcc)
            {
                SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;
            }

            if (SystemStateInfo.ChgTimerCnt_ms > T_DELAY_DETECTION_MS)
            {
                /* 0.96 * V-Target */
                u32UTerminal = stChargeCfgData.Ucv;
                u32UTerminal *= 96;
                u32UTerminal /= 100;

                /* 0.85 I Target */
                u32ITerminal = stChargeCfgData.I2;
                u32ITerminal *= 85;
                u32ITerminal /= 100;

                if (stAdcMeters.U_ChgOutput_mV >= (u32UTerminal * 10))
                {
                    if(++stChargeCtrl.U_FilterCnt >= VOLT_FILT_TM)
                    {
                        stChargeCtrl.U_FilterCnt = VOLT_FILT_TM;
                    }
                }
                else
                {
                    stChargeCtrl.U_FilterCnt = 0;
                }

                if (stAdcMeters.I_Charge_mA <= (u32ITerminal * 10))
                {
                     if(++stChargeCtrl.I_FilterCnt >= CURR_FILT_TM)
                    {
                        stChargeCtrl.I_FilterCnt = CURR_FILT_TM;
                    }
                }
                else
                {
                    stChargeCtrl.I_FilterCnt = 0;
                }


                if ((  (stChargeCtrl.U_FilterCnt >= VOLT_FILT_TM)
                    && (stChargeCtrl.I_FilterCnt >= CURR_FILT_TM) )
                 || (SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T2)))
                {
                    SystemStateInfo.ChgCcCvCnt_ms = SystemStateInfo.ChgTimerCnt_ms;
                    f_TransferChgState(CHG_CC2);
                }
            }

            break;
        }

        case CHG_CC2:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_CC2\r\n");
            }
            /* Generate PWM and other command */
            stChargeCtrl.FanOnCmd = 1;
            stChargeCtrl.PWM_U_DutyX1000 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];

            /* I3 is zero for some configuration */
            if(stChargeCfgData.I3)
            {
                stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I3 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];
            }
            else
            {
                stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I2 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];
            }


            /* when AC not good or temperature high, then I down to 50%
               Also this condition don't count to charger timer */
            if (stSystemStateBits.AcLow || stSystemStateBits.BattOT)
            {
                stChargeCtrl.PWM_I_DutyX1000 = stChargeCtrl.PWM_I_DutyX1000 >> 1;
                stChargeCtrl.StopTimerAcc = TRUE;
            }
            else
            {
                stChargeCtrl.StopTimerAcc = FALSE;
            }

            if(!stChargeCtrl.StopTimerAcc)
            {
                SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;
            }

            if (SystemStateInfo.ChgTimerCnt_ms > T_DELAY_DETECTION_MS)
            {
                /* 0.96 * V-Target */
                u32UTerminal = stChargeCfgData.Ucv;
                u32UTerminal *= 96;
                u32UTerminal /= 100;

                /* 0.85 I Target */
                u32ITerminal = (stChargeCfgData.I3 > 0)? stChargeCfgData.I3 : stChargeCfgData.I2;
                u32ITerminal *= 85;
                u32ITerminal /= 100;


                if (stAdcMeters.I_Charge_mA <= (u32ITerminal * 10))
                {
                     if(++stChargeCtrl.I_FilterCnt >= CURR_FILT_TM)
                     {
                        stChargeCtrl.I_FilterCnt = CURR_FILT_TM;
                     }
                }
                else
                {
                    stChargeCtrl.I_FilterCnt = 0;
                }


                if ((stChargeCtrl.I_FilterCnt >= CURR_FILT_TM)
                 || (SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T3)))
                {
                    SystemStateInfo.ChgCcCvCnt_ms += SystemStateInfo.ChgTimerCnt_ms;
                    f_TransferChgState(CHG_CV);
                }
            }
            break;
        }

        /* we are using stop mode, so RAM value can keep */
        /* sleep min time is 25s, because we need to wakeup to feed dog */
        case CHG_CV:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_CV\r\n");
            }

            /* Generate PWM and other command */
            stChargeCtrl.PWM_U_DutyX1000 = 750ul * (uint32_t)stChargeCfgData.U5 / (uint32_t)awdVoltTypeValue[stProductInfo.U_Type];

            /* I3 is zero for some configuration */
            if(stChargeCfgData.I3)
            {
                stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I3 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];
            }
            else
            {
                stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I2 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];
            }


            /* when AC not good or temperature high, then I down to 50%
               Also this condition don't count to charger timer */
            if (stSystemStateBits.AcLow || stSystemStateBits.BattOT)
            {
                stChargeCtrl.PWM_I_DutyX1000 = stChargeCtrl.PWM_I_DutyX1000 >> 1;
                stChargeCtrl.StopTimerAcc = TRUE;
            }
            else
            {
                stChargeCtrl.StopTimerAcc = FALSE;
            }

            /* TODO: double check this condition */
            if (stAdcMeters.I_Charge_mA >= (3 * awdCurTypeValue[stProductInfo.I_Type]))
            {
                stChargeCtrl.FanOnCmd = 1;
            }

            if(!stChargeCtrl.StopTimerAcc)
            {
                SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;
            }

            if (SystemStateInfo.ChgTimerCnt_ms > T_DELAY_DETECTION_MS)
            {
                /* 0.96 * V-Target */
                u32UTerminal = stChargeCfgData.Ucv;
                u32UTerminal *= 96;
                u32UTerminal /= 100;

                /* 0.85 I Target */
                u32ITerminal = stChargeCfgData.i4;
                u32ITerminal *= 85;
                u32ITerminal /= 100;


                if (stAdcMeters.I_Charge_mA <= (u32ITerminal * 10))
                {
                     if(++stChargeCtrl.I_FilterCnt >= CURR_FILT_TM)
                     {
                        stChargeCtrl.I_FilterCnt = CURR_FILT_TM;
                     }
                }
                else
                {
                    stChargeCtrl.I_FilterCnt = 0;
                }


                if ((stChargeCtrl.I_FilterCnt >= CURR_FILT_TM)
                 || (SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T4))
                 || ((SystemStateInfo.ChgCcCvCnt_ms + SystemStateInfo.ChgTimerCnt_ms) >= CoverterMinToMs(stChargeCfgData.To)))
                {
                    SystemStateInfo.ChgCcCvCnt_ms += SystemStateInfo.ChgTimerCnt_ms;
                    f_TransferChgState(CHG_PULL);
                }
            }
            break;
        }

        /* when in this state, means that we should not turn on charger again */
        case CHG_PULL:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_PULL\r\n");
            }
            SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;
            /* Generate PWM and other command */

            stChargeCtrl.PWM_U_DutyX1000 = 750ul * (uint32_t)stChargeCfgData.U5 / (uint32_t)awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I5 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];

            if (stChargeCfgData.T5)
            {
                if (SystemStateInfo.ChgCcCvCnt_ms  <= CoverterMinToMs(60))
                {
                    u32TempVal = CoverterMinToMs(60);
                }
                else if (SystemStateInfo.ChgCcCvCnt_ms <= CoverterMinToMs(stChargeCfgData.T5))
                {
                    u32TempVal = SystemStateInfo.ChgCcCvCnt_ms;
                }
                else
                {
                    u32TempVal = CoverterMinToMs(stChargeCfgData.T5);
                }
            }
            else
            {
                u32TempVal = 0;
            }

            if ((stChargeCtrl.I_FilterCnt >= CURR_FILT_TM)
             || (SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T4))
             || ((SystemStateInfo.ChgCcCvCnt_ms + SystemStateInfo.ChgTimerCnt_ms) >= CoverterMinToMs(stChargeCfgData.To)))
            {
                f_TransferChgState(CHG_FLOAT);
            }

            break;
        }

        case CHG_FLOAT:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_FLOAT\r\n");
            }
            SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;

            /* Generate PWM and other command */
            stChargeCtrl.FanOnCmd = 0;
            stChargeCtrl.PWM_U_DutyX1000 = 750ul * (uint32_t)stChargeCfgData.U6 / (uint32_t)awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I6 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];

            if (stChargeCfgData.T6 != 0xFFFF)
            {
                if(SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T6))
                {
                    stChargeCtrl.PowerOnCmd = 0;
                    SystemStateInfo.ChgPhaseFinish = 1;
                }
            }
            break;
        }

        default:
            f_TransferChgState(CHG_PRECHARGE);
            break;
    }
}
/****************************************************************************
*
*  Function: SYS_StateMachine
*
*  Purpose :    Run ADC slow task, mainly for slow ADC channel, such as temperature
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Suggest to run every 100ms
*
****************************************************************************/
void SYS_RunStateMachine(uint16_t callPeriod_ms)
{
    static uint16_t RunCounter = 0;

    RunCounter += callPeriod_ms;

    if(RunCounter >= 1000)
    {
        RunCounter = 0;
        SystemStateInfo.TimeSincePowerOn_s++;
    }
    
    stChargeCtrl.RelayOnCmd = 0;
    stChargeCtrl.FanOnCmd = 0;
    stChargeCtrl.PowerOnCmd = 0;
    stChargeCtrl.MosOnCmd = 0;
    
    switch(SystemStateInfo.CurrentState)
    {
        /* only arrive here when power on or software reset */
        case SYS_INITIAL:
        {
            if(SystemStateInfo.TimerCnt_ms == 0)
            {
                SM_PrintfString("SYS_INITIAL\r\n");
                SYS_Initial();
                //PowerOnProc();
            }

            SystemStateInfo.TimerCnt_ms += callPeriod_ms;

            if(SystemStateInfo.TimerCnt_ms > 2000)
            {
                f_TransferSystemState(SYS_SELF_TEST);
            }
        }
        break;

        /* system state doesn't command to discharge or charge state, just
           keep system alive */
        case SYS_IDLE:
        {
            if(SystemStateInfo.TimerCnt_ms == 0)
            {
                SM_PrintfString("SYS_IDLE\r\n");
            }
            SystemStateInfo.TimerCnt_ms += callPeriod_ms;
            
            /* FCT test has highest priority */
            if(stSystemStateBits.EnterTest)
            {
                f_TransferSystemState(SYS_FCT);
            }
            else if((SystemStateInfo.ChgPhaseFinish == 0) && stSystemStateBits.AllowEnterChg)
            {
                f_TransferSystemState(SYS_CHARGING);
            }
            break;
        }
        
        case SYS_SELF_TEST:
        {
            if(SystemStateInfo.TimerCnt_ms == 0)
            {
                SM_PrintfString("SYS_SELF_TEST\r\n");
            }
            
            SystemStateInfo.TimerCnt_ms += callPeriod_ms;
            /* TODO: double check whether we neet this */
            f_TransferSystemState(SYS_IDLE);
            break;
        }
        
        case SYS_CHARGING:
        {
            if(SystemStateInfo.TimerCnt_ms == 0)
            {
                SM_PrintfString("SYS_CHARGING\r\n");
            }
            SystemStateInfo.TimerCnt_ms += callPeriod_ms;
            CHG_StateMachine(callPeriod_ms);
            
            if(SystemStateInfo.ChgPhaseFinish == 1)
            {
                f_TransferSystemState(SYS_IDLE);
            }
            break;
        }

        /* Only enter this when factory functional check test, so don't need to switch out */
        case SYS_FCT:
        {
            if(SystemStateInfo.TimerCnt_ms == 0)
            {
                SM_PrintfString("SYS_FCT\r\n");
            }

            SystemStateInfo.TimerCnt_ms += callPeriod_ms;
            
            stChargeCtrl.PWM_U_DutyX1000 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrl.PWM_I_DutyX1000 = (uint32_t)stChargeCfgData.I2 * (uint32_t)1000 / (uint32_t)awdCurTypeValue[stProductInfo.I_Type];
            
            if(SystemStateInfo.TimerCnt_ms >= 3000)
            {
                stChargeCtrl.RelayOnCmd = 1;
                stChargeCtrl.FanOnCmd = 1;
                stChargeCtrl.PowerOnCmd = 1;
                stChargeCtrl.MosOnCmd = 1;
            }
            else if(SystemStateInfo.TimerCnt_ms >= 1000)
            {
                stChargeCtrl.RelayOnCmd = 1;
                stChargeCtrl.FanOnCmd = 0;
                stChargeCtrl.PowerOnCmd = 0;
                stChargeCtrl.MosOnCmd = 0;
            }
            break;
        }

        default:
            break;
    }
}


void SYS_Initial(void)
{
    /* 1. make all control signal in-active */
    /* navigate to different state based on wakeup source */
    memset(&SystemStateInfo, 0u, sizeof(SystemStateInfo_t));
    memset(&stStatusCounter, 0u, sizeof(stStatusCounter_t));
}



static void f_TransferSystemState(enSystemState newState)
{
    SystemStateInfo.PreState = SystemStateInfo.CurrentState;
    SystemStateInfo.CurrentState = newState;

    SystemStateInfo.TimerCnt_ms = 0;

    //SystemStateInfo.ChgPhaseFinish = 0;

    f_TransferSystemState(CHG_PRECHARGE);
}

static void f_TransferChgState(enChargePhase_t newState)
{
    SystemStateInfo.PreChgState = SystemStateInfo.CurrentChgState;

    SystemStateInfo.ChgTimerCnt_ms = 0;

    stChargeCtrl.StopTimerAcc = FALSE;

    stChargeCtrl.U_FilterCnt = 0;
    stChargeCtrl.I_FilterCnt = 0;
}


static void f_EnterSleepMode(void)
{

}


static void f_ExitFromSleepMode(void)
{

}

static void f_ClearTempVarsAndInfo(void)
{

}

enSystemState SYS_GetSystemstate(void)
{
    return SystemStateInfo.CurrentState;
}