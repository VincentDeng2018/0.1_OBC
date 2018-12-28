/******************************************************************************************************
*            CanOpenState.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CanOpenState.c
*
*    DESCRIPTION: CANOPEN protocol handler APIs
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/10 22:34:20
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/8/25 17:44:21
******************************************************************************************************/
#include "CanOpenBasic.h"

/*********************************************************************************
* Global Functions
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

void HeardBeatInitial(void)
{
    /* TODO: map to OD 0x1017 when get OD ready */
    stCanOpenDate.HeartBeatTimer_ms = 1000;
    stCanOpenDate.HeartBeatCounter_ms = 0;
}


/* stop heartbeat */
void HeardBeatStop(void)
{
    stCanOpenDate.HeartBeatTimer_ms = 0;
    stCanOpenDate.HeartBeatCounter_ms = 0;
}


void HeardBeatClearCounter(void)
{
    stCanOpenDate.HeartBeatCounter_ms = 0;
}

/* HeartBeatCallback every 1ms */
void HeartBeatHandler(uint16_t callPeriod)
{
    if(stCanOpenDate.HeartBeatTimer_ms)
    {
        if(stCanOpenDate.HeartBeatCounter_ms >= stCanOpenDate.HeartBeatTimer_ms)
        {
            stCanMessage_t HeartBeatMsg = {0};
            
            stCanOpenDate.HeartBeatCounter_ms = 0;
            
            HeartBeatMsg.cob_id = stCanOpenDate.NodeId + 0x700;
            HeartBeatMsg.len = (uint8_t)0x01;
            HeartBeatMsg.rtr = 0;
            HeartBeatMsg.data[0] = (uint8_t)GetNodeState();
            CAN_RequestSend(&HeartBeatMsg);
        }
        else
        {
            stCanOpenDate.HeartBeatCounter_ms = stCanOpenDate.HeartBeatCounter_ms + callPeriod;
        }
        
    }
    else
    {
        stCanOpenDate.HeartBeatCounter_ms = 0;
    }
}



