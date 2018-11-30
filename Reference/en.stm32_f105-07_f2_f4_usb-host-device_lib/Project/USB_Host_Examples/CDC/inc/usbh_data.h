/**
  ******************************************************************************
  * @file    usbh_data.h
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file is the header file for usb data file
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
#ifndef __USBH_DATA__
#define __USBH_DATA__

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usb_conf.h"
#include <stdio.h>

struct fsdata_file {
  const struct fsdata_file *next;
  const uint8_t *name;
  const uint8_t *data;
  const uint16_t len;
};

struct fsdata_file_noconst {
  struct fsdata_file *next;
  uint8_t *name;
  uint8_t *data;
  unsigned long len;
};



extern const uint8_t Stored_File1[368];
extern const uint8_t Stored_File2[5603];
extern const uint8_t Stored_File3[800] ;

static const struct fsdata_file file_text3[] = {{NULL, (uint8_t*)"file3.txt",(uint8_t*)Stored_File3, sizeof(Stored_File3)}};
static const struct fsdata_file file_text2[] = {{file_text3, (uint8_t*)"file2.txt",(uint8_t*)Stored_File2, sizeof(Stored_File2)}};
static const struct fsdata_file file_text1[] = {{file_text2, (uint8_t*)"file1.txt",(uint8_t*)Stored_File1, sizeof(Stored_File1)}};

#define FS_ROOT   file_text1

#endif /* __USBH_DATA__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
