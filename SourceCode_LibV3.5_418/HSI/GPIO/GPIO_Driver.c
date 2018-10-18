/******************************************************************************************************
*            GPIO_Driver.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: GPIO_Driver.c
*
*    DESCRIPTION: Define task related API, array, etc
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:51:20
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/5/24 21:51:24
******************************************************************************************************/

#include "GPIO_Globals.h"


/*********************************************************************************
* Global Functions
*********************************************************************************/


/*********************************************************************************
* Task Call back Functions
*********************************************************************************/


/*******************************************************************************
* Macros
*******************************************************************************/


/*******************************************************************************
* Data Struct Define
*******************************************************************************/


/*******************************************************************************
* Local Variables
*******************************************************************************/


void GPIO_Initial(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                         | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    /************** Configure LCD pins: SCK, MISO and NSS ---------------------------------*/
    /* Confugure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN | SPI_SCK_PIN | SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
    
    /* reset pin */
    GPIO_InitStructure.GPIO_Pin = LCD_RESET_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RESET_PORT, &GPIO_InitStructure);
}
