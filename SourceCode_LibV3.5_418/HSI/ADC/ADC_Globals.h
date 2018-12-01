/******************************************************************************************************
*            ADC_Globals.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: ADC_Globals.h
*
*    DESCRIPTION: Header file for task define
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:48:19
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                      2018/5/24 21:48:23
******************************************************************************************************/

#ifndef _ADC_GLOBALS_H
#define _ADC_GLOBALS_H

#include "stm32f10x_adc.h"
#include "ADC_Config.h"
/**********************************************************************************************
* Macros
**********************************************************************************************/
typedef enum
{
    ADC_KEY = 0,
    ADC_I_SET,
    ADC_C_SET,
    ADC_I_CHG,
    ADC_U_SET_TEST,
    ADC_TEMP1,
    ADC_TEMP2,
    ADC_U_PACK,
    ADC_U_OUT,
    ADC_U_BAT_POS,
    ADC_U_BAT_NEG,
    ADC_END
}enAdcIndex;

extern uint16_t DMA_AdcResult[ADC_END];

/**********************************************************************************************
* Global Functions
**********************************************************************************************/
extern void ADC_Initial(void);
extern void ADC_DMA_IRQHandler(void);

#endif