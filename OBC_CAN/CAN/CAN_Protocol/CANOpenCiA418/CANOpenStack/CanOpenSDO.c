/******************************************************************************************************
*            CanOpenSDO.c
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
#include <string.h>

/*********************************************************************************
* Global Functions
*********************************************************************************/
void PollingNodeDeviceProfileType(void);
void SDOInitial(void);
void SDOCyclicHandler(uint16_t callPeriod);
void ProceedExpSdoRxData(stCanMessage_t *pMsg);
void PollingProfileOd(void);

/*******************************************************************************
* Macros
*******************************************************************************/


/*******************************************************************************
* Data Struct Define
*******************************************************************************/
stNodeDeviceProfile_t stNodeDeviceProfile[MAX_CAN_NODE];

const uint8_t ExpeditedSDOReturnCS[4] =
{
    EXP_READ_ONE_BYTE,
    EXP_READ_TWO_BYTE,
    EXP_READ_THREE_BYTE,
    EXP_READE_FOUR_BYTE,
};
/*******************************************************************************
* Local Variables
*******************************************************************************/
static uint8_t CanNodeID = 0x01u;
static stCanMessage_t PollingMsg =
{
    0,     //cob_id
    0,     //rtr
    0x08,  //len
    {0x40, 0, 0x10, 0, 0, 0, 0, 0} //data[8]
};

void SDOInitial(void)
{
    memset(&stNodeDeviceProfile, 0, sizeof(stNodeDeviceProfile_t) * MAX_CAN_NODE);
}


void PollingNodeDeviceProfileType(void)
{
    if(CanNodeID > MAX_CAN_NODE)
    {
        CanNodeID = 0x01u;
    }

    if(CanNodeID != stCanOpenDate.NodeId)
    {
        PollingMsg.cob_id = CanNodeID + 0x600;
        PollingMsg.data[0] = 0x40;
        PollingMsg.data[1] = 0;
        PollingMsg.data[2] = 0x10;
        PollingMsg.data[3] = 0;
        PollingMsg.data[4] = 0;
        PollingMsg.data[5] = 0;
        PollingMsg.data[6] = 0;
        PollingMsg.data[7] = 0;
        CAN_RequestSend(&PollingMsg);
        stNodeDeviceProfile[CanNodeID].StartTimeout = 1;
    }
    else
    {
        /* skip myself */
    }
    CanNodeID++;
}


void PollingProfileOd(void)
{
    static uint16_t OD_Phase = 0u;

    switch(OD_Phase)
    {
        case 0u:
            PollingMsg.cob_id = CanNodeID + 0x600;
            PollingMsg.data[0] = 0x40;
            PollingMsg.data[1] = 1;
            PollingMsg.data[2] = 0x60;
            PollingMsg.data[3] = 0;
            PollingMsg.data[4] = 0;
            PollingMsg.data[5] = 0;
            PollingMsg.data[6] = 0;
            PollingMsg.data[7] = 0;
            OD_Phase++;
            break;

        case 1u:
            PollingMsg.data[1] = 0xA0;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 2u:
            PollingMsg.data[1] = 0xA1;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 3u:
            PollingMsg.data[1] = 0xA2;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 4u:
            PollingMsg.data[1] = 0xA3;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 5u:
            PollingMsg.data[1] = 0xA4;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 6u:
            PollingMsg.data[1] = 0xA5;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 7u:
            PollingMsg.data[1] = 0xA6;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 8u:
            PollingMsg.data[1] = 0xA7;
            PollingMsg.data[2] = 0x60;
            OD_Phase++;
            break;

        case 9U:
            PollingMsg.data[1] = 0xA8;
            PollingMsg.data[2] = 0x60;
            OD_Phase = 0;
            break;
        
        default:
            break;
    }

    CAN_RequestSend(&PollingMsg);
}

void SDOCyclicHandler(uint16_t callPeriod)
{
    uint16_t static CyclicCounter = 0u;
    
#ifdef CIA_418_OD
    if(CyclicCounter >= 100u)
    {
        CyclicCounter = 0u;
        PollingProfileOd();
    }
    else
    {
        CyclicCounter = CyclicCounter + callPeriod;
    }
#else
    if(stCanOpenDate.SlaveNodeFlag != I_AM_SLAVE_NODE)
    {
        /* TODO: check OD to figure out whether there is parameter to define the timeout */
        if((++CyclicCounter % 1000) == 0x131)
        {
            PollingNodeDeviceProfileType();
        }

        if(stNodeDeviceProfile[CanNodeID].StartTimeout)
        {
            /* TODO: timeout value should remap to OD? */
            if(stNodeDeviceProfile[CanNodeID].WaitCounter++ > 500)
            {
                /* when timeout response error */
                PollingMsg.data[0] = 0x80;
                /* todo: why 0x4 and 0x5? */
                PollingMsg.data[6] = 4;
                PollingMsg.data[7] = 5;
                CAN_RequestSend(&PollingMsg);
                stNodeDeviceProfile[CanNodeID].StartTimeout = 0;
                stNodeDeviceProfile[CanNodeID].WaitCounter = 0;
            }
        }
    }
#endif
}


void ProceedExpSdoRxData(stCanMessage_t *pMsg)
{
    uint16_t u16odIndex = (uint16_t)pMsg->data[1u] + (((uint16_t)pMsg->data[2u]) << 8u);

    uint8_t u8csByte = pMsg->data[0u];
    uint8_t u8length = 0u;
    uint8_t result = OD_RW_SUCCESS;
    uint8_t buffer[4u] = {0u};

    stCanMessage_t stCanMessageTemp =
    {
        0u,      //cob_id
        0u,      //rtr
        8u,      //len
        {0u}    //data[8]
    };

    stCanMessageTemp.cob_id = CanNodeID + 0x580u;
    stCanMessageTemp.data[1u] = pMsg->data[1u];
    stCanMessageTemp.data[2u] = pMsg->data[2u];
    stCanMessageTemp.data[3u] = pMsg->data[3u];

    /* means write OD */
    if((u8csByte & 0xF0u) == 0x20u)
    {
        switch(u8csByte)
        {
            case EXP_WRITE_ONE_BYTE:
                u8length = 1;

                buffer[0u] = pMsg->data[4u];
                break;

            case EXP_WRITE_TWO_BYTE:
                u8length = 2;
                if(stCanOpenDate.BigEndianFlag == 0u)
                {
                    buffer[0u] = pMsg->data[5u];
                    buffer[1u] = pMsg->data[4u];
                }
                else
                {
                    buffer[0u] = pMsg->data[4u];
                    buffer[1u] = pMsg->data[5u];
                }
                break;

            case EXP_WRITE_THREE_BYTE:
                u8length = 3;

                if(stCanOpenDate.BigEndianFlag == 0u)
                {
                    buffer[0u] = pMsg->data[6u];
                    buffer[1u] = pMsg->data[5u];
                    buffer[2u] = pMsg->data[4u];
                }
                else
                {
                    buffer[0u] = pMsg->data[4u];
                    buffer[1u] = pMsg->data[5u];
                    buffer[2u] = pMsg->data[6u];
                }
                break;

            case EXP_WRITE_FOUR_BYTE:
                u8length = 4;
                if(stCanOpenDate.BigEndianFlag == 0u)
                {
                    buffer[0u] = pMsg->data[7u];
                    buffer[1u] = pMsg->data[6u];
                    buffer[2u] = pMsg->data[5u];
                    buffer[3u] = pMsg->data[4u];
                }
                else
                {
                    buffer[0u] = pMsg->data[4u];
                    buffer[1u] = pMsg->data[5u];
                    buffer[2u] = pMsg->data[6u];
                    buffer[2u] = pMsg->data[7u];
                }
                break;

            default:
                u8length = 1;
                buffer[0u] = pMsg->data[4u];
                break;
        }

         /* write OD */
        if(OD_RW_SUCCESS == WriteOdMap(u16odIndex, pMsg->data[3], u8length, &buffer[0u]))
        {
            stCanMessageTemp.data[0] = 0x60;
            result = OD_RW_SUCCESS;

        }
        else
        {
            stCanMessageTemp.data[0] = EXP_CMD_NACK;
            result = EXP_CMD_NACK;
        }

        stCanMessageTemp.data[4] = pMsg->data[4u];
        stCanMessageTemp.data[5] = pMsg->data[5u];
        stCanMessageTemp.data[6] = pMsg->data[6u];
        stCanMessageTemp.data[7] = pMsg->data[7u];

        CAN_RequestSend(&stCanMessageTemp);
    }
    /* read or read response */
    else if((u8csByte & 0xF0u) == 0x40u)
    {
        /* read request */
        if(u8csByte == 0x40)
        {
            /* read OD */
            if(OD_RW_SUCCESS == ReadOdMap(u16odIndex, pMsg->data[3], &u8length, &buffer[0u]))
            {
                stCanMessageTemp.data[0] = ExpeditedSDOReturnCS[u8length - 1];
                result = OD_RW_SUCCESS;
            }
            else
            {
                stCanMessageTemp.data[0] = EXP_CMD_NACK;
                result = EXP_CMD_NACK;
            }

            if(stCanOpenDate.BigEndianFlag == 0u)
            {
                if(u8length == 2u)
                {
                    pMsg->data[4u] = buffer[1u];
                    pMsg->data[5u] = buffer[0u];
                }
                else if(u8length == 3u)
                {
                    pMsg->data[4u] = buffer[2u];
                    pMsg->data[5u] = buffer[1u];
                    pMsg->data[6u] = buffer[0u];
                }
                else if(u8length == 4u)
                {
                    pMsg->data[4u] = buffer[3u];
                    pMsg->data[5u] = buffer[2u];
                    pMsg->data[6u] = buffer[1u];
                    pMsg->data[7u] = buffer[0u];
                }
            }

            memcpy(&stCanMessageTemp.data[4u], &buffer[0u], u8length);

            CAN_RequestSend(&stCanMessageTemp);
        }
        /* read one byte response */
        else
        {
            if(u8csByte == 0x4Fu)
            {
                /* Read one byte response */
                u8length = 1u;

                buffer[0u] = pMsg->data[4u];
            }
            /* read two byte response */
            else if(u8csByte == 0x4Bu)
            {
                u8length = 2u;

                if(stCanOpenDate.BigEndianFlag == 0u)
                {
                    buffer[0u] = pMsg->data[5u];
                    buffer[1u] = pMsg->data[4u];
                }
                else
                {
                    buffer[0u] = pMsg->data[4u];
                    buffer[1u] = pMsg->data[5u];
                }
            }
            /* read three byte response */
            else if(u8csByte == 0x47u)
            {
                u8length = 3u;
                if(stCanOpenDate.BigEndianFlag == 0u)
                {
                    buffer[0u] = pMsg->data[6u];
                    buffer[1u] = pMsg->data[5u];
                    buffer[2u] = pMsg->data[4u];
                }
                else
                {
                    buffer[0u] = pMsg->data[4u];
                    buffer[1u] = pMsg->data[5u];
                    buffer[2u] = pMsg->data[6u];
                }
            }
            /* read four byte response */
            else if(u8csByte == 0x43u)
            {
                u8length = 4u;
                if(stCanOpenDate.BigEndianFlag == 0u)
                {
                    buffer[0u] = pMsg->data[7u];
                    buffer[1u] = pMsg->data[6u];
                    buffer[2u] = pMsg->data[5u];
                    buffer[3u] = pMsg->data[4u];
                }
                else
                {
                    buffer[0u] = pMsg->data[4u];
                    buffer[1u] = pMsg->data[5u];
                    buffer[2u] = pMsg->data[6u];
                    buffer[2u] = pMsg->data[7u];
                }
            }

            /* update OD data based on read response result */
            WriteOdMap(u16odIndex, pMsg->data[3], u8length, &buffer[0u]);
        }
    }
    /* read or write error response */
    else if((u8csByte & 0xF0u) == 0x80u)
    {
        /* ignore it */
    }
    /* write ok response */
    else if((u8csByte & 0xF0u) == 0x60u)
    {
        /* ignore it */
    }
}

