/******************************************************************************************************
*            CanOpenBasic.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CanOpenBasic.c
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
stCanOpenDate_t stCanOpenDate = {0};

/*******************************************************************************
* Local Variables
*******************************************************************************/

uint8_t CanOpenInitial(void)
{
    /* initial CANOpen data and state */
    stCanOpenDate.NodeId = MY_NODE_ID;
    stCanOpenDate.SlaveNodeFlag = I_AM_SLAVE_NODE;
    StateInitial();
    HeardBeatInitial();
    SDOInitial();
    SyncInitial();
    
    return 1;
}


uint8_t CanOpenRxCmdHandler(stCanMessage_t *pCmd)
{
    uint16_t cob_id = pCmd->cob_id;

    switch(cob_id >> 7)
    {
        case SYNC:      /* can be a SYNC or a EMCY message */
            if(cob_id == 0x080) /* SYNC */
            {
            }

            break;

        case PDO1tx:
        case PDO1rx:
        case PDO2tx:
        case PDO2rx:
        case PDO3tx:
        case PDO3rx:
        case PDO4tx:
        case PDO4rx:

            break;

        case SDOtx:
        case SDOrx:
            if((pCmd->cob_id & 0x7f) == MY_NODE_ID)
            {
                ProceedExpSdoRxData(pCmd);
            }
            break;

        case NODE_GUARD:

            break;

        case NMT:
            if(pCmd->data[1] == stCanOpenDate.NodeId)
            {
                ProcessNmtCommand(pCmd->data[0]);
            }
            break;
            
        default:
            break;
    }
    
    return 1;
}


/* This should be call every 1ms */
uint8_t CanOpenTimerDispatch(void)
{
    HeartBeatHandler();
    SDOCyclicHandler();
    
    return 0x0;
}


