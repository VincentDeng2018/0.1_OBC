/******************************************************************************************************
*            ADC_TaskHandler.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: ADC_TaskHandler.c
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

#include "ADC_Globals.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "METER_AdcSample.h"


/*********************************************************************************
* Global Functions
*********************************************************************************/


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
static SemaphoreHandle_t ADC_TaskSem = NULL;
static uint8_t ADC_ConverterPhase = 0u;


void ADC_TaskHandler(void *pvParameters)
{
    ADC_TaskSem = xSemaphoreCreateBinary();

    if(ADC_TaskSem == NULL)
    {
        while(1);
    }

    for(;;)
    {
        xSemaphoreTake(ADC_TaskSem, portMAX_DELAY);
        {
            ADC_ConverterPhase++;
            
            /* some ADC result don't need to update every sample */ 
            switch(ADC_ConverterPhase)
            {
                 /* Battery and charger voltage */
                case 0u:
                {
                    /* 16.83V = 4096 */
                    //stAdcMeters.U_Batt_mV = DMA_AdcResult[ADC_U_BAT_POS] - DMA_AdcResult[ADC_U_BAT_NEG];
                    break;
                }

                case 1u:
                    
                    break;

                case 2u:
                    break;


                case 3u:
                    break;
                
                case 4u:
                    ADC_ConverterPhase = 0;
                    break;

                default:
                    ADC_ConverterPhase = 0;
                    break;
            }
        }
    }
}


void ADC_TaskTrigger(void)
{
    xSemaphoreGive(ADC_TaskSem);
}
