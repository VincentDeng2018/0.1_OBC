/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "hw_config.h" 
#include "ff.h"
#include "ff_gen_drv.h"
#include "diskio.h"
#include "sd_diskio.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "spi.h"
#include "lcd.h"
#include "db.h"
#include "ctrl.h"
#include "display.h"



__IO uint8_t PrevXferComplete = 1;
__IO uint8_t Flag_1ms = 0;
__IO uint8_t Flag_10ms = 0;
__IO uint8_t Flag_500ms = 0;
__IO uint8_t Flag_1s = 0;



uint32_t tmp = 0;
uint32_t tmp1 = 0;
uint32_t tmp_flag=1;
extern uint8_t b_usb;

extern uint16_t MAL_Init (uint8_t lun);



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*Functions ---------------------------------------------------*/
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
	Set_System();
	Led_Config();
	Interrupts_Config();
	TM_Config();
	RTC_init();
	//USART_Config_Default();
	USB_Set();
	SPI_Initial();
	LcdInit();
	IWDG_Config();
	IWDG_ReloadCounter();
	Ctrl_Init();
	PowerOnProc();
	
	

	for (;;)
	{
		if (Flag_1ms)
		{
			Flag_1ms = 0;
			Communicate();
		}
		
		if (Flag_10ms)
		{
			Flag_10ms = 0;
			IWDG_ReloadCounter();
			Ctrl_ADC();
			KeyScan();
			KeyFilt();
			KeyProc();
			GetInputState();
			LoadRefresh();
			Display();
			LEDDisp();
			
			
		}

		if (Flag_500ms)
		{
			Flag_500ms = 0;
			Ctrl();
			CommunicateWDG();
		}
	
	}
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
*                  
* Output         : None
* Return         : None
*******************************************************************************/



#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
