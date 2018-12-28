/******************************************************************************************************
*            CAN_Portable.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CAN_Portable.h
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

#ifndef _CAN_PORTABLE_H
#define _CAN_PORTABLE_H

/**********************************************************************************************
* Macros
**********************************************************************************************/
/* setting for myself */
#define RTOS_IS_USED
#define MY_NODE_ID      0x2
#define I_AM_SLAVE_NODE 1u

/* CAN bus setting */
#define MAX_CAN_NODE 127
//#define CIA_418_OD
/**********************************************************************************************
* Global Functions
**********************************************************************************************/
extern uint8_t GetBatteryState(void);
extern int16_t GetBatteryTemp_100mC(void);
extern uint32_t GetTotalCharge_Ah(void);
extern uint16_t GetLastCharge_100mAh(void);
extern uint16_t GetBatteryVolt_10mV(void);
extern uint16_t GetChargeCmd_10mA(void);
extern uint16_t GetBatterySOC(void);

#endif
