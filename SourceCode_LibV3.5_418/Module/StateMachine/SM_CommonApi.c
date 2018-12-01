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

/********************************************************************************
* Global DECLARATIONS
********************************************************************************/
void SM_SystemStateBitsUpdate(void);

/********************************************************************************
* Global VARIABLES
********************************************************************************/
stSystemStateBits_t stSystemStateBits = {0u};
stStatusCounter_t stStatusCounter = {0u};

const uint16_t awdVoltTypeValue[VOLTAGE_TYPE_NUM] = {1200, 2400, 3600, 4800};
const uint16_t awdCurTypeValue[CURRENT_TYPE_NUM] = {500, 1000, 1500, 2000,2500,3000};
/********************************************************************************
* LOCAL FUNCTION PROTOTYPES
********************************************************************************/


/********************************************************************************
* LOCAL VARIABLES
********************************************************************************/


/****************************************************************************
*
*  Function: SM_BatteryVoltageMonitor
*
*  Purpose :    Battery Voltage detection
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Suggest to run every 100ms or faster
*
****************************************************************************/
void SM_BatteryVoltageMonitor(void)
{

}


/****************************************************************************
*
*  Function: SM_TemperatureMonitor
*
*  Purpose :    Temperature detection
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Suggest to run every 100ms or faster
*
****************************************************************************/
void SM_TemperatureMonitor(void)
{
    /* over temperature detection: chg and discharge setting is same */
    if(stSystemStateBits.BattOT)
    {
        if(stAdcMeters.T_BattMeter_100mC < BATT_OT_RESUME_100MC)
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

    /* charge under temperature detection */
    if(stSystemStateBits.BattUT)
    {
        if(stAdcMeters.T_BattMeter_100mC > BATT_UT_RESUME_100MC)
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

    /* discharge under temperature detection */
    if(stSystemStateBits.ChgBoardOT)
    {
        if(stAdcMeters.T_ChargeMeter_100mC < CHG_BOARD_OT_RESUME_100MC)
        {
            stSystemStateBits.ChgBoardOT = 0u;
        }
    }
    else
    {
        if(stAdcMeters.T_ChargeMeter_100mC > CHG_BOARD_OT_100MC)
        {
            stSystemStateBits.ChgBoardOT = 1u;
        }
        else
        {
            stSystemStateBits.ChgBoardOT = 0u;
        }
    }
}


void SM_SystemStateBitsUpdate(void)
{
    SM_BatteryVoltageMonitor();
    SM_TemperatureMonitor();

    /* some statebits are determined by several bits, update here */

    stSystemStateBits.AllowEnterChg = !stSystemStateBits.BattOV
                                      && !stSystemStateBits.BattAbsUV
                                      && !stSystemStateBits.ChgBoardOT
                                      && !stSystemStateBits.BattOT
                                      && !stSystemStateBits.BattUT
                                      && !stSystemStateBits.ChgBoardOT
                                      && !stSystemStateBits.RelayFail
                                      && !stSystemStateBits.FanFail
                                      && !stSystemStateBits.ChargeOC
                                      && !stSystemStateBits.AcLow
                                      && !stSystemStateBits.AcOff
                                      && stSystemStateBits.BattConnectOk
                                      && !stSystemStateBits.BattConnectReverse
                                      &&!stSystemStateBits.EnterTest;
                                      
    stSystemStateBits.AllowStayChg = !stSystemStateBits.BattOV
                                      && !stSystemStateBits.BattAbsUV
                                      && !stSystemStateBits.ChgBoardOT
                                      && !stSystemStateBits.BattOT
                                      && !stSystemStateBits.BattUT
                                      && !stSystemStateBits.ChgBoardOT
                                      && !stSystemStateBits.RelayFail
                                      && !stSystemStateBits.FanFail
                                      && !stSystemStateBits.ChargeOC
                                      && !stSystemStateBits.AcLow
                                      && stSystemStateBits.BattConnectOk
                                      && !stSystemStateBits.BattConnectReverse;
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
    
    /* reach Battery reverse counter? */ 
    if(stStatusCounter.BattConnectOk_Cnt >= CONN_OK_ACTIVE_CNT)
    {
        stStatusCounter.BattConnectOk_Cnt = CONN_OK_ACTIVE_CNT;
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
    if(stStatusCounter.BattConnectOk_Cnt >= CONN_OK_ACTIVE_CNT)
    {
        stStatusCounter.BattConnectOk_Cnt = CONN_OK_ACTIVE_CNT;
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
}


static void BattChgVoltMonitor(void)
{
    
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

}


static void EnterTestMode(void)
{
    if(stAdcFilter[ADC_U_SET_TEST].SlowAvgCnt < 200)
    {
        stSystemStateBits.EnterTest = 1;
    }
    else
    {
        stSystemStateBits.EnterTest = 0;
    }
}


/* TODO: update it based on ADC sample result */
static void ProductTypeDetection(void)
{
    stProductInfo.U_Type = 0;
    stProductInfo.I_Type = 0;
}
