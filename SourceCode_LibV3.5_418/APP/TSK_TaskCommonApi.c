/******************************************************************************************************
*            TSK_TaskCommonApi.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: TSK_TaskCommonApi.h
*
*    DESCRIPTION: Define task related API, array, etc
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/10 22:34:20
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/5/10 22:34:22
******************************************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "TSK_TaskCommonApi.h"
#include "CanOpenBasic.h"

/*********************************************************************************
* Global Functions
*********************************************************************************/
void TSK_CreateTasks(void);
void f_DelayForX_us(const uint32_t s_u32delayTime_us);


/*********************************************************************************
* Task Call back Functions
*********************************************************************************/
void f_TimerTask(void *pvParameters);
void f_StateMachineTask(void *pvParameters);
extern void LCD_TaskHandler(void *pvParameters);

/*******************************************************************************
* Macros
*******************************************************************************/
/* Define priority for every task, higher priority, the number is higher */
#define TIMER_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 1 )
#define SM_TASK_PRIORITY                    ( tskIDLE_PRIORITY + 2 )
#define CAN_RX_TASK_PRIORITY                ( tskIDLE_PRIORITY + 4 )
#define CAN_TX_TASK_PRIORITY                ( tskIDLE_PRIORITY + 3 )
#define LCD_TASK_PRIORITY                ( tskIDLE_PRIORITY + 1 )

/* Define stack size for all the tasks */
#define TIMER_TASK_STACK_SIZE             configMINIMAL_STACK_SIZE
#define SM_TASK_STACK_SIZE                configMINIMAL_STACK_SIZE
#define CAN_RX_STACK_SIZE                 (configMINIMAL_STACK_SIZE << 1)
#define CAN_TX_STACK_SIZE                 (configMINIMAL_STACK_SIZE << 1)
#define LCD_STACK_SIZE                    (configMINIMAL_STACK_SIZE << 1)

/* Define the task index */
enum TSK_ID
{
    TSK_ID_START,
    TIMER_TASK_ID  = TSK_ID_START,
    SM_TASK_ID,
    CAN_RX_ID,
    CAN_TX_ID,
    LCD_TASK_ID,
    TSK_ID_END
};


/*******************************************************************************
* Data Struct Define
*******************************************************************************/
typedef struct
{
    TaskFunction_t f_ptrTaskCode;
    const char * const s_ptru8Name;
    const uint16_t s_u16StackDepth;
    void * const s_ptrvParameters;
    UBaseType_t s_u32Priority;
    TaskHandle_t * const f_ptrCreatedTask;
}UserDefineTask;

/***************** Task List *****************/
/* Insert new taks in this array */
UserDefineTask s_arrayUserDefineTasks[TSK_ID_END] =
{
    /* Task 0: 5ms timer task */
    {
        f_TimerTask,
        "TimerTask",
        TIMER_TASK_STACK_SIZE,
        (void*)0x0,
        TIMER_TASK_PRIORITY,
        (void*)0x0,
    },

    /* Task 1: state machine task */
    {
        f_StateMachineTask,
        "SmTask",
        SM_TASK_STACK_SIZE,
        (void*)0x0,
        SM_TASK_PRIORITY,
        (void*)0x0,
    },
    
    /* Task 2: CAN RX */
    {
        CAN_RxHandlerTask,
        "CAN_RX",
        CAN_RX_STACK_SIZE,
        (void*)0x0,
        CAN_RX_TASK_PRIORITY,
        (void*)0x0,
    },
    
    /* Task 3: CAN TX */
    {
        CAN_TxHandlerTask,
        "CAN_TX",
        CAN_TX_STACK_SIZE,
        (void*)0x0,
        CAN_TX_TASK_PRIORITY,
        (void*)0x0,
    },
    
    /* Task 4: LCD Task */
    {
        LCD_TaskHandler,
        "LDC_TASK",
        LCD_STACK_SIZE,
        (void*)0x0,
        LCD_TASK_PRIORITY,
        (void*)0x0,
    },
};

/*******************************************************************************
* Local Variables
*******************************************************************************/
uint8_t s_TSK_u8OsStarted = 0u;

/****************************************************************************
*
*  Function: TSK_CreateTasks()
*
*  Purpose :    Create all the tasks when initilization and start the scheduler
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   this function is called by main function for only once
*
****************************************************************************/
void TSK_CreateTasks(void)
{
    enum TSK_ID s_u16TaskId = TSK_ID_START;

    for(; s_u16TaskId < TSK_ID_END; s_u16TaskId++)
    {
        xTaskCreate(s_arrayUserDefineTasks[s_u16TaskId].f_ptrTaskCode,
                    s_arrayUserDefineTasks[s_u16TaskId].s_ptru8Name,
                    s_arrayUserDefineTasks[s_u16TaskId].s_u16StackDepth,
                    s_arrayUserDefineTasks[s_u16TaskId].s_ptrvParameters,
                    s_arrayUserDefineTasks[s_u16TaskId].s_u32Priority,
                    s_arrayUserDefineTasks[s_u16TaskId].f_ptrCreatedTask);
    }
    /* indicate OS already started */
    s_TSK_u8OsStarted = 1u;
}


#if configUSE_TICK_HOOK > 0

void vApplicationTickHook(void);

volatile uint32_t ui32_ms_cnt = 0;

void vApplicationTickHook(void)
{
    // increments every 1ms if configCPU_CLOCK_HZ is set correctly
    ui32_ms_cnt++;
}

#endif




void f_TimerTask(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS( 5 );

    xLastWakeTime = xTaskGetTickCount();

    /* Enter the loop that defines the task behavior. */
    for( ;; )
    {
        vTaskDelayUntil( &xLastWakeTime, xPeriod );

        /* Task code added here */
        TSK_PeriodicTaskHandler();
    }
}


void f_StateMachineTask(void *pvParameters)
{
    for( ;; )
    {
        vTaskDelay(pdMS_TO_TICKS(100));

        /* Task code added here */
        //TSK_PeriodicTaskHandler();
    }
}


#if 0
/****************************************************************************
*
*  Function: f_DelayForX_us()
*
*  Purpose :    us Delay API for task delay or hard delay
*
*  Parms Passed   :   s_u32delayTime_us: how many micro seconds to delay
*
*  Returns        :   Nothing
*
*  Description    :   this function is called by main function or tasks.
*
****************************************************************************/
void f_DelayForX_us(const uint32_t s_u32delayTime_us)
{
    uint16_t s_u16delayTime_ms = 0u;
    uint16_t s_u16delayTime_us = 0u;

    uint32_t currentTickCounter = portNVIC_SYSTICK_CURRENT_VALUE_REG;

    if(s_u32delayTime_us > 1000)
    {
        s_u16delayTime_ms = s_u32delayTime_us / 1000u ;
        s_u16delayTime_us = s_u32delayTime_us % 1000u ;
    }

    if(s_u16delayTime_ms)
    {
        if(0u == s_TSK_u8OsStarted)
        {
            while(portNVIC_SYSTICK_CURRENT_VALUE_REG <= (currentTickCounter + 1))
            {

            }
        }
        else
        {
            vTaskDelay(s_u16delayTime_ms);
        }
    }
    else
    {

    }
}
#endif
