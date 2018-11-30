/**
  ******************************************************************************
  * @file    usbh_usr_lcd.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes the some user routines for LCD 
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
#include "usbh_usr_lcd.h"
#include "usb_conf.h"

/** @addtogroup USBH_USER
* @{
*/


/** @defgroup USBH_USR_LCD 
* @brief    This file includes the some user routines for LCD 
* @{
*/

/** @defgroup USBH_USR_LCD_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_LCD_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_LCD_Private_Macros
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_LCD_Private_Variables
* @{
*/
int16_t x_loc = 0, y_loc = 0;
int16_t prev_x = 0, prev_y = 0;
/**
* @}
*/

/** @defgroup USBH_USR_LCD_Exported_Variables
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_LCD_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_LCD_Private_Functions
* @{
*/

/**
* @brief  HID_MOUSE_UpdatePosition
*         The function is to handle mouse scroll to update the mouse position 
*         on display window
* @param   x : USB HID Mouse X co-ordinate
* @param   y :  USB HID Mouse Y co-ordinate
* @retval None
*/
void HID_MOUSE_UpdatePosition(int8_t x, int8_t y)
{
  /* Set the color for button press status */
  LCD_SetTextColor(LCD_COLOR_GREEN);
  LCD_SetBackColor(LCD_COLOR_BLACK);

  if ((x != 0) || (y != 0))
  {
    x_loc += x / 2;
    y_loc += y / 2;

    if (y_loc > MOUSE_WINDOW_HEIGHT - 15)
    {
      y_loc = MOUSE_WINDOW_HEIGHT - 15;
    }
    if (x_loc > MOUSE_WINDOW_WIDTH - 10)
    {
      x_loc = MOUSE_WINDOW_WIDTH - 10;
    }

    if (y_loc < 2)
    {
      y_loc = 2;
    }
    if (x_loc < 2)
    {
      x_loc = 2;
    }

#if defined(USE_STM324x9I_EVAL)
    LCD_SetBackColor(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_BLACK);

    LCD_DisplayChar(MOUSE_WINDOW_Y_POS + prev_y, MOUSE_WINDOW_X_POS - prev_x,
                    'x');


    LCD_SetTextColor(LCD_COLOR_GREEN);

    LCD_DisplayChar(MOUSE_WINDOW_Y_POS + y_loc, MOUSE_WINDOW_X_POS - x_loc,
                    'x');

#else
    if ((prev_x != 0) && (prev_y != 0))
    {
      LCD_DisplayChar(MOUSE_WINDOW_X + prev_y, MOUSE_WINDOW_Y - prev_x, ' ');
    }
    else if ((prev_x == 0) && (prev_y == 0))
    {

      LCD_DisplayChar(MOUSE_WINDOW_X + 1, MOUSE_WINDOW_Y - 1, ' ');
    }

    LCD_DisplayChar(MOUSE_WINDOW_X + y_loc, MOUSE_WINDOW_Y - x_loc, 'x');
#endif

    prev_x = x_loc;
    prev_y = y_loc;
  }
}

/**
* @brief  HID_MOUSE_ButtonPressed
*         The function is to handle mouse button press
* @param  button_idx : mouse button pressed
* @retval None
*/

void HID_MOUSE_ButtonPressed(uint8_t button_idx)
{

  /* Set the color for button press status */
  LCD_SetTextColor(LCD_COLOR_GREEN);
  LCD_SetBackColor(LCD_COLOR_GREEN);

  /* Change the color of button pressed to indicate button press */
  switch (button_idx)
  {
    /* Left Button Pressed */
  case 0:
#if defined(USE_STM324x9I_EVAL)
    LCD_DrawFullRect(HID_MOUSE_BUTTON1_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#else
    LCD_DrawFullRect(HID_MOUSE_BUTTON_XCHORD, HID_MOUSE_BUTTON1_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#endif

    break;


    /* Right Button Pressed */
  case 1:
#if defined(USE_STM324x9I_EVAL)
    LCD_DrawFullRect(HID_MOUSE_BUTTON3_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#else
    LCD_DrawFullRect(HID_MOUSE_BUTTON_XCHORD, HID_MOUSE_BUTTON2_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#endif

    break;

    /* Middle button Pressed */
  case 2:
#if defined(USE_STM324x9I_EVAL)
    LCD_DrawFullRect(HID_MOUSE_BUTTON2_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#else
    LCD_DrawFullRect(HID_MOUSE_BUTTON_XCHORD, HID_MOUSE_BUTTON3_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#endif

    break;
  }
}

/**
* @brief  HID_Mouse_ButtonReleased
*         The function is to handle mouse button release
* @param  button_idx : mouse button released
* @retval None
*/

void HID_MOUSE_ButtonReleased(uint8_t button_idx)
{
  /* Set the color for release status */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_WHITE);

  /* Change the color of button released to default button color */
  switch (button_idx)
  {
    /* Left Button Released */
  case 0:
#if defined(USE_STM324x9I_EVAL)
    LCD_DrawFullRect(HID_MOUSE_BUTTON1_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#else
    LCD_DrawFullRect(HID_MOUSE_BUTTON_XCHORD, HID_MOUSE_BUTTON1_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#endif
    break;

    /* Right Button Released */
  case 1:
#if defined(USE_STM324x9I_EVAL)
    LCD_DrawFullRect(HID_MOUSE_BUTTON3_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#else
    LCD_DrawFullRect(HID_MOUSE_BUTTON_XCHORD, HID_MOUSE_BUTTON2_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#endif
    break;

    /* Middle Button Released */
  case 2:
#if defined(USE_STM324x9I_EVAL)
    LCD_DrawFullRect(HID_MOUSE_BUTTON2_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#else
    LCD_DrawFullRect(HID_MOUSE_BUTTON_XCHORD, HID_MOUSE_BUTTON3_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
#endif
    break;
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
