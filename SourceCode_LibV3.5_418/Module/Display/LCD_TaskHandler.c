/******************************************************************************************************
*            LCD_TaskHandler.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: LCD_TaskHandler.c
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

#include "lcd.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "LcdScreen.h"
#include "METER_AdcSample.h"

/*********************************************************************************
* Global Functions
*********************************************************************************/

/*******************************************************************************
* Macros
*******************************************************************************/
#define LCD_SLEEP_TIMEOUT_MS        60000


/*******************************************************************************
* Data Struct Define
*******************************************************************************/
typedef enum
{
    LCD_INIT = 0x0,
    LCD_TESTING,
    LCD_RUNNING, /* Plot voltage and current curve for last charge */
    LCD_SLEEP
}enLcdState;

typedef enum
{
    HOME_SCREEN = 0x0,
    MENU_SCREEN,
    CHARGE_CFG_SCREEN,
    CHARGE_DATA_SCREEN,
    END_SCREEN
}enScreenId_t;

typedef struct
{
    enScreenId_t CurrentScreen;
    enScreenId_t PreScreen;
    uint8_t  ReDrawStaticItem;
    uint16_t ChgDataIndex;
    enKeyPress  KeyPress;
    enLcdState CurrentState;
    uint32_t  NoKeyTimer_ms;
}stScreenInfo_t;

stScreenInfo_t stScreenInfo;

/*******************************************************************************
* Local Variables
*******************************************************************************/


/*********************************************************************************
* Task Call back Functions
*********************************************************************************/
static void LcdTaskInit(void);
static void ShowTitle(void);
static void HomeScreen(void);
static void HomeScreenStaticFrame(void);
static void DrawCurveCoordinate(void);
static void ClearContentArea(void);
static void UpdateKeyState(void);
static void HomeScreenStaticFrame(void);
static void MenuScreen(void);
static void ChargeDataScreen(void);
static void NavigateScreen(enScreenId_t newScreen);
static void ConverterNumToString(uint16_t num, uint8_t *pCharBuff);

static void ShowTitle(void)
{
    /* Company title */
    DispStr("PowerFirst Technology Co.LTD.", TITLE_TEXT_X_START, TITLE_TEXT_Y_START,
                  BLACK, WHITE);
    DrawLine(TITLE_UNDERLINE_X_START, TITLE_UNDERLINE_X_END,
             TITLE_UNDERLINE_Y_START, TITLE_UNDERLINE_Y_END, BLACK);
}

static void HomeScreen(void)
{
    if(stScreenInfo.ReDrawStaticItem)
    {
        stScreenInfo.ReDrawStaticItem = 0u;
        /* clean content area */
        ClearScreen(1, 158, TITLE_UNDERLINE_Y_END + 1, ROW - 1);

        /* Draw static text and frame */
        HomeScreenStaticFrame();

        DrawCurveCoordinate();
    }
    
    switch(GetKeyPressed())
    {
        case KEY_1_UP:
        case KEY_2_DOWN:
            NavigateScreen(MENU_SCREEN);
            break;

        case KEY_3_BACK:
        case KEY_4_ENTER:
        default:
            break;
    }
}

static void HomeScreenStaticFrame(void)
{
    DispStr("Model:", HOME_TEXT_MODLE_X_START, HOME_TEXT_MODLE_Y_START,
                  BLACK, WHITE);

    DispStr("Battery: 12V 10A", HOME_TEXT_BATT_X_START, HOME_TEXT_BATT_Y_START,
                  BLACK, WHITE);

    DispStr("ChrgCfg:", HOME_TEXT_CFG_X_START, HOME_TEXT_CFG_Y_START,
                  BLACK, WHITE);

    DispStr("Status:", HOME_TEXT_STATE_X_START, HOME_TEXT_STATE_Y_START,
                  BLACK, WHITE);

    DispStr("U:", HOME_TEXT_U_X_START, HOME_TEXT_U_Y_START,
                  BLACK, WHITE);

    DispStr("I:", HOME_TEXT_I_X_START, HOME_TEXT_I_Y_START,
                  BLACK, WHITE);

    DispStr("Capacity:", HOME_TEXT_CAP_X_START, HOME_TEXT_CAP_Y_START,
                  BLACK, WHITE);

    DispStr("AH", HOME_VAL_CAP_X_START + 5 * 8, HOME_TEXT_CAP_Y_START,
                  BLACK, WHITE);

    DispStr("Time:", HOME_TEXT_TIME_X_START, HOME_TEXT_TIME_Y_START,
                  BLACK, WHITE);
}

static void DrawCurveCoordinate(void)
{
    uint16_t i;

    /************************** U axis **************************/
    DrawLine(145, 146, 32, 212, BLACK);

    /****** array *****/
    PutPixel(145 - 1, 32 + 1, BLACK);
    PutPixel(145 - 2, 32 + 2, BLACK);
    PutPixel(145 - 3, 32 + 3, BLACK);
    PutPixel(145 - 4, 32 + 4, BLACK);
    PutPixel(146 + 1, 32 + 1, BLACK);
    PutPixel(146 + 2, 32 + 2, BLACK);
    PutPixel(146 + 3, 32 + 3, BLACK);
    PutPixel(146 + 4, 32 + 4, BLACK);

    /* Scale for U: Y:212 = 0, 12 scale, each scale division is 13 pixel */
    for(i = 0; i < 8; i++)
    {
        DrawLine(146, 150, 56 + i * 19, 57 + i * 19, BLACK);
        DrawLine(304 - 5, 304, 56 + i * 19, 57 + i * 19, BLACK);
    }

    /************************** I axis **************************/
    DrawLine(304, 305, 32, 212, BLACK);

    /****** array *****/
    PutPixel(304 - 1, 32 + 1, BLACK);
    PutPixel(304 - 2, 32 + 2, BLACK);
    PutPixel(304 - 3, 32 + 3, BLACK);
    PutPixel(304 - 4, 32 + 4, BLACK);
    PutPixel(305 + 1, 32 + 1, BLACK);
    PutPixel(305 + 2, 32 + 2, BLACK);
    PutPixel(305 + 3, 32 + 3, BLACK);
    PutPixel(305 + 4, 32 + 4, BLACK);

    /************************** t axis **************************/
    DrawLine(145, 309, 211, 212, BLACK);
    /****** array *****/
    PutPixel(309 - 1, 211 + 1, BLACK);
    PutPixel(309 - 2, 211 + 2, BLACK);
    PutPixel(309 - 3, 211 + 3, BLACK);
    PutPixel(309 - 1, 211 - 1, BLACK);
    PutPixel(309 - 2, 211 - 2, BLACK);
    PutPixel(309 - 3, 211 - 3, BLACK);

    DispStr("m", 310, 212-4,
                  BLACK, WHITE);

    for(i = 0; i < 5; i++)
    {
        DrawLine(146 + 24 + 27 *i, 146 + 24 + 27 *i + 1, 211 - 5, 211, BLACK);
    }

    DispStr("V ", 150, 32,
                  BLACK, WHITE);

    DrawLine(192, 192 + 24, 32 + 8, 32 + 9, BLUE);
    DispStr("U/t", 192 + 22, 32,
                  BLACK, WHITE);

    DispStr("I", 310, 32,
                  BLACK, WHITE);
    DrawLine(192 + 22 + 32, 192 + 24 + 32 + 24, 32 + 8, 32 + 9, RED);
    DispStr("I/t", 192 + 24 + 32 + 24, 32,
                  BLACK, WHITE);

}


static void MenuScreen(void)
{
    DispStr("Charge Config:", HOME_TEXT_MODLE_X_START, HOME_TEXT_MODLE_Y_START,
                  WHITE, BLACK);

    DispStr("Charge Data:", HOME_TEXT_BATT_X_START, HOME_TEXT_BATT_Y_START + 8,
                  BLACK, WHITE);
}


static void ChargeDataScreen(void)
{
    uint8_t StringBuff[4] = {0};

    if(stScreenInfo.ReDrawStaticItem)
    {
        stScreenInfo.ReDrawStaticItem = 0u;
        DispStr("Charge NO.:", HOME_TEXT_MODLE_X_START, HOME_TEXT_MODLE_Y_START,
                      BLACK, WHITE);
    }

    switch(GetKeyPressed())
    {
        case KEY_1_UP:
            if(stScreenInfo.ChgDataIndex > 1)
            {
                stScreenInfo.ChgDataIndex--;
            }

            break;

        case KEY_2_DOWN:
            stScreenInfo.ChgDataIndex++;
            break;

        case KEY_3_BACK:
            NavigateScreen(MENU_SCREEN);
            break;

        case KEY_4_ENTER:
        default:
            break;
    }

    ConverterNumToString(stScreenInfo.ChgDataIndex, &StringBuff[0]);

    DispStr(&StringBuff[0], HOME_TEXT_BATT_X_START + 12 * 8, HOME_TEXT_MODLE_Y_START,
                      WHITE, BLACK);
}

static void NavigateScreen(enScreenId_t newScreen)
{
    stScreenInfo.PreScreen = stScreenInfo.CurrentScreen;
    stScreenInfo.CurrentScreen = newScreen;
    stScreenInfo.ReDrawStaticItem = 1;
}

/***** Update key state and no key timer ****/
static void UpdateKeyState(void)
{
    enKeyPress keyPressed = KEY_NONE;

    if(GetKeyPressed() != KEY_NONE)
    {
        stScreenInfo.NoKeyTimer_ms = 0;

        /* debounce key state */
        vTaskDelay(10);
        if(GetKeyPressed() != KEY_NONE)
        {
            keyPressed = GetKeyPressed();
            stScreenInfo.NoKeyTimer_ms = 0;
        }
    }
    else
    {
        /* run every 10ms */
        stScreenInfo.NoKeyTimer_ms += 10u;
        if(stScreenInfo.NoKeyTimer_ms >= LCD_SLEEP_TIMEOUT_MS)
        {
            /* TODO: LCD enter sleep mode via turn off backlight, but screen is keeping refresh */
        }
    }

    stScreenInfo.KeyPress = keyPressed;
}



void LCD_TaskHandler(void *pvParameters)
{
    LcdTaskInit();

    for(;;)
    {
        vTaskDelay(10);

        UpdateKeyState();

        switch(stScreenInfo.CurrentScreen)
        {
            case CHARGE_DATA_SCREEN:
                break;

            case CHARGE_CFG_SCREEN:
                ChargeDataScreen();
                break;

            case HOME_SCREEN:
            default:
                HomeScreen();
                break;
        }
    }
}


void Delay(uint16_t x_ms)
{
    vTaskDelay(x_ms);
}

static void LcdTaskInit(void)
{
    LCD_Init();

    stScreenInfo.CurrentScreen = HOME_SCREEN;
    stScreenInfo.PreScreen  = HOME_SCREEN;
    stScreenInfo.ReDrawStaticItem = 1;
    stScreenInfo.KeyPress = KEY_NONE;
    stScreenInfo.NoKeyTimer_ms = 0;
    stScreenInfo.ChgDataIndex = 1;

    /* Title will not change all the time */
    ShowTitle();
}


static void ConverterNumToString(uint16_t num, uint8_t *pCharBuff)
{
    pCharBuff[0] = (num / 100) + '0';
    pCharBuff[1] = ((num / 10) % 10) + '0';
    pCharBuff[2] = (num % 10) + '0';
    pCharBuff[3] = '\0';
}