/******************************************************************************************************
*            CanOpenNmt.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CanOpenNmt.c
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

void ChangeSlaveNodeState(uint8_t nmtCs, uint8_t slaveNodeId)
{
    /* only master is allowed to change slave node state*/
    if(!stCanOpenDate.SlaveNodeFlag)
    {
        stCanMessage_t NmtMsg = {0};

        NmtMsg.cob_id = NMT << 7;
        NmtMsg.len = (uint8_t)0x02;
        NmtMsg.rtr = 0;
        NmtMsg.data[0] = nmtCs;
        NmtMsg.data[1] = slaveNodeId;
        CAN_RequestSend(&NmtMsg);
    }
}


void ProcessNmtCommand(uint8_t nmtCs)
{
    if( (stCanOpenDate.SlaveNodeFlag)
        && ((stCanOpenDate.NodeState== Pre_operational) ||
            (stCanOpenDate.NodeState == Operational) ||
            (stCanOpenDate.NodeState == Stopped)) ) 
    {
        switch(nmtCs)
        { /* command specifier (cs) */
            case NMT_Start_Node:
                if ( (stCanOpenDate.NodeState == Pre_operational) || (stCanOpenDate.NodeState == Stopped) )
                {
                    SetNodeState(Operational);
                }
            break;
            
            case NMT_Stop_Node:
                if ( stCanOpenDate.NodeState == Pre_operational ||
                     stCanOpenDate.NodeState == Operational )
                {
                    SetNodeState(Stopped);
                }
            break;
            
            case NMT_Enter_PreOperational:
                if ( stCanOpenDate.NodeState == Operational ||
                     stCanOpenDate.NodeState == Stopped )
                {
                    SetNodeState(Pre_operational);
                }
            break;
            
            case NMT_Reset_Node:
                SetNodeState(Initialisation);
            break;
            
            case NMT_Reset_Comunication:
            {
                SetNodeState(Initialisation);
            }
             break;
        
        }/* end switch */
    }
}



