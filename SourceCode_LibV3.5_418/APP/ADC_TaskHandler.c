/******************************************************************************************************
*            LCD_TaskHandler.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: LCD_TaskHandler.c
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

#include "lcd.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


/*********************************************************************************
* Global Functions
*********************************************************************************/


/*********************************************************************************
* Task Call back Functions
*********************************************************************************/


/*******************************************************************************
* Macros
*******************************************************************************/
typedef enum
{
    LCD_INIT = 0x0,
    LCD_TESTING,
    LCD_RUNNING, /* Plot voltage and current curve for last charge */
    LCD_SLEEP
}enLcdState;


/*******************************************************************************
* Data Struct Define
*******************************************************************************/


/*******************************************************************************
* Local Variables
*******************************************************************************/
static SemaphoreHandle_t LCD_TaskSem = NULL;
static enLcdState LCD_PresentState = LCD_INIT;


void PlotChargeCurve(void)
{
    /* draw Double Y and X line */
    //DrawLine(10, 11, 40, 320 - 20, BLACK);
    //DrawLine(10, 240 - 20, 320 - 21, 320 - 20, BLACK);
    //DrawLine(10, 240 - 20, 160 - 21, 160 - 20, BLACK);
    
    //DrawLine(30, 80, 50, 100, BLACK);
    
    /* Write text to the x,y */
    
    /* Company title */
    DispStr("PowerFirst Technology Co.LTD.", 3, 3,
                  BLACK, WHITE); // ord:0~95
    DrawLine(2, 240-2, 20, 21, BLACK); 
    
    DispStr("Model:xxxxx BattType:12V 10A", 3, 25,
                  BLACK, WHITE); // ord:0~95
    DispStr("ChrgCfg:xxxxx Status:Float", 3, 45,
                  BLACK, WHITE); // ord:0~95
    
}

void LCD_TaskHandler(void *pvParameters)
{
    LCD_TaskSem = xSemaphoreCreateBinary();

    if(LCD_TaskSem == NULL)
    {
        while(1);
    }

    for(;;)
    {
        xSemaphoreTake(LCD_TaskSem, portMAX_DELAY);
        {
            switch(LCD_PresentState)
            {
                case LCD_INIT:
                {
                    LCD_PresentState = LCD_TESTING;
                    LCD_Init();
                    break;
                }

                case LCD_TESTING:
                    //DispBand();
                    PlotChargeCurve();
                    //DispScaleVer_Green();
                    
                    break;

                case LCD_RUNNING:
                    break;


                case LCD_SLEEP:
                    break;

                default:
                    break;
            }
        }
    }
}


void LCD_TaskTrigger(void)
{
    xSemaphoreGive(LCD_TaskSem);
}

void Delay(uint16_t x_ms)
{
    vTaskDelay(x_ms);
}


