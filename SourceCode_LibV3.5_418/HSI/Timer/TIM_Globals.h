/******************************************************************************************************
*            TIM_Globals.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: GPIO_Globals.h
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

#ifndef _TIM_GLOBALS_H
#define _TIM_GLOBALS_H

#include "stm32f10x_gpio.h"
#include "TIM_Config.h"
/**********************************************************************************************
* Macros
**********************************************************************************************/

/**********************************************************************************************
* Global Functions
**********************************************************************************************/
void TIM_Initial(void);
static inline void TIM_DisableTimerInterrupt(void);
static inline void TIM_EnableTimerInterrupt(void);

static inline void TIM_DisableTimerInterrupt(void)
{
    /* Disable the Interrupt sources */
    TIM5->DIER &= (uint16_t)~TIM_IT_CC1;
}


static inline void TIM_EnableTimerInterrupt(void)
{
    /* Enable the Interrupt sources */
    TIM5->DIER |= TIM_IT_CC1;
}

#endif
