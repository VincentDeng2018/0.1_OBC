/******************************************************************************************************
*            CAN_Handler_Win.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CAN_HandlerRTOS.c
*
*    DESCRIPTION: Portable code for CANopen in freeRTOS + stm32f10x
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/8/2 21:29:16
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Copy from canfestival CANOPEN stack and change some api to make it run in
*                                  freeRTOS and stm32f10x                 2018/8/2 21:31:12
******************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "CanOpenBasic.h"
#include <string.h>

uint8_t CAN_RequestSend(stCanMessage_t *m)
{
    extern void CanOpenTxRequestApi(stCanMessage_t CanOpenRequest);

    stCanMessage_t msg = {0u};
    
    memcpy(&msg, m, sizeof(stCanMessage_t));
    
    CanOpenTxRequestApi(msg);

    return 1u;
}