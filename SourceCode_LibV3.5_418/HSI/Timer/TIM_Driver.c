/******************************************************************************************************
*            GPIO_Driver.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: GPIO_Driver.c
*
*    DESCRIPTION: Define task related API, array, etc
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:51:20
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/5/24 21:51:24
******************************************************************************************************/

#include "TIM_Globals.h"


/*********************************************************************************
* Global Functions
*********************************************************************************/
extern uint8_t CanOpenTimerDispatch(uint16_t callPeriod);

/*********************************************************************************
* Task Call back Functions
*********************************************************************************/


/*******************************************************************************
* Macros
*******************************************************************************/


/*******************************************************************************
* Data Struct Define
*******************************************************************************/


/*******************************************************************************
* Local Variables
*******************************************************************************/
static uint16_t u16TimerLast = 0;

void TIM_Initial(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 时钟及分频设置 */
    {
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        
        /* TIM5 clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
        
        /* Time Base configuration */
        /* 72M / 72 = 1us */
        TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
        //计数模式:向上计数
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseStructure.TIM_Period = 1001 - 1; /* 1000us -->1ms */
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        //重新计数的起始值
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

        TIM_TimeBaseInit(TIMER_MODULE, &TIM_TimeBaseStructure);
    }
    

    /* TIM IT enable */
    TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);

    /* TIM enable counter */
    TIM_Cmd(TIM5, ENABLE);

    /* Enable the TIM5 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
        u16TimerLast = TIM_GetCounter(TIM5);
        CanOpenTimerDispatch(1u);
    }
}


uint16_t TIM_GetElapsedTime(void)
{
    uint16_t u16TimerNow = TIM_GetCounter(TIM5);

  /* Calculate the time difference */
    return (u16TimerNow > u16TimerLast) ? (u16TimerNow - u16TimerLast) : (u16TimerLast - u16TimerNow);
}
