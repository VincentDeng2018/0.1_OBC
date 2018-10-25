/******************************************************************************************************
*            METER_AdcSample.c
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: METER_AdcSample.c
*
*    DESCRIPTION: Filter ADC result and convert to physical meters
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/6/4 10:05:45
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version, copy from Solar project
******************************************************************************************************/


/**********************************************************************************************
* Includes
**********************************************************************************************/
/* Standard lib include */

/* Chip lib include */
/* user include */
#include "METER_AdcSample.h"
#include "DCDC_Control.h"


#define ADC_VBATT_MAX_MV        16500L
#define ADC_LVOUT_MAX_10MV      6600L
#define ADC_HVOUT_MAX_100MV      5770L
#define ADC_VOUT_1V_10MV        62u
#define ADC_TEMP_OFFSET_FACTOR  500

#ifndef NULL
#define NULL (0)
#endif

/********************************************************************************
* Global DECLARATIONS
********************************************************************************/
void f_AdcFastFilter(uint16_t adcCnt, enAdcIndex index, uint16_t autoZero);
void f_AdcSlowFilter(enAdcIndex index);
uint16_t f_GetAdcResult(enAdcIndex index);
uint16_t ISampleAutoZero(void);
void ADC_FilterIniital(void);
void ADC_EnableAutoZero(uint16_t enableOrNot);

/********************************************************************************
* LOCAL FUNCTION PROTOTYPES
********************************************************************************/
static void f_LvLCurrentSlowFilterCb(enAdcIndex adcIndex);
static void f_PackVoltsSlowFilterCb(enAdcIndex adcIndex);
static void f_LvVoltsFilterCb(enAdcIndex adcIndex);
static void f_HvCurrrentFilterCb(enAdcIndex adcIndex);
static void f_HvVoltsFilterCb(enAdcIndex adcIndex);
static void f_BoardTemperatureFilterCb(enAdcIndex adcIndex);
static void f_AuxPowerFilterCb(enAdcIndex adcIndex);


/********************************************************************************
* Global VARIABLES
********************************************************************************/
stAdcFilter_t stAdcFilter[ADC_END] =
{
//  Raw fastG fastPre fastCur zeroCnt zeroOffset SlowShift SlowCnt SlowSum SlowFilterCb

    {0, 2,    0,      0,      0,      0,         4,       0,      0,    &f_PackVoltsSlowFilterCb},  //T_BattMeter_100mC
    {0, 1,    0,      0,      0,      0,         4,       0,      0,    &f_LvVoltsFilterCb},        //T_ChargeMeter_100mC
    
    {0, 2,    0,      0,      0,      0,         3,       0,      0,    &f_LvLCurrentSlowFilterCb},    //U_Pack_mV
    {0, 2,    0,      0,      0,      0,         3,       0,      0,    &f_LvLCurrentSlowFilterCb},    //U_Batt_mV
    {0, 2,    0,      0,      0,      0,         3,       0,      0,    &f_LvLCurrentSlowFilterCb},    //U_ChgOutput_mV
    {0, 2,    0,      0,      0,      0,         3,       0,      0,    &f_LvLCurrentSlowFilterCb},    //U_KeySelected_10mV
    {0, 2,    0,      0,      0,      0,         3,       0,      0,    &f_LvLCurrentSlowFilterCb},    //U_ISet_10mV
    
    {0, 2,    0,      0,      0,      0,         4,       0,      0,    &f_HvVoltsFilterCb},        //U_CSet_10mV
    {0, 2,    0,      0,      0,      0,         4,       0,      0,    &f_HvCurrrentFilterCb},     //I_Charge_mA

    {0, 3,    0,      0,      0,      0,         4,       0,      0,    &f_BoardTemperatureFilterCb},   //U_Test_10mV
};


stADCMeter_t stAdcMeters = {0};


/********************************************************************************
* LOCAL VARIABLES
********************************************************************************/
static uint16_t ADC_EnableAutoZeroFlag = 0;

/****************************************************************************
*
*  Function: ADC_FilterIniital
*
*  Purpose :    Initial ADC filter structure
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   
*
****************************************************************************/
void ADC_FilterIniital(void)
{
    uint16_t i = 0;
    
    for(i = 0; i < ADC_END; i++)
    {
        if((stAdcFilter[i].SlowAvgShift == 0) || ((1 << stAdcFilter[i].SlowAvgShift) >= 1024))
        {
            stAdcFilter[i].SlowAvgShift = 5;
        }
        
        stAdcFilter[i].SlowAvgCnt = 0;
        stAdcFilter[i].SlowAvgSum = 0;
    }
}


/****************************************************************************
*
*  Function: f_AdcSampleSWICallback
*
*  Purpose :    ADC sequence sample finish ISR call back function
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Note: this call back function should not take much time, don't add any
*                     operation containing OS delay API or hard code delay;
*
****************************************************************************/
void f_AdcSampleSWICallback(void)
{

}


/****************************************************************************
*
*  Function: f_AdcSampleSlowTask
*
*  Purpose :    Run ADC slow task, mainly for slow ADC channel, such as temperature
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Suggest to run every <= 100ms
*
****************************************************************************/
void f_AdcSampleSlowTask(void)
{

}


/****************************************************************************
*
*  Function: f_AdcFastFilter
*
*  Purpose :    ADC filter common API
*
*  Parms Passed   :   index: ADC channel
*
*  Returns        :   Nothing
*
*  Description    :   This filter is used as fast ADC FIR filter
*
****************************************************************************/
void f_AdcFastFilter(uint16_t adcCnt, enAdcIndex index, uint16_t autoZero)
{
    int32_t tempVal = (int32_t)adcCnt; /* get current ADC counter */

    stAdcFilter_t *pAdcFilter = &stAdcFilter[index];

    if(autoZero)
    {
        pAdcFilter->zeroOffsetCali =  ((tempVal - pAdcFilter->zeroCntVal) >> 1)
                                + (pAdcFilter->zeroOffsetCali >> 1);

        if(pAdcFilter->zeroOffsetCali > 40)
        {
            pAdcFilter->zeroOffsetCali = 40;
        }
        else if(pAdcFilter->zeroOffsetCali < -40)
        {
            pAdcFilter->zeroOffsetCali = -40;
        }
    }

    tempVal -= pAdcFilter->zeroOffsetCali;

    if(tempVal < 0)
    {
        tempVal = 0;
    }

#ifdef FAST_FILTER_ENABLE
    /* Y(n) = (X(n) + (1<<fast - 1)Y(n-1)) >> fastGain */
    //tempVal = tempVal + (uint32_t)pAdcFilter->fastPreVal * ((1u << (uint32_t)pAdcFilter->fastGain) - 1);
    tempVal = tempVal + ((uint32_t)pAdcFilter->fastPreVal << pAdcFilter->fastGain) - (uint32_t)pAdcFilter->fastPreVal;
    pAdcFilter->fastCurrVal =  (uint16_t)(tempVal >> pAdcFilter->fastGain);
    pAdcFilter->fastPreVal = pAdcFilter->fastCurrVal;
#else
    pAdcFilter->fastCurrVal = tempVal;
#endif
    /* uncomment this line to allow slow filter running if not called by ISR */
    /* call slow filter */
    f_AdcSlowFilter(index);
}


/****************************************************************************
*
*  Function: f_AdcSlowFilter
*
*  Purpose :    ADC slow filter
*
*  Parms Passed   :   index: ADC channel
*
*  Returns        :   Nothing
*
*  Description    :   The filter method is calculating average counter
*
****************************************************************************/
void f_AdcSlowFilter(enAdcIndex index)
{
    /* sum the ADC value */
    stAdcFilter[index].SlowAvgSum += stAdcFilter[index].fastCurrVal;
    stAdcFilter[index].SlowAvgCnt++;

    /* Average counter has arrived, then call cb function to handle them */
    if(stAdcFilter[index].SlowAvgCnt >=  (1 << stAdcFilter[index].SlowAvgShift))
    {
        if(NULL != stAdcFilter[index].SlowFilterCb)
        {
            stAdcFilter[index].SlowFilterCb(index);
        }

        /* Clear temp result */
        stAdcFilter[index].SlowAvgCnt = 0;
        stAdcFilter[index].SlowAvgSum = 0;
    }
}



/****************************************************************************
*
*  Function: ISampleAutoZero
*
*  Purpose :    To detect whether it is the time to zero offset calc for I sample
*
*  Parms Passed   :   None
*
*  Returns        :   1 means ok to zero calc, 0 means not
*
*  Description    :
*
****************************************************************************/
uint16_t ISampleAutoZero(void)
{
    return(ADC_EnableAutoZeroFlag == 1);
}


void ADC_EnableAutoZero(uint16_t enableOrNot)
{
    ADC_EnableAutoZeroFlag = enableOrNot;
}


/* call back function to convert ADC counter to physical value */
static void f_PackVoltsSlowFilterCb(enAdcIndex adcIndex)
{
    uint32_t tempVal = stAdcFilter[adcIndex].SlowAvgSum >> stAdcFilter[adcIndex].SlowAvgShift;

    stAdcMeters.f_PackVoltsSlowFilterCb = (uint16_t)(tempVal * (uint32_t)10 / (uint32_t)113);
}
