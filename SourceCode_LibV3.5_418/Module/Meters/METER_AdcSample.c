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
static void f_BattVoltsSlowFilterCb(enAdcIndex adcIndex);
static void f_KeyPressedFilterCb(enAdcIndex adcIndex);
static void f_ISetFilterCb(enAdcIndex adcIndex);
static void f_CSetFilterCb(enAdcIndex adcIndex);
static void f_USetFilterCb(enAdcIndex adcIndex);
static void f_BattTempFilterCb(enAdcIndex adcIndex);
static void f_BattCurrentSlowFilterCb(enAdcIndex adcIndex);


/********************************************************************************
* Global VARIABLES
********************************************************************************/

stTemperatureTable_t stTemperatureTable[] =
{
    {-300,   3569},
    {-200,   3255},
    {-100,   2860},
    {0   ,   2414},
    {100 ,   1963},
    {200 ,   1549},
    {300 ,   1198},
    {400 ,   915 },
    {500 ,   696 },
    {600 ,   531 },
    {700 ,   407 },
    {800 ,   314 },
    {900 ,   244 },
    {1000,   192 },
    {1100,   153 },
    {1200,   122 },
};
#define TEMP_TABLE_LEN (sizeof(stTemperatureTable) / sizeof(stTemperatureTable_t))

stAdcFilter_t stAdcFilter[ADC_U_BAT_NEG] =
{
//  Raw fastG fastPre fastCur zeroCnt zeroOffset SlowShift SlowAccCnt SlowSum SlowAvgCnt SlowFilterCb
    {0, 2,    0,      0,      0,      0,         6,       0,      0,  0,  &f_KeyPressedFilterCb},       //ADC_KEY
    {0, 1,    0,      0,      0,      0,         4,       0,      0,  0,  &f_ISetFilterCb},    //ADC_I_SET
    {0, 2,    0,      0,      0,      0,         3,       0,      0,  0,  &f_CSetFilterCb},    //ADC_C_SET
    {0, 2,    0,      0,      0,      0,         3,       0,      0,  0,  &f_BattCurrentSlowFilterCb},    //ADC_I_CHG
    {0, 2,    0,      0,      0,      0,         3,       0,      0,  0,  &f_USetFilterCb},    //ADC_U_SET_TEST
    {0, 2,    0,      0,      0,      0,         3,       0,      0,  0,  &f_BattTempFilterCb},    //ADC_TEMP1
    {0, 2,    0,      0,      0,      0,         3,       0,      0,  0,  &f_BattTempFilterCb},    //ADC_TEMP2
    {0, 2,    0,      0,      0,      0,         3,       0,      0,  0,  &f_BattVoltsSlowFilterCb},    //ADC_U_PACK
    {0, 2,    0,      0,      0,      0,         4,       0,      0,  0,  &f_BattVoltsSlowFilterCb},        //ADC_U_OUT
    {0, 2,    0,      0,      0,      0,         4,       0,      0,  0,  &f_BattVoltsSlowFilterCb},     //ADC_U_BAT_POS
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
        
        stAdcFilter[i].SlowAvgAccCnt = 0;
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
    stAdcFilter[index].SlowAvgAccCnt++;

    /* Average counter has arrived, then call cb function to handle them */
    if(stAdcFilter[index].SlowAvgAccCnt >=  (1 << stAdcFilter[index].SlowAvgShift))
    {
        stAdcFilter[index].SlowAvgCnt = (uint16_t)(stAdcFilter[index].SlowAvgSum >> stAdcFilter[index].SlowAvgShift); 
        if(NULL != stAdcFilter[index].SlowFilterCb)
        {
            stAdcFilter[index].SlowFilterCb(index);
        }

        /* Clear temp result */
        stAdcFilter[index].SlowAvgAccCnt = 0;
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
static void f_BattVoltsSlowFilterCb(enAdcIndex adcIndex)
{
    uint32_t tempVal = stAdcFilter[adcIndex].SlowAvgCnt;
    
    /* 3.3V --> 16830mV */
    tempVal = (tempVal * 16830) >> 12;
    
    if(adcIndex == ADC_U_PACK)
    {
        stAdcMeters.U_Pack_mV = (uint16_t)tempVal;
    }
    else if(adcIndex == ADC_U_OUT)
    {
        stAdcMeters.U_ChgOutput_mV = (uint16_t)tempVal;
    }
    else
    {
        stAdcMeters.U_Batt_mV = (uint16_t)tempVal;
    }
}

static void f_BattCurrentSlowFilterCb(enAdcIndex adcIndex)
{
    uint32_t tempVal = stAdcFilter[adcIndex].SlowAvgCnt;
    
    /* 1.3V --> MaxCurrent */
    tempVal = (tempVal * 5000) / 1616;
    

    stAdcMeters.I_Charge_mA = (uint16_t)tempVal;
}


/* call back function to convert ADC counter to physical value */
static void f_BattTempFilterCb(enAdcIndex adcIndex)
{
    int32_t tempVal = stAdcFilter[adcIndex].SlowAvgCnt;
    int32_t a = 0;
    
    uint16_t i = 0;
    
    int16_t *pTemperature = &stAdcMeters.T_BattMeter_100mC;
    
    if(adcIndex == ADC_TEMP1)
    {
        pTemperature = &stAdcMeters.T_ChargeMeter_100mC;
    }
    
    /* Y = aX + b: Y = NewTemperature_100mc, X = newAdcCounter - stTemperatureTable[i].AdcCounter
    a = (stTemperatureTable[i+1].TBatt_100mc - stTemperatureTable[i].TBatt_100mc) /
        (stTemperatureTable[i+1].AdcCounter - stTemperatureTable[i].AdcCounter)
    b = stTemperatureTable[i].TBatt_100mc
    */
        
    if(tempVal >= stTemperatureTable[0].AdcCounter)
    {
        *pTemperature = stTemperatureTable[0].TBatt_100mc;
    }
    else if(tempVal <= stTemperatureTable[TEMP_TABLE_LEN - 1].AdcCounter)
    {
        *pTemperature = stTemperatureTable[TEMP_TABLE_LEN - 1].TBatt_100mc;
    }
    else
    {
        for(i = 0; i < TEMP_TABLE_LEN; i++)
        {
            if(tempVal > stTemperatureTable[i].AdcCounter)
            {
                a = (int32_t)(stTemperatureTable[i - 1].TBatt_100mc - stTemperatureTable[i].TBatt_100mc);
                a = a * (tempVal - (int32_t)stTemperatureTable[i - 1].AdcCounter);
                a = a / ((int32_t)stTemperatureTable[i - 1].AdcCounter - (int32_t)stTemperatureTable[i].AdcCounter);
                *pTemperature = (int16_t)(a + stTemperatureTable[i -1].TBatt_100mc);
                break;
            }
        }
    }
       
}

static void f_KeyPressedFilterCb(enAdcIndex adcIndex)
{
    uint16_t tempVal = stAdcFilter[adcIndex].SlowAvgCnt;
    
    //No Key= 4096 
    //Key1  = 3097
    //Key2  = 2048       
    //Key3  = 1084 
    //Key4  = 0           
          
    if(tempVal > 3596)
    {
        stAdcMeters.enKeyPressed = KEY_NONE;
    }
    else if(tempVal > 2572)
    {
        stAdcMeters.enKeyPressed = KEY_1_UP;
    }
    else if(tempVal > 1566)
    {
        stAdcMeters.enKeyPressed = KEY_2_DOWN;
    }
    else if(tempVal > 542)
    {
        stAdcMeters.enKeyPressed = KEY_3_BACK;
    }
    else
    {
        stAdcMeters.enKeyPressed = KEY_4_ENTER;
    }
}


static void f_ISetFilterCb(enAdcIndex adcIndex)
{
    uint16_t tempVal = stAdcFilter[adcIndex].SlowAvgCnt;
    
    //0 0    4096
    //0 1    2707
    //1 0    2048
    //1 1    1630

    if(tempVal > 3900)
    {
        stAdcMeters.ISetup = SETUP_0;
    }
    else if(tempVal > 2600)
    {
        stAdcMeters.ISetup = SETUP_1;
    }
    else if(tempVal > 1800)
    {
        stAdcMeters.ISetup = SETUP_2;
    }
    else
    {
        stAdcMeters.ISetup = SETUP_3;
    }
}


static void f_CSetFilterCb(enAdcIndex adcIndex)
{
    uint16_t tempVal = stAdcFilter[adcIndex].SlowAvgCnt;
    
    //0 0    4096
    //0 1    2707
    //1 0    2048
    //1 1    1630

    if(tempVal > 3900)
    {
        stAdcMeters.USetup = SETUP_0;
    }
    else if(tempVal > 2600)
    {
        stAdcMeters.USetup = SETUP_1;
    }
    else if(tempVal > 1800)
    {
        stAdcMeters.USetup = SETUP_2;
    }
    else
    {
        stAdcMeters.USetup = SETUP_3;
    }
}


static void f_USetFilterCb(enAdcIndex adcIndex)
{
    uint16_t tempVal = stAdcFilter[adcIndex].SlowAvgCnt;
    
    //0 0    4096
    //0 1    2707
    //1 0    2048
    //1 1    1630

    if(tempVal > 3900)
    {
        stAdcMeters.USetup = SETUP_0;
    }
    else if(tempVal > 2600)
    {
        stAdcMeters.USetup = SETUP_1;
    }
    else if(tempVal > 1800)
    {
        stAdcMeters.USetup = SETUP_2;
    }
    else
    {
        stAdcMeters.USetup = SETUP_3;
    }
}

uint16_t f_GetAdcFilterResult(enAdcIndex index)
{
    return stAdcFilter[index].SlowAvgCnt;
}