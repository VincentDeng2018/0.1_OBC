/******************************************************************************************************
*            METER_AdcSample.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: METER_AdcSample.h
*
*    DESCRIPTION: Header file for ADC module
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/6/4 10:07:20
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version
******************************************************************************************************/


/**********************************************************************************************
* Includes
**********************************************************************************************/
#ifndef _METER_ADCSAMPLE_H
#define _METER_ADCSAMPLE_H


/* Chip lib include */
/* user include */
#include "ADC_Globals.h"

/*********************************************************************************
* Macros
*********************************************************************************/
#define MAX_UBATT_MV     16830
#define MAX_IBATT_CNT     1613 /* 1.3V means max current */

/*********************************************************************************
* Data and Structure
*********************************************************************************/
typedef struct
{
    int16     T_BattMeter_100mC;
    int16     T_ChargeMeter_100mC;
    uint16_t  U_Pack_mV;
    uint16_t  U_Batt_mV;
    
    uint16_t  U_ChgOutput_mV;
    uint16_t  U_KeySelected_10mV;
    uint16_t  U_ISet_10mV;
    uint16_t  U_CSet_10mV;
    
    uint16_t  I_Charge_mA;
    uint16_t  U_Test_10mV;
}stADCMeter_t;

extern stADCMeter_t stAdcMeters;

typedef void (*f_SlowFilterCallBack)(enAdcIndex enIndex);

typedef struct
{
    uint16_t adcRaw;
    uint16_t fastGain;
    uint16_t fastPreVal;
    uint16_t fastCurrVal;
    int16 zeroCntVal;
    int16 zeroOffsetCali;
    
    uint16_t SlowAvgShift : 4;  /* means 1 << n */
    uint16_t SlowAvgCnt : 12;

    uint32_t SlowAvgSum;
    
    f_SlowFilterCallBack SlowFilterCb;
}stAdcFilter_t;

extern stAdcFilter_t stAdcFilter[ADC_END];


/*********************************************************************************
* Global DECLARATIONS
*********************************************************************************/
extern void f_AdcSampleSlowTask(void);
extern void f_AdcSampleSWICallback(void);
extern uint16_t f_GetAdcResult(enAdcIndex index);
extern void f_AdcFastFilter(uint16_t adcCnt, enAdcIndex index, uint16_t autoZero);
extern uint16_t ISampleAutoZero(void);
extern void ADC_FilterIniital(void);
extern void f_AdcSlowFilter(enAdcIndex index);
extern void ADC_EnableAutoZero(uint16_t enableOrNot);

#endif
