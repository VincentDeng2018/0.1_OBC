/**
  ******************************************************************************
  * @file    USBD_usr.c
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
#include "usbd_ioreq.h"
#include "lcd_log.h"

#include "usb_conf.h"

/** @addtogroup USBD_USER
* @{
*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small LCD_UsrLog (option LD Linker->Libraries->Small
 * LCD_UsrLog set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif                          /* __GNUC__ */

/** @addtogroup USBD_MSC_DEMO_USER_cb
* @{
*/

/** @defgroup USBD_USR 
* @brief    This file includes the user application layer
* @{
*/

/** @defgroup USBD_USR_FS_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_FS_Private_Defines
* @{
*/

/**
* @}
*/


/** @defgroup USBD_USR_FS_Private_Macros
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_FS_Private_Variables
* @{
*/
/* Points to the DEVICE_PROP structure of current device */
/* The purpose of this register is to speed up the execution */

USBD_Usr_cb_TypeDef USR_FS_cb = {
  USBD_USR_FS_Init,
  USBD_USR_FS_DeviceReset,
  USBD_USR_FS_DeviceConfigured,
  USBD_USR_FS_DeviceSuspended,
  USBD_USR_FS_DeviceResumed,

  USBD_USR_FS_DeviceConnected,
  USBD_USR_FS_DeviceDisconnected,
};

USBD_Usr_cb_TypeDef USR_HS_cb = {
  USBD_USR_HS_Init,
  USBD_USR_HS_DeviceReset,
  USBD_USR_HS_DeviceConfigured,
  USBD_USR_HS_DeviceSuspended,
  USBD_USR_HS_DeviceResumed,

  USBD_USR_HS_DeviceConnected,
  USBD_USR_HS_DeviceDisconnected,
};

/**
* @}
*/

/** @defgroup USBD_USR_FS_Private_Constants
* @{
*/

/**
* @}
*/



/** @defgroup USBD_USR_FS_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_FS_Private_Functions
* @{
*/

#define USER_INFORMATION1  (uint8_t*)"MSC running on High speed."
#define USER_INFORMATION2  (uint8_t*)"HID running on Full speed."

/**
* @brief  USBD_USR_FS_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBD_USR_FS_Init(void)
{

  /* Configure the IOE on which the JoyStick is connected */
#if defined(USE_STM324x9I_EVAL)
  IOE16_Config();
#else
  IOE_Config();
#endif

  /* Setup SysTick Timer for 1 msec interrupts This interrupt is used to probe 
   * the joystick */
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */
    while (1);
  }

  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

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

#else
#error "Missing define: Evaluation board (ie. USE_STM322xG_EVAL)"
#endif

  LCD_LOG_Init();

  LCD_LOG_SetHeader((uint8_t *) "USB Dual Devices");
  LCD_UsrLog("> USB device library started.\n");
  LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1");

  /* Information panel */
  LCD_SetTextColor(Green);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 42, USER_INFORMATION1);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 30, USER_INFORMATION2);
  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);

}

/**
* @brief  USBD_USR_FS_DeviceReset 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBD_USR_FS_DeviceReset(uint8_t speed)
{
}


/**
* @brief  USBD_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Status
*/
void USBD_USR_FS_DeviceConfigured(void)
{
  LCD_UsrLog("> HID Interface started.\n");
}

/**
* @brief  USBD_USR_FS_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBD_USR_FS_DeviceSuspended(void)
{
  LCD_UsrLog("> HID Device in Suspend Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBD_USR_FS_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBD_USR_FS_DeviceResumed(void)
{
  /* Users can do their application actions here for the USB-Reset */
}

/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Status
*/
void USBD_USR_FS_DeviceConnected(void)
{
  LCD_UsrLog("> USB FS Device Connected.\n");
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Status
*/
void USBD_USR_FS_DeviceDisconnected(void)
{
  LCD_UsrLog("> USB FS Device Disconnected.\n");
}

/*****************************************************************************/
/**
* @brief  USBD_USR_FS_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBD_USR_HS_Init(void)
{
}

/**
* @brief  USBD_USR_FS_DeviceReset 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBD_USR_HS_DeviceReset(uint8_t speed)
{
  switch (speed)
  {
  case USB_OTG_SPEED_HIGH:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1   [HS]");
    break;

  case USB_OTG_SPEED_FULL:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1   [FS]");
    break;
  default:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1   [??]");
  }
}


/**
* @brief  USBD_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Status
*/
void USBD_USR_HS_DeviceConfigured(void)
{
  LCD_UsrLog("> MSC Interface started.\n");
}

/**
* @brief  USBD_USR_FS_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBD_USR_HS_DeviceSuspended(void)
{
  LCD_UsrLog("> MSC Device in Suspend Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBD_USR_FS_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBD_USR_HS_DeviceResumed(void)
{
  /* Users can do their application actions here for the USB-Reset */
}

/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Status
*/
void USBD_USR_HS_DeviceConnected(void)
{
  LCD_UsrLog("> USB HS Device Connected.\n");
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Status
*/
void USBD_USR_HS_DeviceDisconnected(void)
{
  LCD_UsrLog("> USB HS Device Disconnected.\n");
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
