/******************************************************************************************************
*            main.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: main.c
*
*    DESCRIPTION: Main function
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/7/30 20:55:21
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/7/30 20:55:30
******************************************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
/* Scheduler includes. */ 
#include "FreeRTOS.h"
#include "task.h"

#include "TSK_TaskCommonApi.h"
#include "GPIO_Globals.h"

extern void CAN_Initial(void);

int main(void)
{
    /* hardware initial */
    portDISABLE_INTERRUPTS();
    SystemInit();
    GPIO_Initial();
    CAN_Initial();

    /* disable interrupt until task create finish */
    TSK_CreateTasks();
    portENABLE_INTERRUPTS();

    /* Start the scheduler. */
    vTaskStartScheduler();

    /* Will only get here if there was not enough heap space to create the
    idle task. */
    return 0;

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
