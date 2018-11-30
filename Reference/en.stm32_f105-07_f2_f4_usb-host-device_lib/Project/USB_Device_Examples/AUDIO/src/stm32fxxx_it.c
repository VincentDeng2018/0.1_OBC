/**
  ******************************************************************************
  * @file    stm32fxxx_it.c
  * @author  MCD Application Team
  * @version v1.2.1
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
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_audio_core.h"
#include "lcd_log.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler(USB_OTG_CORE_HANDLE * pdev);

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
extern uint32_t USBD_OTG_EP1IN_ISR_Handler(USB_OTG_CORE_HANDLE * pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler(USB_OTG_CORE_HANDLE * pdev);
#endif
extern AUDIO_FOPS_TypeDef AUDIO_OUT_fops;

uint8_t *USER_INFORMATION1[6] = {
  (uint8_t *) ">STATE: Application Idle        ",
  (uint8_t *) ">STATE: Application Active      ",
  (uint8_t *) ">STATE: PLAYING                 ",
  (uint8_t *) ">STATE: PAUSED                  ",
  (uint8_t *) ">STATE: STOPPED                 ",
  (uint8_t *) ">STATE: Application Error !     ",
};

uint8_t *USER_INFORMATION2[2] = {
  (uint8_t *) ">Output: HEADPHONE                  ",
  (uint8_t *) ">Output: SPEAKER                    "
};

uint32_t PrevState = 0xFF;
uint32_t PrevOutput = OUTPUT_DEVICE_HEADPHONE;
uint32_t OutputState = OUTPUT_DEVICE_HEADPHONE;
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
  /* Check if the audio player state has changed */
  if (PrevState != AUDIO_OUT_fops.GetState())
  {
    /* Information panel */
    LCD_SetTextColor(Green);
    LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 48,
                          USER_INFORMATION1[AUDIO_OUT_fops.GetState()]);
    LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);

    /* Update the previous state variable */
    PrevState = AUDIO_OUT_fops.GetState();
  }
#if !defined(USE_STM324x9I_EVAL)
  if (OutputState != PrevOutput)
  {

    Codec_SwitchOutput(OutputState);

    PrevOutput = OutputState;

    /* Information panel */
    LCD_SetTextColor(Green);
    if (OutputState == OUTPUT_DEVICE_HEADPHONE)
    {
      LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 32, USER_INFORMATION2[0]);
    }
    else
    {
      LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 32, USER_INFORMATION2[1]);
    }
    LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);
  }
#endif
}

/**
  * @brief  This function handles OTG HS or FS Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_HS
void OTG_HS_IRQHandler(void)
#else
void OTG_FS_IRQHandler(void)
#endif                          /* USE_USB_OTG_HS */
{
  USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/**
  * @brief  This function handles EP1_IN Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
  USBD_OTG_EP1IN_ISR_Handler(&USB_OTG_dev);
}

/**
  * @brief  This function handles EP1_OUT Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
  USBD_OTG_EP1OUT_ISR_Handler(&USB_OTG_dev);
}
#endif                          /* USB_OTG_HS_DEDICATED_EP1_ENABLED */

/**
  * @brief  This function handles EXTI15_10 or EXTI9_5 interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_STM3210C_EVAL
void EXTI9_5_IRQHandler(void)
#else
void EXTI15_10_IRQHandler(void)
#endif
{
  static uint8_t FallingEdge = 0;

  /* Check if the interrupt source is the KEY EXTI line */
  if (EXTI_GetFlagStatus(KEY_BUTTON_EXTI_LINE) == SET)
  {
    /* Check if the push button is pushed or released (to ignore the button
     * released event) */
    if (FallingEdge == 0)
    {
      /* Switch the output target between Speaker and Headphone */
      OutputState = (OutputState == OUTPUT_DEVICE_SPEAKER) ?
        OUTPUT_DEVICE_HEADPHONE : OUTPUT_DEVICE_SPEAKER;

      /* Increment the variable to indicate that next interrupt corresponds to
       * push button released event: Release event should be ignored */
      FallingEdge++;
    }
    else
    {
      /* Reset the variable to indicate that the next interrupt corresponds to
       * push button pushed event. */
      FallingEdge = 0;
    }

    /* Clear the interrupt line flag */
    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
  }
}

/******************************************************************************/
/* STM32Fxxx Peripherals Interrupt Handlers */
/* Add here the Interrupt Handler for the used peripheral(s) (PPP), for the */
/* available peripheral interrupt handler's name please refer to the startup */
/* file (startup_stm32fxxx.s).  */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/* void PPP_IRQHandler(void) { } */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
