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
    union unEndianTest
    {
        uint8_t u8Data[2u];
        uint16_t u16Data;
    };
    
    union unEndianTest EndianTest = {0u};
    
    EndianTest.u16Data = 0xAA55u;
    
    if(EndianTest.u8Data[0u] == 0xAAu)
    {
        stCanOpenDate.BigEndianFlag = 1u;
    }
    else
    {
        stCanOpenDate.BigEndianFlag = 0u;
    }

    /* initial CANOpen data and state */
    stCanOpenDate.NodeId = MY_NODE_ID;
    stCanOpenDate.SlaveNodeFlag = I_AM_SLAVE_NODE;
    StateInitial();
    HeardBeatInitial();
    SDOInitial();
    //SyncInitial();
    
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
                ProcessSyncData(pCmd->data[0u]);
            }

            break;

        case PDO1tx:
            break;
            
        case PDO1rx:
        {
            SaveRxPDO1(&pCmd->data[0]);
            break;
        }
            
        case PDO2tx:
            break;
            
        case PDO2rx:
        {
            SaveRxPDO2(&pCmd->data[0]);
            break;
        }
        
        case PDO3tx:
            break;
            
        case PDO3rx:
        {
            SaveRxPDO3(&pCmd->data[0]);
            break;
        }
        
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
            if(pCmd->data[1u] == stCanOpenDate.NodeId)
            {
                ProcessNmtCommand(pCmd->data[0]);
            }
            break;
            
        default:
            break;
    }

    stCanOpenDate.CanFrameCnt++;
    
    return 1;
}


/* This should be call every 1ms */
uint8_t CanOpenTimerDispatch(uint16_t callPeriod)
{
    static uint16_t CAN_Timeout = 0;
    
    HeartBeatHandler(callPeriod);
    SDOCyclicHandler(callPeriod);
    
    if(CAN_Timeout >= 1000u)
    {
        CAN_Timeout = 0u;
        
        if(stCanOpenDate.CanFrameCnt)
        {
            stCanOpenDate.CanFrameCnt = 0u;
            
            stCanOpenDate.CanBusActiveFlag = 1u;
        }
        else
        {
            stCanOpenDate.CanBusActiveFlag = 0u;
        }
    }
    else
    {
        CAN_Timeout = CAN_Timeout + callPeriod;
    }
    
    
    return 0x0;
}


