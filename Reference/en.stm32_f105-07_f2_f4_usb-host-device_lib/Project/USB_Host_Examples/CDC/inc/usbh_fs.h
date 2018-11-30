/**
  ******************************************************************************
  * @file    usbh_fs.h
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file is the header file for usbh_fs file
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
#ifndef __USH_FS_H__
#define __USH_FS_H__


/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usb_conf.h"
#include <stdio.h>

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_CDC_USER_CALLBACKS
* @{
*/
  
/** @defgroup USBH_USR 
  * @brief This file is the header file for user action
  * @{
  */ 

/** @defgroup USBH_CORE_Exported_Variables
  * @{
  */ 

typedef enum {
  FS_SUCCESS = 0,
  FS_FAIL,
  FS_BUSY,
  FS_READ_FAIL,
  FS_WR_FAIL,
  FS_OPEN_FAIL
    
}FS_State;


struct fs_file {
  uint8_t *data;
  uint32_t len;
};
/**
  * @}
  */ 


/** @defgroup USBH_CORE_Exported_FunctionsPrototype
  * @{
  */ 
uint32_t Fs_Open(uint8_t *, struct fs_file *);
/**
  * @}
  */ 

#endif /* __USBH_FS_H */
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
