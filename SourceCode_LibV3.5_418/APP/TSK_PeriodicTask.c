/******************************************************************************************************
*            TSK_PeriodicTask.c
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: TSK_PeriodicTask.c
*
*    DESCRIPTION: Periodic task management function
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:12:05
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version                   2018/5/24 21:12:09
******************************************************************************************************/


/**********************************************************************************************
* Includes
**********************************************************************************************/
/* Standard lib include */


/* Chip lib include */


/* user include */
#include "TSK_TaskCommonApi.h"
#include "GPIO_Globals.h"
#include "CAN_Globals.h"
#include "CanOpenBasic.h"
#include "lcd.h"
#include "communication.h"

/********************************************************************************************************
* Global DECLARATIONS
********************************************************************************************************/
void TSK_PeriodicTaskHandler(void);


CanTxMsg TxMsg1={0xAB,0,CAN_ID_STD,CAN_RTR_DATA,8,{0xAB,0,0,0,0,0,0,0}};
CanTxMsg TxMsg2={0xCD,0,CAN_ID_STD,CAN_RTR_DATA,8,{0xCD,0,0,0,0,0,0,0}};

/********************************************************************************************************
* LOCAL FUNCTION PROTOTYPES
********************************************************************************************************/
static void PeriodTask_5ms(void);
static void PeriodTask_10ms(void);
static void PeriodTask_50ms(void);
static void PeriodTask_20ms(void);
static void PeriodTask_100ms(void);
static void PeriodTask_500ms(void);
static void PeriodTask_1s(void);

/********************************************************************************************************
* LOCAL VARIABLES
********************************************************************************************************/
/*  10ms / 5ms */
const uint16_t TSK_10msTaskBaseOn5ms = 2u;
const uint16_t TSK_20msTaskBaseOn5ms = 4u;
const uint16_t TSK_50msTaskBaseOn5ms = 10u;
const uint16_t TSK_100msTaskBaseOn5ms = 20u;
const uint16_t TSK_500msTaskBaseOn5ms = 100u;
const uint16_t TSK_1sTaskBaseOn5ms = 200u;

/****************************************************************************
*
*  Function: TSK_PeriodicTaskHandler
*
*  Purpose :    Overall task management
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Overall task management
*
****************************************************************************/
void TSK_PeriodicTaskHandler(void)
{
    static uint16_t TSK_u16TimerCntOn5ms = 0;
    static uint16_t TSK_u16TimerCnt50msOn5ms = 0;
    static uint16_t TSK_u16TimerCnt100msOn5ms = 0;
    static uint16_t TSK_u16TimerCnt500msOn5ms = 0;
    static uint16_t TSK_u16TimerCnt1sOn5ms = 0;

    PeriodTask_5ms();

    /* for other taks, Shift two cnt for each task, to decrease task number in same time */
    /* 10ms taks   */
    if((TSK_u16TimerCntOn5ms % TSK_10msTaskBaseOn5ms) == 0)
    {
        PeriodTask_10ms();
    }
    /* 20ms taks   */
    if((TSK_u16TimerCntOn5ms % TSK_20msTaskBaseOn5ms) == 1)
    {
        PeriodTask_20ms();
    }
    /* 50ms taks   */
    if(++TSK_u16TimerCnt50msOn5ms >= TSK_50msTaskBaseOn5ms)
    {
        TSK_u16TimerCnt50msOn5ms = 0;
        PeriodTask_50ms();
    }
    /* 100ms taks   */
    if(++TSK_u16TimerCnt100msOn5ms >= TSK_100msTaskBaseOn5ms)
    {
        TSK_u16TimerCnt100msOn5ms = 0;
        PeriodTask_100ms();
    }
    /* 500ms taks   */
    if(++TSK_u16TimerCnt500msOn5ms >= TSK_500msTaskBaseOn5ms)
    {
        TSK_u16TimerCnt500msOn5ms = 0;
        PeriodTask_500ms();
    }
    /* 1s taks   */
    if(++TSK_u16TimerCnt1sOn5ms >= TSK_1sTaskBaseOn5ms)
    {
        TSK_u16TimerCnt1sOn5ms = 0;
        PeriodTask_1s();
    }

    TSK_u16TimerCntOn5ms++;
}


/****************************************************************************
*
*  Function: PeriodTask_5ms
*
*  Purpose :    5ms task
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Handle 5ms task
*
****************************************************************************/
static void PeriodTask_5ms(void)
{
    Communicate();
}


/****************************************************************************
*
*  Function: PeriodTask_10ms
*
*  Purpose :    10ms task
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Handle 10ms task
*
****************************************************************************/
static void PeriodTask_10ms(void)
{
    IWDG_ReloadCounter();
    Ctrl_ADC();
    KeyScan();
    KeyFilt();
    KeyProc();
    GetInputState();
    LoadRefresh();
    //Display();
    LEDDisp();
}


/****************************************************************************
*
*  Function: PeriodTask_20ms
*
*  Purpose :    20ms task
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Handle 20ms task
*
****************************************************************************/
static void PeriodTask_20ms(void)
{

}


/****************************************************************************
*
*  Function: PeriodTask_50ms
*
*  Purpose :    20ms task
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Handle 50ms task
*
****************************************************************************/
static void PeriodTask_50ms(void)
{

}

/****************************************************************************
*
*  Function: PeriodTask_100ms
*
*  Purpose :    100ms task
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Handle 100ms task
*
****************************************************************************/
static void PeriodTask_100ms(void)
{

}

/****************************************************************************
*
*  Function: PeriodTask_500ms
*
*  Purpose :    500ms task
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Handle 500ms task
*
****************************************************************************/
static void PeriodTask_500ms(void)
{
    GPIO_TogglePin(GPIOC, GPIO_Pin_0);
    Ctrl();
    CommunicateWDG();
}



/****************************************************************************
*
*  Function: PeriodTask_1s
*
*  Purpose :    1s task
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   Handle 1s task
*
****************************************************************************/
static void PeriodTask_1s(void)
{
  
}
