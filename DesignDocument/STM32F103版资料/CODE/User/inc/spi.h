/****************************************************************
 Copyright (C),2014-2016,SZZKJ
 File name:		spi.h
 Author:		WangFeng
 Version:		1.00
 Date:			2014-12-20
 Description:	
 
 Others:		
 
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 History
 ----------------------------------------------------------------
 Index:			1
 Date:			2014-12-20
 Author:		WangFeng
 Modification:	Create this file
****************************************************************/

#ifndef __SPI__H
#define	__SPI__H

#include "stm3210x_eval.h"

#define U8 unsigned char
#define BYTE U8
#define WORD unsigned short
#define UINT unsigned int



#ifdef __cplusplus
	extern "C" {
#endif

#define SPI_CHN_FLASH				0
#define SPI_CHN_LCD					1

#define SPI_SCK_LOW()				GPIOB->BRR = GPIO_Pin_13
#define SPI_SCK_HIGH()				GPIOB->BSRR = GPIO_Pin_13
#define SPI_SCK_INIT()				PORTBbits.RB13 = 1;TRISBbits.TRISB13=0
#define SPI_SDO_LOW()				GPIOB->BRR = GPIO_Pin_14
#define SPI_SDO_HIGH()				GPIOB->BSRR = GPIO_Pin_14
#define SPI_SDO_INIT()				PORTBbits.RB14 = 1;TRISBbits.TRISB14=0	


#define SPI_CHN_LCD_CE_LOW()		GPIOA->BRR = GPIO_Pin_6
#define SPI_CHN_LCD_CE_HIGH()		GPIOA->BSRR = GPIO_Pin_6
#define SPI_CHN_LCD_CE_INIT()		PORTAbits.RA6 = 1;TRISAbits.TRISA6=0

#define SPI_CHN_FLASH_CE_LOW()		
#define SPI_CHN_FLASH_CE_HIGH()		
#define SPI_CHN_FLASH_CE_INIT()		


void SPI_Initial(void);
void SPI_Output(U8 Chn, U8 Data);
void SPI_Dly(void);

#ifdef __cplusplus
}
#endif

#endif


/* end of file */


