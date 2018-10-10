#include <stm32f10x.h>
#include <stdbool.h>
#include "canfestival.h"
#include "timer_canfestival.h"

/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;
__IO uint16_t CCR1_Val = 49152;

void setTimer(TIMEVAL value)
{
    uint16_t capture = 0;
    static bool bLEDOn = true;

    capture = TIM_GetCapture1(TIM5);
    TIM_SetCompare1(TIM5, capture + value);

    if(bLEDOn == true)
    {
        bLEDOn = FALSE;
        GPIO_WriteBit(GPIOF, GPIO_Pin_6, Bit_RESET);
    }
    else
    {
        bLEDOn = true;
        GPIO_WriteBit(GPIOF, GPIO_Pin_6, Bit_SET);
    }
}

TIMEVAL getElapsedTime(void)
{
    uint16_t timer = TIM_GetCounter(TIM5);
  /* Calculate the time difference */
    return (timer > last_time_set) ? (timer - last_time_set) : (last_time_set - timer);
}

/* TIM5 configure */
static void TIM5_Configuration(void)
{
    /* 时钟及分频设置 */
    {
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        /* Time Base configuration */
        /* 72M / 72 = 1us */
        TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
        //计数模式:向上计数
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseStructure.TIM_Period = 1000 - 1; /* 1000us -->1ms */
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        //重新计数的起始值
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

        TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    }


    /* TIM IT enable */
    TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);

    /* TIM enable counter */
    TIM_Cmd(TIM5, ENABLE);
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void RCC_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

  /* TIM5 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  /* clock enable */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
}

void TIM5_start(void)
{
    RCC_Configuration();
    
    /* configure TIM5 for remote and encoder */
    NVIC_Configuration();
    TIM5_Configuration();
}

void TIM5_IRQHandler(void)
{
    uint16_t capture = 0;

    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
        last_time_set = TIM_GetCounter(TIM5);
        TimeDispatch();
    }
}
