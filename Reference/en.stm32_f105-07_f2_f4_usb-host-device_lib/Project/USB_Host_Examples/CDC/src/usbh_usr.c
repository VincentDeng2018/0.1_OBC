/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes the usb host library user callbacks
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
#include "lcd_log.h"
#include "usbh_usr.h"
#include "usbh_fs.h"
#include "usbh_data.h"
#include "usbh_cdc_core.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_CDC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file includes the usb host library user callbacks
* @{
*/


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/

/**
* @}
*/

/** @addtogroup USER
* @{
*/

/** @defgroup USBH_USR
* @brief    This file includes the user application layer
* @{
*/

/** @defgroup USBH_CORE_Exported_Types
* @{
*/



/** @defgroup USBH_USR_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USBH_USR_Private_Variables
* @{
*/
extern USB_OTG_CORE_HANDLE USB_OTG_Core;
extern USBH_HOST USB_Host;
extern CDC_LineCodingTypeDef CDC_GetLineCode;
extern CDC_LineCodingTypeDef CDC_SetLineCode;
extern CDC_Usercb_TypeDef UserCb;

uint8_t prev_select = 0;
uint8_t enable_display_received_data = 0;
CDC_DEMO_StateMachine cdc_demo;
CDC_DEMO_SETTING_StateMachine cdc_settings_state;
CDC_DEMO_SelectMode cdc_select_mode;

static uint32_t i = 0;
#if defined(USE_STM324x9I_EVAL)
static uint16_t refcolumn = 479;
#else
static uint16_t refcolumn = 320;
#endif

static uint8_t line = 48;

static uint8_t Prev_BaudRateIdx = 0;
static uint8_t Prev_DataBitsIdx = 0;
static uint8_t Prev_ParityIdx = 0;
static uint8_t Prev_StopBitsIdx = 0;
static uint8_t Prev_Select = 0;

uint8_t *CDC_main_menu[] = {
  (uint8_t *)
    "      1 - Send Data                                                          ",
  (uint8_t *)
    "      2 - Receive Data                                                       ",
  (uint8_t *)
    "      3 - Configuration                                                      ",
};

uint8_t *DEMO_SEND_menu[] = {
  (uint8_t *)
    "      1 - Send File 1                                                        ",
  (uint8_t *)
    "      2 - Send File 2                                                        ",
  (uint8_t *)
    "      3 - Send File 3                                                        ",
  (uint8_t *)
    "      4 - Return                                                             ",
};

uint8_t *DEMO_RECEIVE_menu[] = {
  (uint8_t *)
    "      1 - Return                                                             ",
  (uint8_t *)
    "                                                                             ",
  (uint8_t *)
    "                                                                             ",
};

uint8_t *DEMO_CONFIGURATION_menu[] = {
  (uint8_t *)
    "      1 - Apply new settings                                                 ",
  (uint8_t *)
    "      2 - Return                                                             ",
  (uint8_t *)
    "                                                                             ",
};

const uint8_t MSG_BITS_PER_SECOND[] =
  "               : Bit Per Second                          ";
const uint8_t MSG_DATA_BITS[] =
  "               : Data width                              ";
const uint8_t MSG_PARITY[] =
  "               : Parity                                  ";
const uint8_t MSG_STOP_BITS[] =
  "               : Stop Bits                               ";

const uint32_t BaudRateValue[NB_BAUD_RATES] =
  { 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 };

const uint8_t DataBitsValue[4] = { 5, 6, 7, 8 };
const uint8_t ParityArray[3][5] = { "NONE", "EVEN", "ODD" };
const uint8_t StopBitsArray[2][2] = { "1", "2" };

/* Points to the DEVICE_PROP structure of current device */
/* The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks = {
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages ---------------*/
uint8_t MSG_HOST_INIT[] = "> Host Library Initialized\n";
uint8_t MSG_DEV_ATTACHED[] = "> Device Attached\n";
uint8_t MSG_DEV_DISCONNECTED[] = "> Device Disconnected\n";
uint8_t MSG_DEV_ENUMERATED[] = "> Enumeration completed\n";
uint8_t MSG_DEV_HIGHSPEED[] = "> High speed device detected\n";
uint8_t MSG_DEV_FULLSPEED[] = "> Full speed device detected\n";
uint8_t MSG_DEV_LOWSPEED[] = "> Low speed device detected\n";
uint8_t MSG_DEV_ERROR[] = "> Device fault\n";
uint8_t MSG_CDC_CLASS[] = "> CDC device connected\n";
uint8_t MSG_UNREC_ERROR[] = "> Unrecovered error\n";
/**
* @}
*/



/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/
static void CDC_SelectItem(uint8_t ** menu, uint8_t item);
static void CDC_Handle_Menu(void);
static void CDC_Handle_Send_Menu(void);
static void CDC_Handle_Receive_Menu(void);
static void CDC_Handle_Configuration_Menu(void);
static void CDC_ChangeSelectMode(CDC_DEMO_SelectMode select_mode);
static void CDC_AdjustSettingMenu(CDC_DEMO_SETTING_StateMachine *
                                  settings_state);
static void CDC_SelectSettingsItem(uint8_t item);
static void CDC_OutputData(uint8_t * ptr);
static void CDC_SetInitialValue(void);

/** @defgroup USBH_USR_Private_Functions
* @{
*/





/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
  static uint8_t startup = 0;

  if (startup == 0)
  {
    startup = 1;
    /* Configure the LEDs */
    STM_EVAL_LEDInit(LED1);
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);

    /* Initialize the LCD */
#if defined (USE_STM322xG_EVAL)
    STM322xG_LCD_Init();
#elif defined(USE_STM324xG_EVAL)
    STM324xG_LCD_Init();
#elif defined (USE_STM3210C_EVAL)
    STM3210C_LCD_Init();
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
#ifdef USE_USB_OTG_HS
  #ifdef USE_EMBEDDED_PHY
  LCD_LOG_SetHeader((uint8_t *) " USB OTG HS_IN_FS CDC Host");
  #else
  LCD_LOG_SetHeader((uint8_t *) " USB OTG HS CDC Host");
  #endif
#else
    LCD_LOG_SetHeader((uint8_t *) " USB OTG FS CDC Host");
#endif
    LCD_UsrLog("> USB Host library started.\n");
    LCD_LOG_SetFooter((uint8_t *) "USB Host CDC Library v2.2.1");
  }
}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{

  LCD_UsrLog((void *)MSG_DEV_ATTACHED);

}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError(void)
{
  LCD_ErrLog((void *)MSG_UNREC_ERROR);
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected(void)
{

  LCD_SetBackColor(Black);
  LCD_SetTextColor(Black);

  LCD_LOG_ClearTextZone();
  LCD_ErrLog((void *)MSG_DEV_DISCONNECTED);
}

/**
* @brief  USBH_USR_ResetUSBDevice
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    LCD_UsrLog((void *)MSG_DEV_HIGHSPEED);
  }
  else if (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    LCD_UsrLog((void *)MSG_DEV_FULLSPEED);
  }
  else if (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    LCD_UsrLog((void *)MSG_DEV_LOWSPEED);
  }
  else
  {
    LCD_UsrLog((void *)MSG_DEV_ERROR);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;


  LCD_UsrLog("VID : %04luh\n", (uint32_t) (*hs).idVendor);
  LCD_UsrLog("PID : %04luh\n", (uint32_t) (*hs).idProduct);

}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{

}


/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef * itfDesc,
                                          USBH_EpDesc_TypeDef * epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;

  id = itfDesc;

  if ((*id).bInterfaceClass == COMMUNICATION_DEVICE_CLASS_CODE)
  {
    LCD_UsrLog((void *)MSG_CDC_CLASS);
  }
  else
  {
    LCD_UsrLog("USB Class NOT supported");
  }
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  LCD_UsrLog("Manufacturer : %s\n", (char *)ManufacturerString);
}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  LCD_UsrLog("Product : %s\n", (char *)ProductString);
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  LCD_UsrLog("Serial Number : %s\n", (char *)SerialNumString);
}

/**
* @brief  EnumerationDone
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
  /* Enumeration complete */
  LCD_UsrLog((void *)MSG_DEV_ENUMERATED);
  LCD_SetTextColor(Green);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 37,
                        (uint8_t *) "To start the CDC demonstration ");
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 25,
                        (uint8_t *) "Press Key...                       ");
  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);

  UserCb.Receive = CDC_OutputData;
}

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
  LCD_UsrLog("> Device not supported.\n");

}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{

  USBH_USR_Status usbh_usr_status;

  usbh_usr_status = USBH_USR_NO_RESP;

  /* Key B3 is in polling mode to detect user action */
  if (STM_EVAL_PBGetState(Button_KEY) == RESET)
  {
    LCD_LOG_ClearTextZone();
    CDC_SelectItem(CDC_main_menu, 0);
    LCD_UsrLog("> CDC Interface started.\n");
    LCD_UsrLog("> Waiting user input...\n");
    usbh_usr_status = USBH_USR_RESP_OK;
  }

  return usbh_usr_status;

}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected(void)
{
  LCD_ErrLog("Overcurrent detected.\n");

}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
}

/**
* @brief  USBH_USR_Application
*         CDC Main application
* @param  None
* @retval None
*/
int USBH_USR_Application(void)
{
  CDC_Handle_Menu();
  return 0;
}

/**
* @brief  Demo_ProbeKey
*         Probe the joystick state
* @param  state : joystick state
* @retval None
*/

void CDC_DEMO_ProbeKey(JOYState_TypeDef state)
{

  if (cdc_select_mode == CDC_SELECT_MENU)
  {
    /* Handle Menu inputs */
    if ((state == JOY_UP) && (cdc_demo.select > 0))
    {
      cdc_demo.select--;
    }
    else
      if (((cdc_demo.state == CDC_DEMO_WAIT) && (state == JOY_DOWN) &&
           (cdc_demo.select < 2)) || ((cdc_demo.state == CDC_DEMO_SEND) &&
                                      (state == JOY_DOWN) &&
                                      (cdc_demo.select < 3)) ||
          ((cdc_demo.state == CDC_DEMO_CONFIGURATION) && (state == JOY_DOWN) &&
           (cdc_demo.select < 1)))
    {
      cdc_demo.select++;
    }
    else if (state == JOY_SEL)
    {
      cdc_demo.select |= 0x80;
    }
  }
  else
  {
    /* Handle Configuration inputs */
    /**** Vertical selection ****/
    if ((state == JOY_UP) && (cdc_settings_state.select > 0))
    {
      cdc_settings_state.select--;
    }
    else if ((state == JOY_DOWN) && (cdc_settings_state.select < 3))
    {
      cdc_settings_state.select++;
    }
    /**** Horizontal selection ****/
    else if (state == JOY_RIGHT)
    {
      if ((cdc_settings_state.select == 0) &&
          (cdc_settings_state.settings.BaudRateIdx < 9))
      {
        cdc_settings_state.settings.BaudRateIdx++;
      }

      if ((cdc_settings_state.select == 1) &&
          (cdc_settings_state.settings.DataBitsIdx < 3))
      {
        cdc_settings_state.settings.DataBitsIdx++;
      }

      if ((cdc_settings_state.select == 2) &&
          (cdc_settings_state.settings.ParityIdx < 2))
      {
        cdc_settings_state.settings.ParityIdx++;
      }

      if ((cdc_settings_state.select == 3) &&
          (cdc_settings_state.settings.StopBitsIdx < 1))
      {
        cdc_settings_state.settings.StopBitsIdx++;
      }

    }
    else if (state == JOY_LEFT)
    {
      if ((cdc_settings_state.select == 0) &&
          (cdc_settings_state.settings.BaudRateIdx > 0))
      {
        cdc_settings_state.settings.BaudRateIdx--;
      }

      if ((cdc_settings_state.select == 1) &&
          (cdc_settings_state.settings.DataBitsIdx > 0))
      {
        cdc_settings_state.settings.DataBitsIdx--;
      }

      if ((cdc_settings_state.select == 2) &&
          (cdc_settings_state.settings.ParityIdx > 0))
      {
        cdc_settings_state.settings.ParityIdx--;
      }

      if ((cdc_settings_state.select == 3) &&
          (cdc_settings_state.settings.StopBitsIdx > 0))
      {
        cdc_settings_state.settings.StopBitsIdx--;
      }
    }
  }
}

/**
* @brief  Demo_SelectItem
*         manage the menu on the screen
* @param  menu : menu table
*         item : selected item to be highlighted
* @retval None
*/

static void CDC_SelectItem(uint8_t ** menu, uint8_t item)
{

  LCD_SetTextColor(White);

  switch (item)
  {
  case 0:
    LCD_SetBackColor(Red);
    LCD_DisplayStringLine(LINE_01, menu[0]);
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_02, menu[1]);
    LCD_DisplayStringLine(LINE_03, menu[2]);

    break;
  case 1:
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_01, menu[0]);
    LCD_SetBackColor(Red);
    LCD_DisplayStringLine(LINE_02, menu[1]);
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_03, menu[2]);

    break;
  case 2:
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_01, menu[0]);
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_02, menu[1]);
    LCD_SetBackColor(Red);
    LCD_DisplayStringLine(LINE_03, menu[2]);

    break;
  case 3:
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_01, menu[1]);
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_02, menu[2]);
    LCD_SetBackColor(Red);
    LCD_DisplayStringLine(LINE_03, menu[3]);

    break;

  case 4:
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_01, menu[2]);
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_02, menu[3]);
    LCD_SetBackColor(Red);
    LCD_DisplayStringLine(LINE_03, menu[4]);

    break;

  default:
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(LINE_01, menu[0]);
    LCD_DisplayStringLine(LINE_02, menu[1]);
    LCD_DisplayStringLine(LINE_03, menu[2]);
    break;
  }
  LCD_SetBackColor(Black);

}

/**
* @brief  Demo_Application
*         Demo state machine
* @param  None
* @retval None
*/

static void CDC_Handle_Menu(void)
{

  static uint8_t debounce = 0;

  switch (cdc_demo.state)
  {
  case CDC_DEMO_IDLE:
    CDC_SelectItem(CDC_main_menu, 0);
    cdc_demo.state = CDC_DEMO_WAIT;
    cdc_demo.select = 0;
    LCD_LOG_ClearTextZone();
    LCD_UsrLog("> Select an operation to start.\n");
    CDC_SetInitialValue();

    break;

  case CDC_DEMO_WAIT:

    if (cdc_demo.select != prev_select)
    {
      prev_select = cdc_demo.select;
      CDC_SelectItem(CDC_main_menu, cdc_demo.select & 0x7F);

      /* Handle select item */
      if (cdc_demo.select & 0x80)
      {
        cdc_demo.select &= 0x7F;

        switch (cdc_demo.select)
        {
        case 0:
          cdc_demo.state = CDC_DEMO_SEND;
          cdc_demo.Send_state = CDC_SEND_IDLE;
          break;

        case 1:
          cdc_demo.state = CDC_DEMO_RECEIVE;
          cdc_demo.Receive_state = CDC_RECEIVE_IDLE;
          break;

        case 2:
          cdc_demo.state = CDC_DEMO_CONFIGURATION;
          cdc_demo.Configuration_state = CDC_CONFIGURATION_IDLE;
          break;
        default:
          break;
        }
      }
    }
    break;

  case CDC_DEMO_SEND:
    CDC_Handle_Send_Menu();
    break;

  case CDC_DEMO_RECEIVE:
    CDC_Handle_Receive_Menu();
    break;
  case CDC_DEMO_CONFIGURATION:
    CDC_Handle_Configuration_Menu();
    break;

  default:
    break;
  }

  if (cdc_demo.state == CDC_DEMO_CONFIGURATION)
  {
    if (STM_EVAL_PBGetState(Button_KEY) == RESET)
    {
      if (debounce == 0)
      {
        if (cdc_select_mode == CDC_SELECT_MENU)
        {
          cdc_select_mode = CDC_SELECT_CONFIG;
        }
        else
        {
          cdc_select_mode = CDC_SELECT_MENU;
        }

        CDC_ChangeSelectMode(cdc_select_mode);
        debounce = 1;
      }
    }
    else
    {
      debounce = 0;
    }

    if (cdc_select_mode == CDC_SELECT_CONFIG)
    {
      CDC_AdjustSettingMenu(&cdc_settings_state);
    }

  }
}


/**
* @brief  Handle Send Menu
* @param  None
* @retval None
*/

static void CDC_Handle_Send_Menu(void)
{
  struct fs_file file = { 0, 0 };

  switch (cdc_demo.Send_state)
  {
  case CDC_SEND_IDLE:
    cdc_demo.Send_state = CDC_SEND_WAIT;
    CDC_SelectItem(DEMO_SEND_menu, 0);
    cdc_demo.select = 0;
    LCD_LOG_ClearTextZone();
    LCD_UsrLog("> Select a file to send.\n");
    break;

  case CDC_SEND_WAIT:
    if (cdc_demo.select != prev_select)
    {
      prev_select = cdc_demo.select;
      CDC_SelectItem(DEMO_SEND_menu, cdc_demo.select & 0x7F);

      /* Handle select item */
      if (cdc_demo.select & 0x80)
      {
        cdc_demo.select &= 0x7F;
        switch (cdc_demo.select)
        {
        case 0:
          Fs_Open((uint8_t *) "file1.txt", &file);
          CDC_SendData(file.data, file.len);
          LCD_UsrLog("> 'file1.txt' sent.\n");
          break;

        case 1:
          Fs_Open((uint8_t *) "file2.txt", &file);
          CDC_SendData(file.data, file.len);
          LCD_UsrLog("> 'file2.txt' sent.\n");
          break;
        case 2:
          Fs_Open((uint8_t *) "file3.txt", &file);
          CDC_SendData(file.data, file.len);
          LCD_UsrLog("> 'file3.txt' sent.\n");
          break;
        case 3:                /* Return */
          cdc_demo.state = CDC_DEMO_IDLE;
          cdc_demo.select = 0;
          break;
        default:
          break;
        }
      }
    }
    break;

  default:
    break;
  }
}

/**
* @brief  Handle Receive Menu
* @param  None
* @retval None
*/

static void CDC_Handle_Receive_Menu(void)
{
  switch (cdc_demo.Receive_state)
  {
  case CDC_SEND_IDLE:
    LCD_LOG_ClearTextZone();
    cdc_demo.Receive_state = CDC_RECEIVE_WAIT;
    CDC_SelectItem(DEMO_RECEIVE_menu, 0);
    LCD_UsrLog("> Received data (ASCII):\n");
    enable_display_received_data = 1;
    CDC_StartReception(&USB_OTG_Core);
    i = 0;
#if defined(USE_STM324x9I_EVAL)
    refcolumn = 479;
#else
    refcolumn = 320;
#endif
    line = 48;
    cdc_demo.select = 0;
    break;

  case CDC_SEND_WAIT:
    if (cdc_demo.select != prev_select)
    {
      prev_select = cdc_demo.select;
      CDC_SelectItem(DEMO_RECEIVE_menu, cdc_demo.select & 0x7F);
      /* Handle select item */
      if (cdc_demo.select & 0x80)
      {
        cdc_demo.select &= 0x7F;
        switch (cdc_demo.select)
        {
        case 0:                /* Return */
          cdc_demo.state = CDC_DEMO_IDLE;
          cdc_demo.select = 0;
          enable_display_received_data = 0;
          CDC_StopReception(&USB_OTG_Core);
          break;
        default:
          break;
        }
      }
    }
    break;

  default:
    break;
  }
}

/**
* @brief  Handle Configuration Menu
* @param  None
* @retval None
*/

static void CDC_Handle_Configuration_Menu(void)
{
  switch (cdc_demo.Configuration_state)
  {
  case CDC_CONFIGURATION_IDLE:
    LCD_LOG_ClearTextZone();
    LCD_DbgLog("[  Key   ]: Select menu set\n");
    LCD_DbgLog("[Joystick]: Change settings/Parse Menu\n");
    cdc_settings_state.select = 0;
    CDC_SelectSettingsItem(0xFF);
    cdc_demo.Configuration_state = CDC_CONFIGURATION_WAIT;
    CDC_SelectItem(DEMO_CONFIGURATION_menu, 0);
    cdc_demo.select = 0;

    break;

  case CDC_CONFIGURATION_WAIT:

    if (cdc_demo.select != prev_select)
    {
      prev_select = cdc_demo.select;
      CDC_SelectItem(DEMO_CONFIGURATION_menu, cdc_demo.select & 0x7F);
      /* Handle select item */
      if (cdc_demo.select & 0x80)
      {
        cdc_demo.select &= 0x7F;
        switch (cdc_demo.select)
        {
        case 0:
          CDC_ChangeStateToIssueSetConfig(&USB_OTG_Core, &USB_Host);

        case 1:                /* Return */
          cdc_demo.state = CDC_DEMO_IDLE;
          cdc_demo.select = 0;
          break;

        default:
          break;
        }
      }
    }
    break;

  default:
    break;
  }
}


/**
* @brief  CDC_ChangeSelectMode
*         change the selection mode
* @param  mode : selection mode
* @retval None
*/

static void CDC_ChangeSelectMode(CDC_DEMO_SelectMode select_mode)
{
  if (select_mode == CDC_SELECT_CONFIG)
  {
    CDC_SelectItem(DEMO_CONFIGURATION_menu, 0xFF);
    cdc_settings_state.select = 0;
    CDC_SelectSettingsItem(0);
  }
  else
  {
    CDC_SelectItem(DEMO_CONFIGURATION_menu, 0);
    CDC_SelectSettingsItem(0xFF);
  }
}

/**
* @brief  CDC_AdjustSettingMenu
*         manage the setting menu on the screen
* @param  item : selected item to be highlighted
* @retval None
*/

static void CDC_AdjustSettingMenu(CDC_DEMO_SETTING_StateMachine *
                                  settings_state)
{
  uint8_t StrTemp[40];

  LCD_SetBackColor(Red);

  if (settings_state->select != Prev_Select)
  {
    Prev_Select = settings_state->select;
    CDC_SelectSettingsItem(settings_state->select);
  }

  if (settings_state->settings.BaudRateIdx != Prev_BaudRateIdx)
  {

    Prev_BaudRateIdx = settings_state->settings.BaudRateIdx;

    if (BaudRateValue[settings_state->settings.BaudRateIdx] <= 9600)
      sprintf((char *)StrTemp, "         %lu",
              BaudRateValue[settings_state->settings.BaudRateIdx]);
    else if (BaudRateValue[settings_state->settings.BaudRateIdx] <= 57600)
      sprintf((char *)StrTemp, "        %lu",
              BaudRateValue[settings_state->settings.BaudRateIdx]);
    else
      sprintf((char *)StrTemp, "       %lu",
              BaudRateValue[settings_state->settings.BaudRateIdx]);

    LCD_DisplayStringLine(100, StrTemp);

    CDC_SetLineCode.b.dwDTERate =
      BaudRateValue[settings_state->settings.BaudRateIdx];
    CDC_GetLineCode.b.dwDTERate = CDC_SetLineCode.b.dwDTERate;
  }

  if (settings_state->settings.DataBitsIdx != Prev_DataBitsIdx)
  {
    Prev_DataBitsIdx = settings_state->settings.DataBitsIdx;
    sprintf((char *)StrTemp, "            %d",
            DataBitsValue[settings_state->settings.DataBitsIdx]);
    LCD_DisplayStringLine(112, StrTemp);
    CDC_SetLineCode.b.bDataBits =
      DataBitsValue[settings_state->settings.DataBitsIdx];
    CDC_GetLineCode.b.bDataBits = CDC_SetLineCode.b.bDataBits;
  }


  if (settings_state->settings.ParityIdx != Prev_ParityIdx)
  {
    Prev_ParityIdx = settings_state->settings.ParityIdx;
    if (settings_state->settings.ParityIdx == 2)
      sprintf((char *)StrTemp, "          %s",
              (uint8_t *) ParityArray[settings_state->settings.ParityIdx]);
    else
      sprintf((char *)StrTemp, "         %s",
              (uint8_t *) ParityArray[settings_state->settings.ParityIdx]);
    LCD_DisplayStringLine(124, StrTemp);
    CDC_SetLineCode.b.bParityType = settings_state->settings.ParityIdx;
    CDC_GetLineCode.b.bParityType = CDC_SetLineCode.b.bParityType;
  }


  if (settings_state->settings.StopBitsIdx != Prev_StopBitsIdx)
  {
    Prev_StopBitsIdx = settings_state->settings.StopBitsIdx;
    sprintf((char *)StrTemp, "            %s",
            StopBitsArray[settings_state->settings.StopBitsIdx]);
    LCD_DisplayStringLine(136, StrTemp);
    CDC_SetLineCode.b.bCharFormat = settings_state->settings.StopBitsIdx;
    CDC_GetLineCode.b.bCharFormat = CDC_SetLineCode.b.bCharFormat;
  }
  LCD_SetBackColor(Blue);
}


/**
* @brief  CDC_AdjustSettingMenu
*         manage the setting menu on the screen
* @param  item : selected item to be highlighted
* @retval None
*/

static void CDC_SelectSettingsItem(uint8_t item)
{
  uint8_t StrTemp[40];

  LCD_SetTextColor(White);

  switch (item)
  {
  case 0:
    {
      LCD_SetBackColor(Red);
      LCD_DisplayStringLine(100, (uint8_t *) MSG_BITS_PER_SECOND);
      if (CDC_GetLineCode.b.dwDTERate <= 9600)
        sprintf((char *)StrTemp, "         %lu", CDC_GetLineCode.b.dwDTERate);
      else if (CDC_GetLineCode.b.dwDTERate <= 57600)
        sprintf((char *)StrTemp, "        %lu", CDC_GetLineCode.b.dwDTERate);
      else
        sprintf((char *)StrTemp, "       %lu", CDC_GetLineCode.b.dwDTERate);
      LCD_DisplayStringLine(100, StrTemp);

      /* Display the data bits */
      LCD_SetBackColor(Blue);
      LCD_DisplayStringLine(112, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)StrTemp, "            %d", CDC_GetLineCode.b.bDataBits);
      LCD_DisplayStringLine(112, StrTemp);

      /* Display the parity bits */
      LCD_DisplayStringLine(124, (uint8_t *) MSG_PARITY);
      if (CDC_GetLineCode.b.bParityType == 2)
        sprintf((char *)StrTemp, "          %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      else
        sprintf((char *)StrTemp, "         %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      LCD_DisplayStringLine(124, StrTemp);

      /* Display the Stop bits */

      LCD_DisplayStringLine(136, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)StrTemp, "            %s",
              StopBitsArray[CDC_GetLineCode.b.bCharFormat]);
      LCD_DisplayStringLine(136, StrTemp);
    }
    break;

  case 1:
    {
      LCD_SetBackColor(Blue);
      LCD_DisplayStringLine(100, (uint8_t *) MSG_BITS_PER_SECOND);
      if (CDC_GetLineCode.b.dwDTERate <= 9600)
        sprintf((char *)StrTemp, "         %lu", CDC_GetLineCode.b.dwDTERate);
      else if (CDC_GetLineCode.b.dwDTERate <= 57600)
        sprintf((char *)StrTemp, "        %lu", CDC_GetLineCode.b.dwDTERate);
      else
        sprintf((char *)StrTemp, "       %lu", CDC_GetLineCode.b.dwDTERate);
      LCD_DisplayStringLine(100, StrTemp);
      LCD_DisplayStringLine(100, StrTemp);

      /* Display the data bits */
      LCD_SetBackColor(Red);
      LCD_DisplayStringLine(112, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)StrTemp, "            %d", CDC_GetLineCode.b.bDataBits);
      LCD_DisplayStringLine(112, StrTemp);


      /* Display the parity bits */
      LCD_SetBackColor(Blue);
      LCD_DisplayStringLine(124, (uint8_t *) MSG_PARITY);
      if (CDC_GetLineCode.b.bParityType == 2)
        sprintf((char *)StrTemp, "          %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      else
        sprintf((char *)StrTemp, "         %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      LCD_DisplayStringLine(124, StrTemp);

      /* Display the Stop bits */
      LCD_DisplayStringLine(136, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)StrTemp, "            %s",
              StopBitsArray[CDC_GetLineCode.b.bCharFormat]);
      LCD_DisplayStringLine(136, StrTemp);
    }
    break;



  case 2:
    {
      LCD_SetBackColor(Blue);
      LCD_DisplayStringLine(100, (uint8_t *) MSG_BITS_PER_SECOND);
      if (CDC_GetLineCode.b.dwDTERate <= 9600)
        sprintf((char *)StrTemp, "         %lu", CDC_GetLineCode.b.dwDTERate);
      else if (CDC_GetLineCode.b.dwDTERate <= 57600)
        sprintf((char *)StrTemp, "        %lu", CDC_GetLineCode.b.dwDTERate);
      else
        sprintf((char *)StrTemp, "       %lu", CDC_GetLineCode.b.dwDTERate);
      LCD_DisplayStringLine(100, StrTemp);

      /* Display the data bits */
      LCD_DisplayStringLine(112, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)StrTemp, "            %d", CDC_GetLineCode.b.bDataBits);
      LCD_DisplayStringLine(112, StrTemp);


      /* Display the parity bits */
      LCD_SetBackColor(Red);
      LCD_DisplayStringLine(124, (uint8_t *) MSG_PARITY);
      if (CDC_GetLineCode.b.bParityType == 2)
        sprintf((char *)StrTemp, "          %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      else
        sprintf((char *)StrTemp, "         %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      LCD_DisplayStringLine(124, StrTemp);

      /* Display the Stop bits */
      LCD_SetBackColor(Blue);
      LCD_DisplayStringLine(136, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)StrTemp, "            %s",
              StopBitsArray[CDC_GetLineCode.b.bCharFormat]);
      LCD_DisplayStringLine(136, StrTemp);

    }
    break;

  case 3:
    {
      LCD_SetBackColor(Blue);
      LCD_DisplayStringLine(100, (uint8_t *) MSG_BITS_PER_SECOND);
      if (CDC_GetLineCode.b.dwDTERate <= 9600)
        sprintf((char *)StrTemp, "         %lu", CDC_GetLineCode.b.dwDTERate);
      else if (CDC_GetLineCode.b.dwDTERate <= 57600)
        sprintf((char *)StrTemp, "        %lu", CDC_GetLineCode.b.dwDTERate);
      else
        sprintf((char *)StrTemp, "       %lu", CDC_GetLineCode.b.dwDTERate);
      LCD_DisplayStringLine(100, StrTemp);
      LCD_DisplayStringLine(100, StrTemp);

      /* Display the data bits */
      LCD_DisplayStringLine(112, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)StrTemp, "            %d", CDC_GetLineCode.b.bDataBits);
      LCD_DisplayStringLine(112, StrTemp);


      /* Display the parity bits */
      LCD_DisplayStringLine(124, (uint8_t *) MSG_PARITY);
      if (CDC_GetLineCode.b.bParityType == 2)
        sprintf((char *)StrTemp, "          %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      else
        sprintf((char *)StrTemp, "         %s",
                ParityArray[CDC_GetLineCode.b.bParityType]);
      LCD_DisplayStringLine(124, StrTemp);

      /* Display the Stop bits */
      LCD_SetBackColor(Red);
      LCD_DisplayStringLine(136, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)StrTemp, "            %s",
              StopBitsArray[CDC_GetLineCode.b.bCharFormat]);
      LCD_DisplayStringLine(136, StrTemp);

    }
    break;
  default:
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(100, (uint8_t *) MSG_BITS_PER_SECOND);
    if (CDC_GetLineCode.b.dwDTERate <= 9600)
      sprintf((char *)StrTemp, "         %lu", CDC_GetLineCode.b.dwDTERate);
    else if (CDC_GetLineCode.b.dwDTERate <= 57600)
      sprintf((char *)StrTemp, "        %lu", CDC_GetLineCode.b.dwDTERate);
    else
      sprintf((char *)StrTemp, "       %lu", CDC_GetLineCode.b.dwDTERate);
    LCD_DisplayStringLine(100, StrTemp);
    LCD_DisplayStringLine(100, StrTemp);

    /* Display the data bits */
    LCD_DisplayStringLine(112, (uint8_t *) MSG_DATA_BITS);
    sprintf((char *)StrTemp, "            %d", CDC_GetLineCode.b.bDataBits);
    LCD_DisplayStringLine(112, StrTemp);


    /* Display the parity bits */
    LCD_DisplayStringLine(124, (uint8_t *) MSG_PARITY);
    if (CDC_GetLineCode.b.bParityType == 2)
      sprintf((char *)StrTemp, "          %s",
              ParityArray[CDC_GetLineCode.b.bParityType]);
    else
      sprintf((char *)StrTemp, "         %s",
              ParityArray[CDC_GetLineCode.b.bParityType]);
    LCD_DisplayStringLine(124, StrTemp);

    /* Display the Stop bits */
    LCD_DisplayStringLine(136, (uint8_t *) MSG_STOP_BITS);
    sprintf((char *)StrTemp, "            %s",
            StopBitsArray[CDC_GetLineCode.b.bCharFormat]);
    LCD_DisplayStringLine(136, StrTemp);
    break;
  }
}

/**
* @brief  Displays a maximum of 40 small font char on the LCD.
* @param  ptr: pointer to string to display on LCD.
* @retval None
*/
static void CDC_OutputData(uint8_t * ptr)
{

  LCD_SetTextColor(Green);
  if (enable_display_received_data != 0)
  {
    do
    {
      /* Send the string character by character on lCD */

      /* Display one character on LCD */
      LCD_DisplayChar(line, refcolumn, *ptr);
      /* Decrement the column position by 16 */
      refcolumn -= 8;
      /* Point on the next character */
      ptr++;
      /* Increment the character counter */
      i++;

      if (i >= 38)
      {
        i = 0;
#if defined(USE_STM324x9I_EVAL)
        refcolumn = 479;
#else
        refcolumn = 320;
#endif
        line += 12;             /* change the line number */
        if (line > 168)
        {
          LCD_LOG_ClearTextZone();
          line = 48;
        }
      }

    } while ((*ptr != 0) & (*ptr != '\n') & (i < 40));
  }
  LCD_SetTextColor(White);


}


/**
* @brief  Set the cdc demo intial values
* @param  None
* @retval None
*/
static void CDC_SetInitialValue(void)
{

  /* Set the initial value */
  CDC_SetLineCode.b.dwDTERate = CDC_GetLineCode.b.dwDTERate;
  CDC_SetLineCode.b.bDataBits = CDC_GetLineCode.b.bDataBits;
  CDC_SetLineCode.b.bParityType = CDC_GetLineCode.b.bParityType;
  CDC_SetLineCode.b.bCharFormat = CDC_GetLineCode.b.bCharFormat;
  CDC_ChangeStateToIssueSetConfig(&USB_OTG_Core, &USB_Host);

  /* Initialize baud rate index accordingtly */
  switch (CDC_SetLineCode.b.dwDTERate)
  {
  case 2400:
    cdc_settings_state.settings.BaudRateIdx = 0;
    break;
  case 4800:
    cdc_settings_state.settings.BaudRateIdx = 1;
    break;

  case 9600:
    cdc_settings_state.settings.BaudRateIdx = 2;
    break;

  case 19200:
    cdc_settings_state.settings.BaudRateIdx = 3;
    break;

  case 38400:
    cdc_settings_state.settings.BaudRateIdx = 4;
    break;

  case 57600:
    cdc_settings_state.settings.BaudRateIdx = 5;
    break;

  case 115200:
    cdc_settings_state.settings.BaudRateIdx = 6;
    break;

  case 230400:
    cdc_settings_state.settings.BaudRateIdx = 7;
    break;

  case 460800:
    cdc_settings_state.settings.BaudRateIdx = 8;
    break;

  case 921600:
    cdc_settings_state.settings.BaudRateIdx = 9;
    break;
  default:
    break;
  }
  /* Initialize data bits index accordingtly */
  switch (CDC_SetLineCode.b.bDataBits)
  {
  case 5:
    cdc_settings_state.settings.DataBitsIdx = 0;
    break;
  case 6:
    cdc_settings_state.settings.DataBitsIdx = 1;
    break;

  case 7:
    cdc_settings_state.settings.DataBitsIdx = 2;
    break;

  case 8:
    cdc_settings_state.settings.DataBitsIdx = 3;
    break;
  default:
    break;
  }


  /* Initialize stop bits index accordingtly */
  switch (CDC_SetLineCode.b.bCharFormat)
  {
  case 1:
    cdc_settings_state.settings.StopBitsIdx = 0;
    break;
  case 2:
    cdc_settings_state.settings.StopBitsIdx = 1;
    break;
  default:
    break;
  }

  /* Initialize parity index accordingtly */
  switch (CDC_SetLineCode.b.bParityType)
  {
  case 0:
    cdc_settings_state.settings.ParityIdx = 0;
    break;
  case 1:
    cdc_settings_state.settings.ParityIdx = 1;
    break;

  case 2:
    cdc_settings_state.settings.ParityIdx = 2;
    break;

  default:
    break;
  }

  Prev_BaudRateIdx = cdc_settings_state.settings.BaudRateIdx;
  Prev_DataBitsIdx = cdc_settings_state.settings.DataBitsIdx;
  Prev_ParityIdx = cdc_settings_state.settings.StopBitsIdx;
  Prev_StopBitsIdx = cdc_settings_state.settings.ParityIdx;
  Prev_Select = 0;
}

/**
* @}h
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
