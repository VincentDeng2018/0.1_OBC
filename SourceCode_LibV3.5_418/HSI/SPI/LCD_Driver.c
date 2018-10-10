/******************************************************************************************************
*            LCD_Driver.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: LCD_Driver.c
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

#include "LCD_Driver.h"
#include "GPIO_Globals.h"
#include "FreeRTOS.h"
#include "task.h"
/*********************************************************************************
* Global Functions
*********************************************************************************/


/*********************************************************************************
* Task Call back Functions
*********************************************************************************/


/*******************************************************************************
* Macros
*******************************************************************************/

#define Delayms(x)         vTaskDelay(x)

#define portNVIC_SYSTICK_CURRENT_VALUE_REG	( * ( ( volatile uint32_t * ) 0xe000e018 ) )
/*******************************************************************************
* Data Struct Define
*******************************************************************************/


/*******************************************************************************
* Local Variables
*******************************************************************************/
static void ResetPinHigh(void);
static void ResetPinLow(void);
static void CsPinHigh(void);
static void CsPinLow(void);


void LCD_Initial(void)
{
    //-----------------------------------ST7789V reset sequence------------------------------------//
    ResetPinHigh();
    Delayms(1); //Delay 1ms
    ResetPinLow();
    Delayms(10); //Delay 10ms
    ResetPinHigh();
    Delayms(120); //Delay 120ms

    //---------------------------------------------------------------------------------------------------//
    LCD_WriteDataOrCmd(0x11, 0);//Write command 0x11, out of sleep;
    Delayms(120); //Delay 120ms

    //--------------------------------------Display Setting------------------------------------------//
    /* memory data access control */
    LCD_WriteDataOrCmd(0x36, 0);//Write command 0x36;
    LCD_WriteDataOrCmd(0x0, 1);//Write data 0x00;

    LCD_WriteDataOrCmd(0x3a, 0);//Write command 0x3a;
    LCD_WriteDataOrCmd(0x05, 1);//Write data 0x05;

    LCD_WriteDataOrCmd(0x35, 0);//Write command 0x35;
    LCD_WriteDataOrCmd(0x0, 0);//Write data 0x00;

    //--------------------------------ST7789V Frame rate setting----------------------------------//
    LCD_WriteDataOrCmd(0xb2, 0);//Write command 0xb2;
    LCD_WriteDataOrCmd(0x0c, 1);//Write data 0x0c;
    LCD_WriteDataOrCmd(0x0c, 1);//Write data 0x0c;
    LCD_WriteDataOrCmd(0x0, 1);//Write data 0x00;
    LCD_WriteDataOrCmd(0x33, 1);//Write data 0x33;
    LCD_WriteDataOrCmd(0x33, 1);//Write data 0x33;

    LCD_WriteDataOrCmd(0xb7, 0);//Write command 0xb7;
    LCD_WriteDataOrCmd(0x35, 1);//Write data 0x35;

    //---------------------------------ST7789V Power setting--------------------------------------//
    LCD_WriteDataOrCmd(0xbb, 0);//Write command 0xbb;
    LCD_WriteDataOrCmd(0x1a, 1);//Write data 0x1a;

    LCD_WriteDataOrCmd(0xc0, 0);//Write command 0xc0;
    LCD_WriteDataOrCmd(0x2c, 1);//Write data 0x2c;

    LCD_WriteDataOrCmd(0xc2, 0);//Write command 0xc2;
    LCD_WriteDataOrCmd(0x01, 1);//Write data 0x01;

    LCD_WriteDataOrCmd(0xc3, 0);//Write command 0xc3;
    LCD_WriteDataOrCmd(0x11, 0);//Write data 0x0f;

    LCD_WriteDataOrCmd(0xc4, 0);//Write command 0xc4;
    LCD_WriteDataOrCmd(0x20, 1);//Write data 0x20;

    LCD_WriteDataOrCmd(0xc6, 0);//Write command 0xc6;
    LCD_WriteDataOrCmd(0xf, 1);//Write data 0x0f;

    LCD_WriteDataOrCmd(0xd0, 0);//Write command 0xd0;
    LCD_WriteDataOrCmd(0xa4, 1);//Write data 0xa4;
    LCD_WriteDataOrCmd(0xa1, 1);//Write data 0xa1;

    //--------------------------------ST7789V gamma setting---------------------------------------//
    LCD_WriteDataOrCmd(0xe0, 0);//Write command 0xe0;
    LCD_WriteDataOrCmd(0xd0, 1);//Write data 0xd0;
    LCD_WriteDataOrCmd(0x01, 1);//Write data 0x01;
    LCD_WriteDataOrCmd(0x08, 1);//Write data 0x08;
    LCD_WriteDataOrCmd(0x0f, 1);//Write data 0x0f;
    LCD_WriteDataOrCmd(0x11, 1);//Write data 0x11;
    LCD_WriteDataOrCmd(0x2a, 1);//Write data 0x2a;
    LCD_WriteDataOrCmd(0x36, 1);//Write data 0x36;
    LCD_WriteDataOrCmd(0x55, 1);//Write data 0x55;
    LCD_WriteDataOrCmd(0x44, 1);//Write data 0x44;
    LCD_WriteDataOrCmd(0x4a, 1);//Write data 0x4a;
    LCD_WriteDataOrCmd(0x2b, 1);//Write data 0x2b;
    LCD_WriteDataOrCmd(0x1c, 1);//Write data 0x1c;
    LCD_WriteDataOrCmd(0x26, 1);//Write data 0x26;
    LCD_WriteDataOrCmd(0x28, 1);//Write data 0x28;

    LCD_WriteDataOrCmd(0xe1, 0);//Write command 0xe1;
    LCD_WriteDataOrCmd(0xd0, 1);//Write data 0xd0;
    LCD_WriteDataOrCmd(0x02, 1);//Write data 0x02;
    LCD_WriteDataOrCmd(0x07, 1);//Write data 0x07;
    LCD_WriteDataOrCmd(0x0a, 1);//Write data 0x0a;
    LCD_WriteDataOrCmd(0x0b, 1);//Write data 0x0b;
    LCD_WriteDataOrCmd(0x18, 1);//Write data 0x18;
    LCD_WriteDataOrCmd(0x34, 1);//Write data 0x34;
    LCD_WriteDataOrCmd(0x43, 1);//Write data 0x43;
    LCD_WriteDataOrCmd(0x4a, 1);//Write data 0x4a;
    LCD_WriteDataOrCmd(0x2b, 1);//Write data 0x2b;
    LCD_WriteDataOrCmd(0x1b, 1);//Write data 0x1b;
    LCD_WriteDataOrCmd(0x1c, 1);//Write data 0x1c;
    LCD_WriteDataOrCmd(0x26, 1);//Write data 0x26;
    LCD_WriteDataOrCmd(0x28, 1);//Write data 0x28;


    LCD_WriteDataOrCmd(0x29, 0);//Write command 0x29;
    
    Delayms(10); //Delay 10ms
}

static void ResetPinHigh(void)
{
    GPIO_SetBits(LCD_RESET_PORT, LCD_RESET_PIN);
}


static void ResetPinLow(void)
{
    GPIO_ResetBits(LCD_RESET_PORT, LCD_RESET_PIN);
}

static void CsPinHigh(void)
{
    GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN);
}


static void CsPinLow(void)
{
    GPIO_ResetBits(SPI_CS_PORT, SPI_CS_PIN);
}


void LCD_WriteDataOrCmd(uint8_t byte, uint8_t dataFlag)
{
    uint32_t tickStartCnt = 0;
    uint16_t data = 0;

    stLcdData_t stLcdData =
    {
        .CmdByte = dataFlag,
        .TB = byte,

        .Unused = 0
    };

    data = *(uint16_t *)&stLcdData;

    CsPinLow();
    tickStartCnt = portNVIC_SYSTICK_CURRENT_VALUE_REG;

    SPI_I2S_SendData(SPI_MODULE, data);
    /* Wait for SPI_MASTER Tx buffer empty */
    while (   (SPI_I2S_GetFlagStatus(SPI_MODULE, SPI_I2S_FLAG_TXE) == RESET)
           && (SPI_I2S_GetFlagStatus(SPI_MODULE, SPI_I2S_FLAG_BSY) != RESET));

    while(((tickStartCnt + 72000 - portNVIC_SYSTICK_CURRENT_VALUE_REG ) % 72000) <= 1135);

    CsPinHigh();
}


void LCD_Test(void)
{
    static uint16_t testcnt = 0;
    /* GOTo x : */
    int i, j;
    
    if(testcnt++ < 2)
    {
        LCD_Initial();
    }
    else
    {
        LCD_WriteDataOrCmd(0x2a, 0);   
        LCD_WriteDataOrCmd(0, 1);
        LCD_WriteDataOrCmd(30, 1);
        LCD_WriteDataOrCmd(0, 1);
        LCD_WriteDataOrCmd(50, 1);
        
        LCD_WriteDataOrCmd(0x2b, 0);   
        LCD_WriteDataOrCmd(0, 1);
        LCD_WriteDataOrCmd(30, 1);
        LCD_WriteDataOrCmd(0, 1);
        LCD_WriteDataOrCmd(50, 1);
        
        LCD_WriteDataOrCmd(0x2c, 0);
        for(i = 30; i < 50; i++)
        {
            for(j = 30; j < 50; j++)
            {
                LCD_WriteDataOrCmd(0xF800 >> 8, 1);
        
                LCD_WriteDataOrCmd(0xF800 & 0xff, 1);
            }
        }
        
        LCD_WriteDataOrCmd(0x29, 0);//Write command 0x29;
    }
}
