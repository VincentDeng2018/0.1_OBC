/******************************************************************************************************
*            PWM_Driver.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: PWM_Driver.c
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

#include "PWM_Globals.h"


/*********************************************************************************
* Global Functions
*********************************************************************************/
void PWM_IRefUpdate(uint16_t dutyX10);
void PWM_URefUpdate(uint16_t dutyX10);
uint16_t PWM_GetPwmDuty(uint8_t PwmChannel);

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


/****************************************************************************
*
*  Function: PWM_Initial
*
*  Purpose :    Initial PWM setting
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   
*
****************************************************************************/
void PWM_Initial(void)
{
    TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitdata;	 //基本参数
    TIM_OCInitTypeDef TIM_OCInitStructure;	 //PWM输出,
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	 //开启tim4的clk,
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4,ENABLE);	 //通过对应位设置使Tim4寄存器复位,
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4,DISABLE);	//退出复位, 
    
    TIM_TimeBaseInitdata.TIM_Prescaler = 0x0;	 //设置tim的预分频系数=0,72M全频,
    TIM_TimeBaseInitdata.TIM_ClockDivision = TIM_CKD_DIV1;	 //设置tim的时钟分频因子,Tdts=n*Tck_int,
    TIM_TimeBaseInitdata.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数,
    TIM_TimeBaseInitdata.TIM_Period = PWM_VAL_MAX;	 //设置tim的自动重载,13位分辨率约7.5K,
    TIM_TimeBaseInitdata.TIM_RepetitionCounter = 0x00;	 //仅用于tim1&tim8,
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitdata);
    TIM_ARRPreloadConfig(TIM4,ENABLE);	 //允许Tim2计数器的ARR自动重载缓冲,
    
    //----Tim4的CHx_PWM的初始化参数,-----------------------------------------------
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	 //设置OC1M[2:0]为pwm1输出模式,OC1REF="1"为有效,
    TIM_OCInitStructure.TIM_Pulse=0x1fff;	 //设置占空比时间,值将自动写入CCR1,
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	 //设置输出极性,CCER的CC1P位,实际1=OC1REF的有效电平,
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能该通道输出,CCER的CC1E位,
    
    //----高级定时器才有的参数,----------------------------------------------------
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;	 //空闲/死区输出状态=0,Timx_CR2的OISx,只有Tim1/8有,
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;	 //空闲/死区输出状态=0,Timx_CR2的OISxN,只有Tim1/8有,
    TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;	 //设置互补端输出极性,CCER的CC1NP位,
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;	//无互补端输出,CCER的CC1NE位,
    
    //----按照指定参数初始化TIM_OC1~4,
    TIM_OC3Init(TIM4,&TIM_OCInitStructure);	 //按照指定参数初始化TIM_OC3,
    TIM_OC3FastConfig(TIM4,TIM_OCFast_Enable); //开启快速输出模式,3周期,
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能TIM2在CCR3上的预装载寄存器,CCMR2_OC3PE=1,
    TIM_OC4Init(TIM4,&TIM_OCInitStructure);	 //按照指定参数初始化TIM_OC4,
    TIM_OC4FastConfig(TIM4,TIM_OCFast_Enable); //开启快速输出模式,3周期,
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能TIM2在CCR4上的预装载寄存器,CCMR2_OC4PE=1,
    
    //----定时器启动-----------------------
    TIM_ITConfig(TIM4, TIM_IT_Update,DISABLE); //关中断,
    TIM_ClearFlag(TIM4, TIM_FLAG_Update); //必须先清除配置时候产生的更新标志,
    TIM_Cmd(TIM4, ENABLE); //使能定时器,
    
    //PWM_URefUpdate(200);
    //PWM_IRefUpdate(500);
}



/****************************************************************************
*
*  Function: PWM_Initial
*
*  Purpose :    Initial PWM setting
*
*  Parms Passed   :   dutyX10: e.g: 500 means 50%
*
*  Returns        :   Nothing
*
*  Description    :   
*
****************************************************************************/
void PWM_IRefUpdate(uint16_t dutyX10)
{
    uint16_t newCompareVal = 0;
    
    if(dutyX10 != 0)
    {
        newCompareVal = dutyX10 * PWM_CNT_0_1_DUTY;
        
        if(newCompareVal > PWM_VAL_MAX)
        {
            newCompareVal = PWM_VAL_MAX;
        }
        TIM_Cmd(TIM4, ENABLE); //使能定时器,
        TIM_SetCompare3(TIM4, newCompareVal);
    }
    else
    {
        TIM_SetCompare3(TIM4, 0);
        TIM_Cmd(TIM4, DISABLE); //使能定时器,
    }
}


/****************************************************************************
*
*  Function: PWM_Initial
*
*  Purpose :    Initial PWM setting
*
*  Parms Passed   :   dutyX10: e.g: 500 means 50%
*
*  Returns        :   Nothing
*
*  Description    :   
*
****************************************************************************/
void PWM_URefUpdate(uint16_t dutyX10)
{
    uint16_t newCompareVal = 0;
    
    if(dutyX10 != 0)
    {
        newCompareVal = dutyX10 * PWM_CNT_0_1_DUTY;
        
        if(newCompareVal > PWM_VAL_MAX)
        {
            newCompareVal = PWM_VAL_MAX;
        }
        TIM_Cmd(TIM4, ENABLE); //使能定时器,
        TIM_SetCompare4(TIM4, newCompareVal);
    }
    else
    {
        TIM_SetCompare4(TIM4, 0);
        TIM_Cmd(TIM4, DISABLE); //使能定时器,
    }
}