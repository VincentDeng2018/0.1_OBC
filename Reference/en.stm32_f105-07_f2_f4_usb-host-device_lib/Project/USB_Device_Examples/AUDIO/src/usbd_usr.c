/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version v1.2.1
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
#if defined(USE_STM324x9I_EVAL)
static void PLLSAI_Config(void);
#endif
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

#define USER_INFORMATION1      (uint8_t*)">STATE: Application Idle            "
#define USER_INFORMATION2      (uint8_t*)"[Key]: Switch Headphone/Speaker      "
/**
* @brief  USBD_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{
  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

#if defined(USE_STM324x9I_EVAL)
  /* Configure PLLSAI */
  PLLSAI_Config();
#endif

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
#else
#error "Missing define: Evaluation board (ie. USE_STM322xG_EVAL)"
#endif

  LCD_LOG_Init();

#ifdef USE_USB_OTG_HS
  #ifdef USE_EMBEDDED_PHY
  LCD_LOG_SetHeader((uint8_t *) " USB OTG HS_IN_FS AUDIO Device");
  #else
  LCD_LOG_SetHeader((uint8_t *) " USB OTG HS AUDIO Device");
  #endif
#else
  LCD_LOG_SetHeader((uint8_t *) " USB OTG FS AUDIO Device");
#endif
  LCD_UsrLog("> USB device library started.\n");
  LCD_LOG_SetFooter((uint8_t *) "     USB Device Library v1.2.1");

  /* Information panel */
  LCD_SetTextColor(Green);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 48, USER_INFORMATION1);
  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);
#if !defined(USE_STM324x9I_EVAL)
  /* Information panel */
  LCD_SetTextColor(Green);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 32, USER_INFORMATION2);
  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);
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
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library v1.2.1 [HS]");
    break;

  case USB_OTG_SPEED_FULL:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library v1.2.1 [FS]");
    break;
  default:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library v1.2.1 [??]");

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
  LCD_UsrLog("> AUDIO Interface configured.\n");
}

/**
* @brief  USBD_USR_DeviceSuspended 
*         Displays the message on LCD on device suspend Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
  LCD_UsrLog("> USB Device in Suspend Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBD_USR_DeviceResumed 
*         Displays the message on LCD on device resume Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{
  LCD_UsrLog("> USB Device in Idle Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}

/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConnected(void)
{
  LCD_UsrLog("> USB Device Connected.\n");
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceDisconnected(void)
{
  LCD_UsrLog("> USB Device Disconnected.\n");
  EVAL_AUDIO_Stop(CODEC_PDWN_SW);
}



/**
  * @brief  EVAL_AUDIO_TransferComplete_CallBack
  * @param  None
  * @retval None
  */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
}

#if !defined(USE_STM324x9I_EVAL)
/**
  * @brief  Codec_TIMEOUT_UserCallback
  * @param  None
  * @retval None
  */
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  LCD_ErrLog("> I2C Timeout  (CS43L22)\n");

  I2C_GenerateSTOP(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, DISABLE);

  I2C_DeInit(I2C1);

  /* CODEC_I2C peripheral configuration */
  I2C_DeInit(I2C1);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x33;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  /* Enable the I2C peripheral */
  I2C_Cmd(I2C1, ENABLE);
  I2C_Init(I2C1, &I2C_InitStructure);

  LCD_UsrLog("> I2C error recovered.\n");

  return 0;
}

#endif
#if defined(USE_STM324x9I_EVAL)
/**
  * @brief  Configure PLLSAI.
  * @param  None
  * @retval None
  */
static void PLLSAI_Config(void)
{
  /* Configure PLLI2S prescalers */
  /* PLLI2S_VCO : VCO_344M */
  /* SAI_CLK(first level) = PLLI2S_VCO/PLLI2SQ = 344/7 = 49.142 Mhz */
  RCC_PLLI2SConfig(344, 7, 4);

  /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ = 49.142/1 = 49.142 Mhz */
  RCC_SAIPLLI2SClkDivConfig(1);

  /* Configure Clock source for SAI Block A */
  RCC_SAIBlockACLKConfig(RCC_SAIACLKSource_PLLSAI);

  /* Configure Clock source for SAI Block B */
  RCC_SAIBlockBCLKConfig(RCC_SAIBCLKSource_PLLI2S);

  /* Enable PLLSAI Clock */
  RCC_PLLSAICmd(ENABLE);

  /* Wait till PLLSAI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }

  /* Enable PLLI2S Clock */
  RCC_PLLI2SCmd(ENABLE);

  /* Wait till PLLI2S is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY) == RESET)
  {
  }
}

#endif

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
