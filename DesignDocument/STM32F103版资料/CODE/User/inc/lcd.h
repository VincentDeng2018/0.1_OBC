/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		lcd.h
 Author:		WangFeng
 Version:		1.00
 Date:			2014-11-8
 Description:	
 
 Others:		
 
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 History
 ----------------------------------------------------------------
 Index:			1
 Date:			2014-11-8
 Author:		WangFeng
 Modification:	Create this file
****************************************************************/

#ifndef __LCD_H
#define	__LCD_H

#include "stm3210x_eval.h"
#define U8 unsigned char
#define BYTE U8
#define WORD unsigned short
#define U16 WORD


#ifdef __cplusplus
	extern "C" {
#endif

#define LCD_CX					240
#define LCD_CY					128

#define LCD_DATA_X_MAX			((LCD_CX + 7) >> 3)
#define LCD_DATA_Y_MAX			(LCD_CY - 16)


#define LCD_PART_LEFT			0
#define LCD_PART_RIGHT			1

extern BYTE LcdPartIndex;
extern BYTE LcdData[LCD_DATA_Y_MAX][LCD_DATA_X_MAX>>1];

void LcdInit(void);
void LcdClr(void);
void LcdSet(void);
void LcdRefresh(WORD x, WORD y, WORD cx, WORD cy, BYTE *pData);
void delayms(U16 m);

#ifdef __cplusplus
}
#endif

#endif
/* end of file */

