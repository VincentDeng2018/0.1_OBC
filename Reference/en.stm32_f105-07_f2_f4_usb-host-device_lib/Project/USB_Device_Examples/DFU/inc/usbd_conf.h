/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   USB Device configuration file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/** @defgroup USB_CONF_Exported_Defines
  * @{
  */ 


#define USBD_CFG_MAX_NUM                1
#define USBD_ITF_MAX_NUM                MAX_USED_MEDIA
#define USB_MAX_STR_DESC_SIZ            200 
#define USB_SUPPORT_USER_STRING_DESC

#define USBD_SELF_POWERED               

/** @defgroup USB_DFU_Class_Layer_Parameter
  * @{
  */ 
#define XFERSIZE                        1024   /* Max DFU Packet Size   = 1024 bytes */


#define DFU_IN_EP                       0x80
#define DFU_OUT_EP                      0x00

#if !defined (STM32F10X_CL)
 /* Maximum number of supported media (Flash and OTP) */
 #define MAX_USED_MEDIA                  2
#else
 /* Maximum number of supported media (Flash) */
 #define MAX_USED_MEDIA                  1
#endif /* STM32F10X_CL */

/* Flash memory address from where user application will be loaded 
   This address represents the DFU code protected against write and erase operations.*/
#if !defined (STM32F10X_CL)
 #define APP_DEFAULT_ADD                 0x0800C000 /* The first 3 sectors (48 KB) are reserved for DFU code */
#else
 #define APP_DEFAULT_ADD                 0x08008000 /* The first 15 sectors (30 KB) are reserved for DFU code */
#endif /* STM32F10X_CL */

/* Uncomment this define to implement OTP memory interface */
#if !defined (STM32F10X_CL)
 #define DFU_MAL_SUPPORT_OTP
#endif /* STM32F2XX */

/* Uncomment this define to implement template memory interface  */
/* #define DFU_MAL_SUPPORT_MEM */

/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_CONF_Exported_Macros
  * @{
  */
/* Update this define to modify the DFU protected area.
   This area corresponds to the memory where the DFU code should be loaded
   and cannot be erased or overwritten by DFU application. */
#define DFU_MAL_IS_PROTECTED_AREA(add)    (uint8_t)(((add >= 0x08000000) && (add < (APP_DEFAULT_ADD)))? 1:0)

#define TRANSFER_SIZE_BYTES(sze)          ((uint8_t)(sze)), /* XFERSIZEB0 */\
                                          ((uint8_t)(sze >> 8)) /* XFERSIZEB1 */
/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 


#endif //__USBD_CONF__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

