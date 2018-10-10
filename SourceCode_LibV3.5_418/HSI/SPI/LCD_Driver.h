/******************************************************************************************************
*            LCD_Driver.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: LCD_Driver.h
*
*    DESCRIPTION: Header file for task define
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:48:19
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                      2018/5/24 21:48:23
******************************************************************************************************/

#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include "SPI_Globals.h"
/**********************************************************************************************
* Macros
**********************************************************************************************/

typedef struct
{
    uint16_t Unused: 7;
    
    uint16_t TB: 8;
        
    uint16_t CmdByte: 1;

}stLcdData_t;

/**********************************************************************************************
* Global Functions
**********************************************************************************************/
extern void LCD_WriteDataOrCmd(uint8_t byte, uint8_t dataFlag);

#endif
