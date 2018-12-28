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

void StateInitial(void)
{
    stCanOpenDate.NodeState = Pre_operational;
}


enNodeState GetNodeState(void)
{
    return  stCanOpenDate.NodeState;
}


void SetNodeState(enNodeState NewState)
{
    stCanMessage_t StateChangeMsg = {0};
    
    stCanOpenDate.NodeState = NewState;
    
    /* restart lifeguard timeout */
    HeardBeatClearCounter();
    
    StateChangeMsg.cob_id = stCanOpenDate.NodeId + 0x700;
    StateChangeMsg.len = (uint8_t)0x01;
    StateChangeMsg.rtr = 0;
    StateChangeMsg.data[0] = stCanOpenDate.NodeState;
    CAN_RequestSend(&StateChangeMsg);
}



