/**
  ******************************************************************************
  * @file    stm32_it.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "stm32_it.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "def.h"

extern uint32_t b_rtc_int;
extern __IO uint8_t Flag_1ms;
extern __IO uint8_t Flag_10ms;
extern __IO uint8_t Flag_500ms;
extern __IO uint8_t Flag_1s;
void TimerRun(void);


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
    //TimingDelay--;
}

/******************************************************************************/
/*            STM32 Peripherals Interrupt Handlers                        */
/******************************************************************************/
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles EVAL_COM1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
  if (USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
  {
    /* Send the received data to the PC Host*/
    if (USART_GetFlagStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
	  {
	    /* Send the received data to the PC Host*/
	    //Rx_From_Uart();
	  }
  }

  if (USART_GetITStatus(EVAL_COM1, USART_IT_TXE) != RESET)
  {
    /* Send the received data to the PC Host*/
    if (USART_GetFlagStatus(EVAL_COM1, USART_IT_TXE) != RESET)
	  {
	    /* Send the received data to the PC Host*/
	   //Send_To_Uart();
	  }
  }

  /* If overrun condition occurs, clear the ORE flag and recover communication */
  if (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_ORE) != RESET)
  {
    (void)USART_ReceiveData(EVAL_COM1);
  }
}

void RTC_IRQHandler(void) 
{ 
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET) 
  { 
    /* Clear the RTC Second interrupt */ 
    RTC_ClearITPendingBit(RTC_IT_SEC); 
	b_rtc_int = 1;
    
  } 

	if(RTC_GetITStatus(RTC_IT_ALR)!=RESET)//查询是否为闹钟中断 
	 { 
		RTC_ClearITPendingBit(RTC_IT_ALR);//消除闹钟中断标识 

	} 
}

/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB Low Priority interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS) || defined(STM32F37X)
void USB_LP_IRQHandler(void)
#else
void USB_LP_CAN1_RX0_IRQHandler(void)
#endif
{
  USB_Istr();
}

#if defined(STM32F10X_HD) || defined(STM32F10X_XL) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{ 
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
  
}

void SD_SDIO_DMA_IRQHANDLER(void)
{
  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
  SD_ProcessDMAIRQ();
}

#endif /* STM32F10X_HD | STM32F10X_XL*/
/*******************************************************************************
* Function Name  : USB_FS_WKUP_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
void USB_FS_WKUP_IRQHandler(void)
#else
void USBWakeUp_IRQHandler(void)
#endif
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}

/******************************************************************************/
/*                 STM32 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32xxx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/
/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description	 : This function handles TIM1 UP interrupt request.
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void ADC1_2_IRQHandler(void)
{
	
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description	 : This function handles TIM1 UP interrupt request.
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
	//static uint32_t tmp = 0, tmp_flag = 1;
	static U8 TmCnt = 0;
	static U8 TmDiv = 0;

	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
	Flag_1ms = 1;
	TmDiv ++;
	if (TmDiv >= 10) 
	{
		TmDiv = 0; 
		Flag_10ms = 1;
		TmCnt ++; 
		if (TmCnt >= 50) 
		{
			TmCnt = 0; 
			Flag_500ms = 1; 
			Flag_1s = !Flag_1s;
			if (Flag_1s)
			{
				TimerRun();
			}
		}
	}

	
#if 0
	tmp++;
	if (tmp>=50)
	{
		tmp = 0;
		if (tmp_flag)
		{
			tmp_flag = 0;
			USB_NotConfigured_LED();
		}
		else
		{
			tmp_flag = 1;
			USB_Configured_LED();
		}
	}
#endif			
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
