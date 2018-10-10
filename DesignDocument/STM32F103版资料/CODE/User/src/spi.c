/****************************************************************
 Copyright (C),2014-2016,SZZKJ
 File name:		spi.c
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
/*Include files -----------------------------------------------*/
#include "spi.h"
#include "stm32f10x_gpio.h"
/*Defines -----------------------------------------------------*/


#define SPI_DLY()	SPI_Dly()

/*Extern data & funtion----------------------------------------*/

/*Global data -------------------------------------------------*/

/*datas & funcitons define area -------------------------------*/
void SPI_Dly(void)
{
	volatile int cnt;

	for (cnt = 0; cnt < 2; cnt++);
}

/*Functions ---------------------------------------------------*/
/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void SPI_Initial(void)
{
	#if 0
	#if 1
	SPI_SCK_INIT();
	SPI_SDO_INIT();
	SPI_SDI_INIT();
	SPI_CHN_LCD_CE_INIT();
	SPI_CHN_FLASH_CE_INIT();
	#else
	PORTCbits.RC3 = 1;
	TRISCbits.TRISC3=0;
	PORTCbits.RC5 = 1;
	TRISCbits.TRISC5=0;
	TRISCbits.TRISC4=1;
	PORTFbits.RF4 = 1;
	TRISFbits.TRISF4=0;
	PORTDbits.RD0 = 1;
	TRISDbits.TRISD0=0;
	#endif
	#else
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	 /*!< Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_13|GPIO_Pin_14;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	 
	 
	#endif
}

void SPI_Output(U8 Chn, U8 Data)
{
	U8 i;
	
	if (SPI_CHN_FLASH == Chn)
	{
		SPI_CHN_FLASH_CE_LOW();
		SPI_CHN_LCD_CE_HIGH();
	}
	else
	{
		SPI_CHN_FLASH_CE_HIGH();
		SPI_CHN_LCD_CE_LOW();
	}
	SPI_DLY();
	
	for (i = 0; i < 8; i++)
	{
		
		if (Data & (0x80>>i))
		{
			SPI_SDO_HIGH();
		}
		else
		{
			SPI_SDO_LOW();
		}
		SPI_SCK_LOW();
		SPI_DLY();
		SPI_SCK_HIGH();
		SPI_DLY();
		
	}
	SPI_SCK_LOW();
	SPI_CHN_FLASH_CE_HIGH();
	SPI_CHN_LCD_CE_HIGH();
	SPI_SDO_HIGH();	
}

/* end of file */


