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
uint8_t CTRL_RelayCmd = RELAY_OFF;
uint8_t CTRL_FanCmd = FAN_OFF;
uint8_t CTRL_OutRelayCmd = RELAY_OFF;
uint8_t CTRL_ChgPowerCmd = POWER_OFF;
uint8_t CTRL_MosCmd = RELAY_OFF;


/****************************************************************************
*
*  Function: GPIO_Initial
*
*  Purpose :    Initial all the GPIO
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   
*
****************************************************************************/
void GPIO_Initial(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                         | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    
    
    /* Pin1-PC13: AC Low signal */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Pin3 - 6: use for other function */
    
    /*************ADC AF pins *****************/
    /* Pin8-PC0-AdcTemp1, Pin9-PC1-AdcTemp2, Pin10-PC2-AdcUBatt, Pin11-PC3-AdcUOut,
       Pin24-PC4-AdcBatt+, Pin25-PC5-AdcBatt- */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
                                | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Pin14-PA0-AdcKey, Pin15-PA1-AdcISelected, Pin16-PA2-AdcChgSelected */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Pin26-PB0-AdcIChg, Pin27-PB1-TODO: for test? */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Pin17-PA3-CTRL_RelayCommand: 1-On, 0-Off */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /************** Configure LCD pins: SCK, MISO and NSS ---------------------------------*/
    /* Pin20-22 */
    /* Confugure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN | SPI_SCK_PIN | SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
    
    /* Pin57 reset pin */
    GPIO_InitStructure.GPIO_Pin = LCD_RESET_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RESET_PORT, &GPIO_InitStructure);
    
    /******************* LED control *******************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /****************** CAN pin initial in CAN_Driver.c ***************/
    
    /* Pin36-PB15-OutRelay */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Pin37-PC6-Charger Power On */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /*************** TODO: SD Card read & write interface ***********************/
    
    /* Pin41-PA8-CTRL_MosCmd */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    /* Pin42-PA9-Battery connect signal: indicate whether battery is connected */
    /* Pin43-PA10-Battery reverse connect signal: indicate whether battery is reverse connected */
    /* Pin46-PA103-AC off: indicate whether battery is reverse connected */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    /* Pin56-PB4-Fan on/off command */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    /***************** PWM pin initial ******************/
    /* Pin61-PB8-I_PWN, Pin62-PB9-U_PWM */    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
