/**
  ******************************************************************************
  * @file    usbh_fs.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes File read management function.
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
#include "usbh_fs.h"
#include "usbh_data.h"
#include <string.h>

/** @addtogroup USBH_FS
* @{
*/

/** @addtogroup USBH_CDC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_FS
* @brief This file includes File read management function.
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

/** @defgroup USBH_FS 
* @brief    This file includes the user application layer
* @{
*/

/** @defgroup USBH_CORE_Exported_Types
* @{
*/


/** @defgroup USBH_FS_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBH_FS_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USBH_FS_Private_Variables
* @{
*/


/**
* @}
*/

/** @defgroup USBH_FS_Private_Constants
* @{
*/

/**
* @}
*/



/** @defgroup USBH_FS_Private_FunctionPrototypes
* @{
*/

/**
* @}
*/


/** @defgroup USBH_FS_Private_Functions
* @{
*/
/**
* @brief  This function open the file 
* @param  name : name of the file 
* @param  file : this holds the filename and length of the file 
* @retval TRUE : file exist
          FALSE : file not exist  
*/
uint32_t Fs_Open(uint8_t * name, struct fs_file *file)
{
  struct fsdata_file_noconst *f;

  for (f = (struct fsdata_file_noconst *)FS_ROOT; f != NULL;
       f = (struct fsdata_file_noconst *)f->next)
  {
    if (!strcmp((char *)name, (char *)f->name))
    {
      file->data = f->data;
      file->len = f->len;
      return TRUE;
    }
  }
  return FALSE;
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
