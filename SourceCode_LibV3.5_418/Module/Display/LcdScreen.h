/******************************************************************************************************
*            LcdScreen.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: LcdScreen.h
*
*    DESCRIPTION: Header file for screen display
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/11/1 21:20:45
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                      2018/5/24 21:48:23
******************************************************************************************************/

#ifndef _LCDSCREEN_H
#define _LCDSCREEN_H

/**********************************************************************************************
* Macros
**********************************************************************************************/

/**************** Title X, Y coordinate *******************/
#define TITLE_TEXT_X_START                 48u
#define TITLE_TEXT_Y_START                 3u
#define TITLE_UNDERLINE_X_START            2u
#define TITLE_UNDERLINE_X_END              (COL - TITLE_UNDERLINE_X_START)
#define TITLE_UNDERLINE_Y_START            (TITLE_TEXT_Y_START + 16 + 1)
#define TITLE_UNDERLINE_Y_END              (TITLE_UNDERLINE_Y_START + 1u)


/**************** U I T Curve coordinate *******************/

/****************** HOME screen item x, y coordinate *****************/
#define HOME_TEXT_MODLE_X_START      8u
#define HOME_VAL_MODLE_X_START      (HOME_TEXT_MODLE_X_START + 7 * 8) /* 6 char + space */
#define HOME_TEXT_MODLE_Y_START      32u

#define HOME_TEXT_BATT_X_START      HOME_TEXT_MODLE_X_START
#define HOME_TEXT_BATT_Y_START      (HOME_TEXT_MODLE_Y_START + 30)

#define HOME_TEXT_CFG_X_START       HOME_TEXT_MODLE_X_START
#define HOME_VAL_CFG_X_START       (HOME_TEXT_CFG_X_START + 8*8)
#define HOME_TEXT_CFG_Y_START       (HOME_TEXT_BATT_Y_START + 30)

#define HOME_TEXT_STATE_X_START     HOME_TEXT_MODLE_X_START
#define HOME_VAL_STATE_X_START     (HOME_TEXT_STATE_X_START + 7 * 8)
#define HOME_TEXT_STATE_Y_START     (HOME_TEXT_CFG_Y_START + 30)

#define HOME_TEXT_U_X_START       HOME_TEXT_MODLE_X_START
#define HOME_VAL_U_X_START       (HOME_TEXT_U_X_START + 3 * 8)
#define HOME_TEXT_U_Y_START      (HOME_TEXT_STATE_Y_START + 30)

#define HOME_TEXT_I_X_START       80
#define HOME_VAL_I_X_START       (HOME_TEXT_I_X_START + 3 * 8)
#define HOME_TEXT_I_Y_START      (HOME_TEXT_STATE_Y_START + 30)

#define HOME_TEXT_CAP_X_START       HOME_TEXT_MODLE_X_START
#define HOME_VAL_CAP_X_START       (HOME_TEXT_MODLE_X_START + 9 * 8)
#define HOME_TEXT_CAP_Y_START      (HOME_TEXT_U_Y_START + 30)

#define HOME_TEXT_TIME_X_START      HOME_TEXT_MODLE_X_START
#define HOME_VAL_TIME_X_START      (HOME_TEXT_TIME_X_START + 6 * 8)
#define HOME_TEXT_TIME_Y_START     (HOME_TEXT_CAP_Y_START + 30)


/************  Menu screen *****************/
#define MENU_TEXT_CFG_X_START  10
#define MENU_TEXT_CFG_Y_START  50

#define MENU_TEXT_DATA_X_START   MENU_TEXT_CFG_X_START
#define MENU_TEXT_DATA_Y_START  (MENU_TEXT_CFG_Y_START + 40)

/************ Charge data screen ***************/
#define DATA_TEXT_NO_X_START  MENU_TEXT_CFG_X_START
#define DATA_VAL_NO_X_START  DATA_TEXT_NO_X_START + (12 * 8)
#define DATA_TEXT_NO_Y_START  DATA_TEXT_CFG_Y_START


/**********************************************************************************************
* Global Functions
**********************************************************************************************/



#endif
