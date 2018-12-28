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
#include <stdio.h>

/* Chip lib include */
/* user include */
#include "SYS_StateMachine.h"
#include "SM_Config.h"
#include "SM_CommonApi.h"
#include "db.h"
#include "METER_AdcSample.h"
#include "RTC_Calendar.h"

/********************************************************************************
* Global DECLARATIONS
********************************************************************************/
void SYS_StateMachine(void);

/********************************************************************************
* Global VARIABLES
********************************************************************************/
SystemStateInfo_t SystemStateInfo;
ChargeCtrl_t stChargeCtrlNow;
ChargeCtrl_t stChargeCtrlPre;

/********************************************************************************
* LOCAL FUNCTION PROTOTYPES
********************************************************************************/
//static void f_EnterSleepMode(void);
//static void f_ExitFromSleepMode(void);
static void f_TransferSystemState(enSystemState newState);
//static void f_ClearTempVarsAndInfo(void);
static void CHG_StateMachine(uint16_t callPeriod_ms);
static void f_TransferChgState(enChargePhase_t newState);
static uint16_t CalcIPwm_x10(uint16_t I_InternalCfg);


/********************************************************************************
* LOCAL VARIABLES
********************************************************************************/

/* When system state machine enter charging state, then call this sub-charge state machine */
static void CHG_StateMachine(uint16_t callPeriod_ms)
{
    uint32_t u32TempVal = 0;
    uint32_t u32UTerminal = 0;
    uint32_t u32ITerminal = 0;
    RTC_stTime RtcTime = {0};

    stChargeCtrlNow.RelayOnCmd = 1;
    stChargeCtrlNow.FanOnCmd = 0;
    stChargeCtrlNow.PowerOnCmd = 1;
    stChargeCtrlNow.MosOnCmd = 1;

    RTC_GetTime(&RtcTime);
    
    switch(SystemStateInfo.CurrentChgState)
    {
        /* only arrive here when power on or software reset */
        case CHG_PRECHARGE:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_PRECHARGE\r\n");
                printf("Time: %d:%d:%d\r\n", RtcTime.u8hour, RtcTime.u8minute, RtcTime.u8second);
            }
            SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;

            stChargeCtrlNow.PWM_U_Duty_x10 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];
            
            stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I1);

            /* Delay several time, make sure overshoot disappear */
            if (SystemStateInfo.ChgTimerCnt_ms > T_DELAY_DETECTION_MS)
            {
                if (stAdcMeters.U_Pack_mV >= (stChargeCfgData.u1 * 10))
                {
                    stChargeCtrlNow.U_FilterCnt++;
                }
                else
                {
                    stChargeCtrlNow.U_FilterCnt = 0;
                }

                /* goto next phase when timeout or voltage is reach exit value */
                if (   (stChargeCtrlNow.U_FilterCnt >= VOLT_FILT_TM)
                    || (SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T1)))
                {
                    f_TransferChgState(CHG_CC1);
                }
            }
            else if(SystemStateInfo.ChgTimerCnt_ms > 1000)
            {
                stChargeCtrlNow.MosOnCmd = 1;
            }
            break;
        }

        case CHG_CC1:
        {
            if(SystemStateInfo.ChgTimerCnt_ms == 0)
            {
                SM_PrintfString("CHG_CC1\r\n");
                printf("Time: %d:%d:%d\r\n", RtcTime.u8hour, RtcTime.u8minute, RtcTime.u8second);
            }

            /* Generate PWM and other command */
            stChargeCtrlNow.FanOnCmd = 1;
            stChargeCtrlNow.PWM_U_Duty_x10 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I2);

            /* when AC not good or temperature high, then I down to 50%
               Also this condition don't count to charger timer */
            if (stSystemStateBits.AcLow || stSystemStateBits.BattOT)
            {
                stChargeCtrlNow.PWM_I_Duty_x10 = stChargeCtrlNow.PWM_I_Duty_x10 >> 1;
                stChargeCtrlNow.StopTimerAcc = TRUE;
            }
            else
            {
                stChargeCtrlNow.StopTimerAcc = FALSE;
            }

            if(!stChargeCtrlNow.StopTimerAcc)
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

                if (stAdcMeters.U_Pack_mV >= (u32UTerminal * 10))
                {
                    if(++stChargeCtrlNow.U_FilterCnt >= VOLT_FILT_TM)
                    {
                        stChargeCtrlNow.U_FilterCnt = VOLT_FILT_TM;
                    }
                }
                else
                {
                    stChargeCtrlNow.U_FilterCnt = 0;
                }

                if (stAdcMeters.I_Charge_mA <= (u32ITerminal * 10))
                {
                     if(++stChargeCtrlNow.I_FilterCnt >= CURR_FILT_TM)
                    {
                        stChargeCtrlNow.I_FilterCnt = CURR_FILT_TM;
                    }
                }
                else
                {
                    stChargeCtrlNow.I_FilterCnt = 0;
                }


                if ((  (stChargeCtrlNow.U_FilterCnt >= VOLT_FILT_TM)
                    && (stChargeCtrlNow.I_FilterCnt >= CURR_FILT_TM) )
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
                printf("Time: %d:%d:%d\r\n", RtcTime.u8hour, RtcTime.u8minute, RtcTime.u8second);
            }
            /* Generate PWM and other command */
            stChargeCtrlNow.FanOnCmd = 1;
            stChargeCtrlNow.PWM_U_Duty_x10 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];

            /* I3 is zero for some configuration */
            if(stChargeCfgData.I3)
            {
                stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I3);
            }
            else
            {
                stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I2);
            }


            /* when AC not good or temperature high, then I down to 50%
               Also this condition don't count to charger timer */
            if (stSystemStateBits.AcLow || stSystemStateBits.BattOT)
            {
                stChargeCtrlNow.PWM_I_Duty_x10 = stChargeCtrlNow.PWM_I_Duty_x10 >> 1;
                stChargeCtrlNow.StopTimerAcc = TRUE;
            }
            else
            {
                stChargeCtrlNow.StopTimerAcc = FALSE;
            }

            if(!stChargeCtrlNow.StopTimerAcc)
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
                     if(++stChargeCtrlNow.I_FilterCnt >= CURR_FILT_TM)
                     {
                        stChargeCtrlNow.I_FilterCnt = CURR_FILT_TM;
                     }
                }
                else
                {
                    stChargeCtrlNow.I_FilterCnt = 0;
                }


                if ((stChargeCtrlNow.I_FilterCnt >= CURR_FILT_TM)
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
                printf("Time: %d:%d:%d\r\n", RtcTime.u8hour, RtcTime.u8minute, RtcTime.u8second);
            }

            /* Generate PWM and other command */
            stChargeCtrlNow.PWM_U_Duty_x10 = 750ul * (uint32_t)stChargeCfgData.Ucv / (uint32_t)awdVoltTypeValue[stProductInfo.U_Type];

            /* I3 is zero for some configuration */
            if(stChargeCfgData.I3)
            {
                stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I3);
            }
            else
            {
                stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I2);
            }


            /* when AC not good or temperature high, then I down to 50%
               Also this condition don't count to charger timer */
            if (stSystemStateBits.AcLow || stSystemStateBits.BattOT)
            {
                stChargeCtrlNow.PWM_I_Duty_x10 = stChargeCtrlNow.PWM_I_Duty_x10 >> 1;
                stChargeCtrlNow.StopTimerAcc = TRUE;
            }
            else
            {
                stChargeCtrlNow.StopTimerAcc = FALSE;
            }

            /* TODO: double check this condition */
            if (stAdcMeters.I_Charge_mA >= (3 * awdCurTypeValue[stProductInfo.I_Type]))
            {
                stChargeCtrlNow.FanOnCmd = 1;
            }

            if(!stChargeCtrlNow.StopTimerAcc)
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
                     if(++stChargeCtrlNow.I_FilterCnt >= CURR_FILT_TM)
                     {
                        stChargeCtrlNow.I_FilterCnt = CURR_FILT_TM;
                     }
                }
                else
                {
                    stChargeCtrlNow.I_FilterCnt = 0;
                }


                if ((stChargeCtrlNow.I_FilterCnt >= CURR_FILT_TM)
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
                printf("Time: %d:%d:%d\r\n", RtcTime.u8hour, RtcTime.u8minute, RtcTime.u8second);
            }
            SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;
            /* Generate PWM and other command */

            stChargeCtrlNow.PWM_U_Duty_x10 = 750ul * (uint32_t)stChargeCfgData.U5 / (uint32_t)awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I5);

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

            if ((stChargeCtrlNow.I_FilterCnt >= CURR_FILT_TM)
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
                printf("Time: %d:%d:%d\r\n", RtcTime.u8hour, RtcTime.u8minute, RtcTime.u8second);
            }
            SystemStateInfo.ChgTimerCnt_ms += callPeriod_ms;

            /* Generate PWM and other command */
            stChargeCtrlNow.FanOnCmd = 0;
            stChargeCtrlNow.PWM_U_Duty_x10 = 750ul * (uint32_t)stChargeCfgData.U6 / (uint32_t)awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I6);

            if (stChargeCfgData.T6 != 0xFFFF)
            {
                if(SystemStateInfo.ChgTimerCnt_ms >= CoverterMinToMs(stChargeCfgData.T6))
                {
                    stChargeCtrlNow.PowerOnCmd = 0;
                    SystemStateInfo.ChgPhaseFinish = 1;
                }
            }
            break;
        }

        default:
            f_TransferChgState(CHG_PRECHARGE);
            break;
    }
    
    SystemStateInfo.ChargeCapTemp += stAdcMeters.I_Charge_mA;
    u32TempVal = SystemStateInfo.ChargeCapTemp * (uint32_t)callPeriod_ms / (uint32_t)3600000;
    if(u32TempVal >= 1)
    {
        SystemStateInfo.ChargeCap_mAh++;
        SystemStateInfo.ChargeCapTemp = SystemStateInfo.ChargeCapTemp * (uint32_t)callPeriod_ms - (uint32_t)3600000;
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

    stChargeCtrlNow.RelayOnCmd = 0;
    stChargeCtrlNow.FanOnCmd = 0;
    stChargeCtrlNow.PowerOnCmd = 0;
    stChargeCtrlNow.MosOnCmd = 0;

    switch(SystemStateInfo.CurrentState)
    {
        /* only arrive here when power on or software reset */
        case SYS_INITIAL:
        {
            if(SystemStateInfo.TimerCnt_ms == 0)
            {
                SM_PrintfString("SYS_INITIAL\r\n");
                SYS_Initial();
                stSystemStateBits.ForceStayInInit = 0;
                stSystemStateBits.EnterFCT = 0;
                //PowerOnProc();
            }
            ProductTypeDetection();

            SystemStateInfo.TimerCnt_ms += callPeriod_ms;
            //stSystemStateBits.ForceStayInInit = 1;

            if(SystemStateInfo.TimerCnt_ms > 5000 && !stSystemStateBits.ForceStayInInit)
            {
              ADC_EnableAutoZero(0);
              f_TransferSystemState(SYS_SELF_TEST);
            }
            else
            {
                ADC_EnableAutoZero(1);
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

            if(stSystemStateBits.EnterFCT)
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

            if(SystemStateInfo.ChgPhaseFinish ||
               !stSystemStateBits.AllowStayChg)
            {
                f_TransferSystemState(SYS_IDLE);
            }

            SystemStateInfo.TimerCnt_ms += callPeriod_ms;
            CHG_StateMachine(callPeriod_ms);

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

            stChargeCtrlNow.PWM_U_Duty_x10 = 750ul * stChargeCfgData.Ucv / awdVoltTypeValue[stProductInfo.U_Type];
            stChargeCtrlNow.PWM_I_Duty_x10 = CalcIPwm_x10(stChargeCfgData.I2);

            stChargeCtrlNow.RelayOnCmd = 1;
            stChargeCtrlNow.FanOnCmd = 1;
            stChargeCtrlNow.PowerOnCmd = 1;
            stChargeCtrlNow.MosOnCmd = 1;

            break;
        }

        default:
            break;
    }
}


void SYS_Initial(void)
{
    uint16_t wdTmp = 0;

    /* 1. make all control signal in-active */
    /* navigate to different state based on wakeup source */
    memset(&SystemStateInfo, 0u, sizeof(SystemStateInfo_t));
    memset(&stStatusCounter, 0u, sizeof(stStatusCounter_t));
    memcpy(&stChargeCfgData, &castChgCfgData[0], sizeof(ChargeCfgData_t));

    wdTmp = stProductInfo.I_Type + 1;

    stChargeCfgData.I1 = wdTmp*castChgCfgData[0].I1;
    stChargeCfgData.I2 = wdTmp*castChgCfgData[0].I2;
    stChargeCfgData.I3 = wdTmp*castChgCfgData[0].I3;
    stChargeCfgData.i4 = wdTmp*castChgCfgData[0].i4;
    stChargeCfgData.I5 = wdTmp*castChgCfgData[0].I5;
    stChargeCfgData.I6 = wdTmp*castChgCfgData[0].I6;

    wdTmp = stProductInfo.U_Type + 1;
    stChargeCfgData.u0 = wdTmp*castChgCfgData[0].u0;
    stChargeCfgData.u1 = wdTmp*castChgCfgData[0].u1;
    stChargeCfgData.Ucv = wdTmp*castChgCfgData[0].Ucv;
    stChargeCfgData.U5 = wdTmp*castChgCfgData[0].U5;
    stChargeCfgData.U6 = wdTmp*castChgCfgData[0].U6;
}



static void f_TransferSystemState(enSystemState newState)
{
    SystemStateInfo.PreState = SystemStateInfo.CurrentState;
    SystemStateInfo.CurrentState = newState;

    SystemStateInfo.TimerCnt_ms = 0;

    //SystemStateInfo.ChgPhaseFinish = 0;

    f_TransferChgState(CHG_PRECHARGE);
}

static void f_TransferChgState(enChargePhase_t newState)
{
    SystemStateInfo.PreChgState = SystemStateInfo.CurrentChgState;
    SystemStateInfo.CurrentChgState = newState;
    SystemStateInfo.ChgTimerCnt_ms = 0;

    stChargeCtrlNow.StopTimerAcc = FALSE;

    stChargeCtrlNow.U_FilterCnt = 0;
    stChargeCtrlNow.I_FilterCnt = 0;
}


enSystemState SYS_GetSystemstate(void)
{
    return SystemStateInfo.CurrentState;
}



static uint16_t CalcIPwm_x10(uint16_t I_InternalCfg)
{
    uint16_t u16tempIVal_10mA = GetChargeCmd_10mA();
    uint16_t u16result = 0u;
    
    /* select min one as current target */
    if(stSystemStateBits.BattCanActive && (u16tempIVal_10mA < I_InternalCfg))
    {
        u16result = (uint16_t)((uint32_t)u16tempIVal_10mA * (uint32_t)1000u 
                        / (uint32_t)awdCurTypeValue[stProductInfo.I_Type]);
    }
    else
    {
        u16result =  (uint16_t)((uint32_t)I_InternalCfg * (uint32_t)1000u 
                        / (uint32_t)awdCurTypeValue[stProductInfo.I_Type]);
    }
    
    return u16result;
}