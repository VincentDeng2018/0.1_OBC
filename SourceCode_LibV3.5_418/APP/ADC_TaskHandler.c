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
            /* some ADC result don't need to update every sample */
            switch(ADC_ConverterPhase++)
            {
                 /* Battery and charger voltage */
                case 0u:
                {
                    f_AdcFastFilter(DMA_AdcResult[ADC_KEY],    ADC_KEY, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_PACK], ADC_U_PACK, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_OUT],  ADC_U_OUT, 0);
                    if(DMA_AdcResult[ADC_U_BAT_POS] > DMA_AdcResult[ADC_U_BAT_NEG])
                    {
                        f_AdcFastFilter(DMA_AdcResult[ADC_U_BAT_POS] - DMA_AdcResult[ADC_U_BAT_NEG], ADC_U_BAT_POS, 0);
                    }
                    else
                    {
                        f_AdcFastFilter(0, ADC_U_BAT_POS, 0);
                    }
                    f_AdcFastFilter(DMA_AdcResult[ADC_I_CHG], ADC_I_CHG, ISampleAutoZero());
                    f_AdcFastFilter(DMA_AdcResult[ADC_TEMP1], ADC_TEMP1, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_TEMP2], ADC_TEMP2, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_I_SET], ADC_I_SET, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_C_SET], ADC_C_SET, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_SET_TEST], ADC_U_SET_TEST, 0);
                    break;
                }

                case 1u:
                    f_AdcFastFilter(DMA_AdcResult[ADC_I_CHG], ADC_I_CHG, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_OUT],  ADC_U_OUT, 0);
                    break;

                case 2u:
                    f_AdcFastFilter(DMA_AdcResult[ADC_KEY],    ADC_KEY, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_PACK], ADC_U_PACK, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_I_CHG], ADC_I_CHG, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_OUT],  ADC_U_OUT, 0);
                    break;


                case 3u:
                    f_AdcFastFilter(DMA_AdcResult[ADC_I_CHG], ADC_I_CHG, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_OUT],  ADC_U_OUT, 0);
                    break;

                case 4u:
                    f_AdcFastFilter(DMA_AdcResult[ADC_I_CHG], ADC_I_CHG, 0);
                    f_AdcFastFilter(DMA_AdcResult[ADC_U_OUT],  ADC_U_OUT, 0);
                    break;

                case 20:
                    ADC_ConverterPhase = 0;
                    break;
                default:
                    break;
            }
            xSemaphoreGive(ADC_TaskSem);
        }
    }
}


void ADC_TaskTrigger(void)
{
    xSemaphoreGive(ADC_TaskSem);
}