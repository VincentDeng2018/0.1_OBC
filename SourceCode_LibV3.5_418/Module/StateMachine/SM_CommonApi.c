/******************************************************************************************************
*            SM_CommonApi.c
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
*    FILE NAME: SM_CommonApi.c
*
*    DESCRIPTION: Common API for all state-machines
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

/* Chip lib include */
/* user include */
#include "SM_Config.h"
#include "SM_CommonApi.h"
#include "SYS_StateMachine.h"
#include "METER_AdcSample.h"
#include "GPIO_Globals.h"
#include "PWM_Globals.h"

/********************************************************************************
* Global DECLARATIONS
********************************************************************************/
void SM_SystemStateBitsUpdate(void);

/********************************************************************************
* Global VARIABLES
********************************************************************************/
stSystemStateBits_t stSystemStateBits = {0u};
stStatusCounter_t stStatusCounter = {0u};
stProductInfo_t stProductInfo;

const uint16_t awdVoltTypeValue[VOLTAGE_TYPE_NUM] = {1200, 2400, 3600, 4800};
const uint16_t awdCurTypeValue[CURRENT_TYPE_NUM] = {500, 1000, 1500, 2000,2500,3000};
/********************************************************************************
* LOCAL FUNCTION PROTOTYPES
********************************************************************************/
static void BattConnectMonitor(void);
static void BattChgVoltMonitor(void);
static void BattChgTempMonitor(void);
static void EnterTestMode(void);
void ProductTypeDetection(void);
static void RelayStateMonitor(void);
static void AcMonitor(void);

/********************************************************************************
* LOCAL VARIABLES
********************************************************************************/
    static uint16_t PWM_ICmdLast_x10 = 0;
    static uint16_t PWM_UCmdLast_x10 = 0;

void SM_SystemStateBitsUpdate(void)
{
    BattChgTempMonitor();
    BattChgVoltMonitor();
    EnterTestMode();
    AcMonitor();
    BattChgTempMonitor();
    ProductTypeDetection();
    BattConnectMonitor();
    RelayStateMonitor();

    /* some statebits are determined by several bits, update here */

    stSystemStateBits.AllowEnterChg = !stSystemStateBits.BattOV
                                      && !stSystemStateBits.BattAbsUV
                                      && !stSystemStateBits.ChgBoardOT
                                      && !stSystemStateBits.ChargeOC
                                      && !stSystemStateBits.AcLow
                                      && !stSystemStateBits.AcOff
                                      && stSystemStateBits.BattConnectOk
                                      && !stSystemStateBits.BattConnectReverse
                                      && !stSystemStateBits.EnterFCT;

    stSystemStateBits.AllowStayChg = !stSystemStateBits.BattOV
                                  && !stSystemStateBits.BattUV
                                  && !stSystemStateBits.ChargeOC
                                  && !stSystemStateBits.AcOff;
    
    stSystemStateBits.AllowEnterChg = 1;
    stSystemStateBits.AllowStayChg  = 1;

#ifdef DEBUG_MODE
    stSystemStateBits.AllowEnterChg = 1;
    stSystemStateBits.AllowStayChg  = 1;
    stSystemStateBits.EnterFCT = 0;
#endif

    SYS_RunStateMachine(500);

    printf("Systemstate: %x, %d, %d\r\n", *((uint32_t *)&stSystemStateBits), SystemStateInfo.CurrentState, SystemStateInfo.CurrentChgState);
    printf("Iduty->%d, Uduty->%d\r\n", PWM_ICmdLast_x10, PWM_UCmdLast_x10);
}


/* always detection it */
static void AcMonitor(void)
{
    /* AC low detection */
    if(AcLowActive())
    {
        stStatusCounter.AcLow_Cnt++;
    }
    else
    {
        stStatusCounter.AcLow_Cnt -= 2;
    }

    /* reach AC debounce counter? */
    if(stStatusCounter.AcLow_Cnt >= AC_LOW_ACTIVE_CNT)
    {
        stStatusCounter.AcLow_Cnt = AC_LOW_ACTIVE_CNT;
        stSystemStateBits.AcLow = 1;
    }
    else if(stStatusCounter.AcLow_Cnt <= 0)
    {
        stSystemStateBits.AcLow = 0;
        stStatusCounter.AcLow_Cnt = 0;
    }
    else
    {

    }

    /* AC low detection */
    if(AcOffActive())
    {
        stStatusCounter.AcOff_Cnt++;
    }
    else
    {
        stStatusCounter.AcOff_Cnt -= 2;
    }

    /* reach AC debounce counter? */
    if(stStatusCounter.AcOff_Cnt >= AC_LOW_ACTIVE_CNT)
    {
        stStatusCounter.AcOff_Cnt = AC_LOW_ACTIVE_CNT;
        stSystemStateBits.AcOff = 1;
    }
    else if(stStatusCounter.AcOff_Cnt <= 0)
    {
        stSystemStateBits.AcOff = 0;
        stStatusCounter.AcOff_Cnt = 0;
    }
}


static void BattConnectMonitor(void)
{
    /* Battery  connect detection */
    if(BatteryNormalConnected())
    {
        stStatusCounter.BattConnectOk_Cnt++;
    }
    else
    {
        stStatusCounter.BattConnectOk_Cnt -= 2;
    }

    /* reach Battery ok counter? */
    if(stStatusCounter.BattConnectOk_Cnt >= (int16_t)CONN_OK_ACTIVE_CNT)
    {
        stStatusCounter.BattConnectOk_Cnt = (int16_t)CONN_OK_ACTIVE_CNT;
        stSystemStateBits.BattConnectOk = 1;
    }
    else if(stStatusCounter.BattConnectOk_Cnt <= 0)
    {
        stStatusCounter.BattConnectOk_Cnt = 0;
        stSystemStateBits.BattConnectOk = 0;
    }
    else
    {
        /* keep original state */
    }

    /* Battery reverse connect detection */
    if(BatteryReverseConnected())
    {
        stStatusCounter.BattConnectReverse_Cnt++;
    }
    else
    {
        stStatusCounter.BattConnectReverse_Cnt -= 2;
    }

    /* reach Battery reverse counter? */
    if(stStatusCounter.BattConnectReverse_Cnt >= CONN_OK_ACTIVE_CNT)
    {
        stStatusCounter.BattConnectReverse_Cnt = CONN_OK_ACTIVE_CNT;
        stSystemStateBits.BattConnectReverse = 1;
    }
    else if(stStatusCounter.BattConnectReverse_Cnt <= 0)
    {
        stStatusCounter.BattConnectReverse_Cnt = 0;
        stSystemStateBits.BattConnectReverse = 0;
    }
    else
    {
        /* keep original state */
    }
}


static void BattChgVoltMonitor(void)
{
    if(stSystemStateBits.BattUV)
    {
        if(stAdcMeters.U_Batt_mV > U_BATT_UV_RESUME_MV)
        {
            stSystemStateBits.BattUV = 0;
        }
    }
    else
    {
        if(stAdcMeters.U_Batt_mV <= U_BATT_UV_MV)
        {
            stSystemStateBits.BattUV = 1;
        }
    }

    if(stSystemStateBits.BattOV)
    {
        if(stAdcMeters.U_Batt_mV < U_BATT_OV_RESUME_MV)
        {
            stSystemStateBits.BattOV = 0;
        }
    }
    else
    {
        if(stAdcMeters.U_Batt_mV >= U_BATT_OV_MV)
        {
            stSystemStateBits.BattOV = 1;
        }
    }
}


static void BattChgTempMonitor(void)
{
    /* Battery OT */
    if(stSystemStateBits.BattOT)
    {
        if(stAdcMeters.T_BattMeter_100mC < BATT_OT_RESUME_100MC )
        {
            stSystemStateBits.BattOT = 0u;
        }
    }
    else
    {
        if(stAdcMeters.T_BattMeter_100mC > BATT_OT_100MC)
        {
            stSystemStateBits.BattOT = 1u;
        }
        else
        {
            stSystemStateBits.BattOT = 0u;
        }
    }

    /* Battery UT */
    if(stSystemStateBits.BattUT)
    {
        if(stAdcMeters.T_BattMeter_100mC > BATT_UT_RESUME_100MC )
        {
            stSystemStateBits.BattUT = 0u;
        }
    }
    else
    {
        if(stAdcMeters.T_BattMeter_100mC < BATT_UT_100MC)
        {
            stSystemStateBits.BattUT = 1u;
        }
        else
        {
            stSystemStateBits.BattUT = 0u;
        }
    }

    /* Charger board OT */
    if(stSystemStateBits.ChgBoardOT)
    {
        if(stAdcMeters.T_BattMeter_100mC < CHG_BOARD_OT_RESUME_100MC )
        {
            stSystemStateBits.BattOT = 0u;
        }
    }
    else
    {
        if(stAdcMeters.T_BattMeter_100mC > CHG_BOARD_OT_100MC)
        {
            stSystemStateBits.BattOT = 1u;
        }
        else
        {
            stSystemStateBits.BattOT = 0u;
        }
    }
}


static void BattChgCurrentMonitor(void)
{

}


static void RelayStateMonitor(void)
{
    uint16_t U_Delta_mV = 0;

    if(  !stSystemStateBits.BattConnectReverse
      && stSystemStateBits.BattConnectOk
      && stChargeCtrlNow.RelayOnCmd)
    {
        if(stAdcMeters.U_ChgOutput_mV > stAdcMeters.U_Batt_mV)
        {
            U_Delta_mV = stAdcMeters.U_ChgOutput_mV - stAdcMeters.U_Batt_mV;
        }
        else
        {
            U_Delta_mV = stAdcMeters.U_Batt_mV - stAdcMeters.U_ChgOutput_mV;
        }


        if(U_Delta_mV > 2000)
        {
            stStatusCounter.RelayFail_Cnt++;
        }
        else
        {
            stStatusCounter.RelayFail_Cnt = stStatusCounter.RelayFail_Cnt - 2;
        }
    }
    else
    {
        stStatusCounter.RelayFail_Cnt = stStatusCounter.RelayFail_Cnt - 2;
    }

    if(stStatusCounter.RelayFail_Cnt >= 5)
    {
        stSystemStateBits.RelayFail = 1;
        stStatusCounter.RelayFail_Cnt = 5;
    }
    else if(stStatusCounter.RelayFail_Cnt < 0)
    {
        stSystemStateBits.RelayFail = 0;
        stStatusCounter.RelayFail_Cnt = 0;
    }
}


static void EnterTestMode(void)
{
    if(stAdcFilter[ADC_U_SET_TEST].SlowAvgCnt < 200)
    {
        //stSystemStateBits.EnterFCT = 1;
    }
    else
    {
        //stSystemStateBits.EnterFCT = 0;
    }
}


/* TODO: update it based on ADC sample result */
void ProductTypeDetection(void)
{
    stProductInfo.U_Type = stAdcMeters.USetup;
    stProductInfo.I_Type = stAdcMeters.ISetup;
}


void ActuatorStateUpdate(void)
{
    static uint8_t ActionPhase = 0;

    /* when start to gating make sure the sequence is:
       power on and Relay on, then MOS on */
    if(stChargeCtrlNow.MosOnCmd)
    {
        switch(ActionPhase)
        {
            case 0:
            {
                ControlChgPowerOn(stChargeCtrlNow.PowerOnCmd);
                ControlOutRelay(stChargeCtrlNow.RelayOnCmd);
            }
            break;

            case 1:
            {
                ControlMos(stChargeCtrlNow.MosOnCmd);
            }
            break;

            default:
                break;

        }

        if(++ActionPhase >= 5)
        {
            ActionPhase = 0;
        }
    }
    /* turn off everything, then don't care which one is off first actually */
    else
    {
        ActionPhase = 0;
        ControlChgPowerOn(stChargeCtrlNow.PowerOnCmd);
        ControlMos(stChargeCtrlNow.MosOnCmd);
        ControlOutRelay(stChargeCtrlNow.RelayOnCmd);
    }

    ControlFan(stChargeCtrlNow.FanOnCmd);

    if(CTRL_MosCmd == MOS_ON)
    {
        /* Ramp PWM duty, max step is 1% */
        if(stChargeCtrlNow.PWM_I_Duty_x10 > PWM_ICmdLast_x10)
        {
            PWM_ICmdLast_x10 = PWM_ICmdLast_x10 + 5;
        }
        else
        {
            PWM_ICmdLast_x10 = stChargeCtrlNow.PWM_I_Duty_x10;
        }

        if(stChargeCtrlNow.PWM_U_Duty_x10 > PWM_UCmdLast_x10)
        {
            PWM_UCmdLast_x10 = PWM_UCmdLast_x10 + 5;
        }
        else
        {
            PWM_UCmdLast_x10 = stChargeCtrlNow.PWM_U_Duty_x10;
        }
    }
    else
    {
        PWM_UCmdLast_x10 = 0;
        PWM_ICmdLast_x10 = 0;
    }

    PWM_IRefUpdate(PWM_ICmdLast_x10);
    PWM_URefUpdate(PWM_UCmdLast_x10);
}


void SM_PrintfString(char *string)
{
    printf(string);
}


/* Call every 500ms */
void SM_LedControl(void)
{
    static uint8_t LedPhase = 0;

    /* stay in test mode, then light LED in sequence */
    if(SystemStateInfo.CurrentState == SYS_FCT)
    {
        if(LedPhase == 0)
        {
            RedLedControl(LED_ON);
            GreenLedControl(LED_OFF);
            YellowLedControl(LED_OFF);
        }
        else if(LedPhase == 1)
        {
            RedLedControl(LED_OFF);
            GreenLedControl(LED_ON);
            YellowLedControl(LED_OFF);
        }
        else if(LedPhase == 2)
        {
            RedLedControl(LED_OFF);
            GreenLedControl(LED_OFF);
            YellowLedControl(LED_ON);
        }

        if(++LedPhase >= 3)
        {
            LedPhase = 0;
        }
    }
    else if(SystemStateInfo.CurrentState == SYS_IDLE)
    {
        RedLedControl(LED_OFF);
        GreenLedControl(LED_OFF);
        YellowLedControl(LED_TOGGLE);
    }

}