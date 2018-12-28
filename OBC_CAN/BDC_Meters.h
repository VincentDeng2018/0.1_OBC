//----------------------------------------------------------------------------
#ifndef __BDC_METERS_H
#define __BDC_METERS_H
typedef enum
{
    cLeg1Current,
    cLeg2Current,
    cLeg3Current,
    cLeg4Current,
    cLeg5Current,
    cBattCurrent,
    cOutHvCurrent,

    /************* Voltage Meter *************/
    cPackVoltage,
    cLvVoltage,
    cHVVoltage,
    cBatt1Cell1Voltage,
    cBatt1Cell2Voltage,
    cBatt1Cell3Voltage,
    cBatt1Cell4Voltage,
    
    /************ Power Meter ***************/
    cDischargePower,
    cChargePower,
    
    /********** Battery SOC and SOH ********/
    cBatt1SOC,
    cBatt1SOH,
    //cBatt2SOC,
    //cBatt2SOH,
    //cBatt3SOC,
    //cBatt3SOH,
    //cBatt4SOC,
    //cBatt4SOH,
    
    /************ Temperature ****************/
    cBatt1Temp1,
    cBatt1Temp2,
    cCtrlBoardTemp,
    cLvBoardTemp,
    cHvBoardTemp,
    
    /************ State info ************/
    cPresentState,
    cPreviousState,
    cChargeRetryCnt,
    cDischargeRetryCnt,
    cSystemStatebit,
    cQuiteChargeReason,
    cQuiteDischgReason,
    cBalanceState,
    
    /************ Statistics info ************/
    cTotalDischarge,
    cTotalCharge,
    
    cEndOfMeter,
}enBdcMeterIndex;

typedef enum
{
    BMS_INITIAL = 0x0,  /* 0 */
    BMS_IDLE,           /* 1 */
    BMS_TEST_CHG,       /* 2 */
    BMS_TEST_DISCHG,    /* 3 */
    BMS_CHARGE_ON,      /* 4 */
    BMS_DISCHARGE_ON,   /* 5 */
    BMS_SLEEP,          /* 6 */
    BMS_ERROR_OFF,      /* 7 */
    BMS_PRE_CHG,        /* 8 */
    BMS_INVALID
}enBMSStateMachine;

//----------------------------------------------------------------------------
#endif    
