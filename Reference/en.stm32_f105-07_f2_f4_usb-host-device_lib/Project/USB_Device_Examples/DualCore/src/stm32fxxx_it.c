/**
  ******************************************************************************
  * @file    stm32fxxx_it.c 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and peripherals interrupt
  *          service routine.
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
#include "stm32fxxx_it.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define CURSOR_STEP     10

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */


/* Private function prototypes ----------------------------------------------- */
extern USB_OTG_CORE_HANDLE USB_OTG_FS_dev;
extern USB_OTG_CORE_HANDLE USB_OTG_HS_dev;
static uint8_t *USBD_HID_GetPos(void);
extern uint32_t USBD_OTG_ISR_Handler(USB_OTG_CORE_HANDLE * pdev);
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
extern uint32_t USBD_OTG_EP1IN_ISR_Handler(USB_OTG_CORE_HANDLE * pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler(USB_OTG_CORE_HANDLE * pdev);
#endif

/* Private functions --------------------------------------------------------- */

/******************************************************************************/
/* Cortex-M Processor Exceptions Handlers */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
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
  * @brief  This function handles Memory Manage exception.
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
  * @brief  This function handles Bus Fault exception.
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
  * @brief  This function handles Usage Fault exception.
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  static __IO uint32_t counter = 0;
  uint8_t *buf;

  /* check Joystick state every polling interval (10ms) */
  if (counter++ == USBD_HID_GetPollingInterval(&USB_OTG_FS_dev))
  {
    buf = USBD_HID_GetPos();
    if ((buf[1] != 0) || (buf[2] != 0))
    {
      /* send data though IN endpoint */
      USBD_HID_SendReport(&USB_OTG_FS_dev, buf, 4);
    }
    counter = 0;
  }
}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler(&USB_OTG_HS_dev);
}

void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler(&USB_OTG_FS_dev);
}

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/**
  * @brief  This function handles EP1_IN Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
  USBD_OTG_EP1IN_ISR_Handler(&USB_OTG_HS_dev);
}

/**
  * @brief  This function handles EP1_OUT Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
  USBD_OTG_EP1OUT_ISR_Handler(&USB_OTG_HS_dev);
}
#endif

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/**
* @brief  This function handles EXTI15_10_IRQ Handler.
* @param  None
* @retval None
*/
void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
  {

    if (USB_OTG_FS_dev.dev.DevRemoteWakeup)
    {
      if ((USB_OTG_FS_dev.cfg.low_power) &&
          (USB_OTG_FS_dev.dev.device_status == USB_OTG_SUSPENDED))
      {

        /* Reset SLEEPDEEP and SLEEPONEXIT bits */
        SCB->SCR &=
          (uint32_t) ~
          ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

        /* After wake-up from sleep mode, reconfigure the system clock */
        /* After wake-up from STOP reconfigure the system clock */

        /* Enable HSE */
        RCC_HSEConfig(RCC_HSE_ON);

        /* Wait till HSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
        {
        }

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }

        USB_OTG_UngateClock(&USB_OTG_FS_dev);
      }

      USB_OTG_ActiveRemoteWakeup(&USB_OTG_FS_dev);
      USB_OTG_FS_dev.dev.device_status = USB_OTG_FS_dev.dev.device_old_status;
      LCD_UsrLog("> USB Device woke up.\n");
    }
    /* Clear the EXTI line pending bit */
    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
  }
}

/**
  * @brief  This function handles EXTI15_10_IRQ Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_FS
void OTG_FS_WKUP_IRQHandler(void)
{
  if (USB_OTG_FS_dev.cfg.low_power)
  {
    /* Reset SLEEPDEEP and SLEEPONEXIT bits */
    SCB->SCR &=
      (uint32_t) ~
      ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

    /* After wake-up from sleep mode, reconfigure the system clock */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
    }

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }

    USB_OTG_UngateClock(&USB_OTG_FS_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}
#endif


/**
* @brief  USBD_HID_GetPos
* @param  None
* @retval Pointer to report
*/
static uint8_t *USBD_HID_GetPos(void)
{
  int8_t x = 0, y = 0;
  static uint8_t HID_Buffer[4];

#if defined(USE_STM324x9I_EVAL)
  switch (IOE16_JoyStickGetState())
#else
  switch (IOE_JoyStickGetState())
#endif
  {
  case JOY_LEFT:
    x -= CURSOR_STEP;
    break;
  case JOY_RIGHT:
    x += CURSOR_STEP;
    break;
  case JOY_UP:
    y -= CURSOR_STEP;
    break;
  case JOY_DOWN:
    y += CURSOR_STEP;
    break;

  case JOY_SEL:
    break;

  case JOY_NONE:
    break;
  }

  HID_Buffer[0] = 0;
  HID_Buffer[1] = x;
  HID_Buffer[2] = y;
  HID_Buffer[3] = 0;

  return HID_Buffer;
}

/******************************************************************************/
/* STM32F2xx Peripherals Interrupt Handlers */
/* Add here the Interrupt Handler for the used peripheral(s) (PPP), for the */
/* available peripheral interrupt handler's name please refer to the startup */
/* file (startup_stm32f2xx.s).  */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/* void PPP_IRQHandler(void) { } */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
