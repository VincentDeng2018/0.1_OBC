/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   This file includes the user application layer
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
#include "usbd_usr.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
* @{
*/

/** @defgroup USBD_USR 
* @brief    This file includes the user application layer
* @{
*/

/** @defgroup USBD_USR_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_Private_Macros
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_Private_Variables
* @{
*/

USBD_Usr_cb_TypeDef USR_cb = {
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,


  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};

/**
* @}
*/

/** @defgroup USBD_USR_Private_Constants
* @{
*/

/**
* @}
*/



/** @defgroup USBD_USR_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_Private_Functions
* @{
*/

/**
* @brief  USBD_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{
  /* Initialize the LCD */
#if defined (USE_STM322xG_EVAL)
  STM322xG_LCD_Init();
#elif defined(USE_STM324xG_EVAL)
  STM324xG_LCD_Init();

#elif defined(USE_STM324x9I_EVAL)

  LCD_Init();
  LCD_LayerInit();

  /* Enable The Display */
  LCD_DisplayOn();
  /* Connect the Output Buffer to LCD Background Layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  /* Clear the Background Layer */
  LCD_Clear(LCD_COLOR_WHITE);

#elif defined (USE_STM3210C_EVAL)
  STM3210C_LCD_Init();

#elif defined (USE_STM32446_EVAL)  || defined (USE_STM32469I_EVAL)
  /* Nothing */

#else
#error "Missing define: Evaluation board (ie. USE_STM322xG_EVAL)"
#endif
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
  LCD_LOG_Init();

#ifdef USE_USB_OTG_HS
  #ifdef USE_EMBEDDED_PHY
  LCD_LOG_SetHeader((uint8_t *) " USB OTG HS_IN_FS VCP Device");
  #else
  LCD_LOG_SetHeader((uint8_t *) " USB OTG HS VCP Device");
  #endif
#else
  LCD_LOG_SetHeader((uint8_t *) " USB OTG FS VCP Device");
#endif
  LCD_UsrLog("> USB device library started.\n");
  LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1");
#endif
}

/**
* @brief  USBD_USR_DeviceReset 
*         Displays the message on LCD on device Reset Event
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset(uint8_t speed)
{
  switch (speed)
  {
  case USB_OTG_SPEED_HIGH:
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1 [HS]");
#endif
    break;

  case USB_OTG_SPEED_FULL:
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1 [FS]");
#endif
    break;
  default:
#if ! defined (USE_STM32446_EVAL) & ! defined (USE_STM32469I_EVAL)
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1 [??]");
#endif
    break;
  }
}


/**
* @brief  USBD_USR_DeviceConfigured
*         Displays the message on LCD on device configuration Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConfigured(void)
{
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
  LCD_UsrLog("> VCP Interface configured.\n");
#endif
}

/**
* @brief  USBD_USR_DeviceSuspended 
*         Displays the message on LCD on device suspend Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
  LCD_UsrLog("> USB Device in Suspend Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
#endif
}


/**
* @brief  USBD_USR_DeviceResumed 
*         Displays the message on LCD on device resume Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
  LCD_UsrLog("> USB Device in Idle Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
#endif
}


/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConnected(void)
{
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
  LCD_UsrLog("> USB Device Connected.\n");
#endif
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceDisconnected(void)
{
#if ! defined (USE_STM32446_EVAL) && ! defined (USE_STM32469I_EVAL)
  LCD_UsrLog("> USB Device Disconnected.\n");
#endif
}

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
