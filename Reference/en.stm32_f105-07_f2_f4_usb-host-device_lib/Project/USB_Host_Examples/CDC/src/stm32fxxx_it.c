/**
  ******************************************************************************
  * @file    stm32fxxx_it.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes the interrupt handlers for the application
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "usb_bsp.h"
#include "usbh_usr.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#include "stm32fxxx_it.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
extern USB_OTG_CORE_HANDLE USB_OTG_Core;
extern USBH_HOST USB_Host;

/* Private function prototypes ----------------------------------------------- */
extern void USB_OTG_BSP_TimerIRQ(void);

/* Private functions --------------------------------------------------------- */

/******************************************************************************/
/* Cortex-M Processor Exceptions Handlers */
/******************************************************************************/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  EXTI1_IRQHandler
  *         This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    USB_Host.usr_cb->OverCurrentDetected();
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  MemManage_Handler
  *         This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  BusFault_Handler
  *         This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  UsageFault_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  SVC_Handler
  *         This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  DebugMon_Handler
  *         This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  PendSV_Handler
  *         This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  USB_OTG_BSP_TimerIRQ();
}


/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
  USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

/**
  * @brief  This function EXTI Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_STM3210C_EVAL
void EXTI15_10_IRQHandler(void)
#elif defined(USE_STM324x9I_EVAL)
void EXTI9_5_IRQHandler(void)
#else
void EXTI2_IRQHandler(void)
#endif
{
  uint32_t i = 0;
#if defined(USE_STM324x9I_EVAL)
  if (IOE16_GetITStatus())
  {
    for (i = 0; i < 500000; i++);
    CDC_DEMO_ProbeKey(IOE16_JoyStickGetState());

    /* Clear the interrupt pending bits */
    IOE_ClearGITPending(IOE_GIT_GPIO);
  }
  EXTI_ClearITPendingBit(IOE16_IT_EXTI_LINE);
#else

  if (IOE_GetGITStatus(IOE_2_ADDR, IOE_GIT_GPIO))
  {

    for (i = 0; i < 500000; i++);
    CDC_DEMO_ProbeKey(IOE_JoyStickGetState());

    /* Clear the interrupt pending bits */
    IOE_ClearGITPending(IOE_2_ADDR, IOE_GIT_GPIO);
    IOE_ClearIOITPending(IOE_2_ADDR, IOE_JOY_IT);
  }

  EXTI_ClearITPendingBit(IOE_IT_EXTI_LINE);
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
