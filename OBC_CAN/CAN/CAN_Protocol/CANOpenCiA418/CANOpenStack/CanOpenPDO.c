/******************************************************************************************************
*            CanOpenPDO.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CanOpenPDO.c
*
*    DESCRIPTION: CANOPEN protocol handler APIs
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/10 22:34:20
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/10/11 20:29:25
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

void PDOInitial(void)
{
    stCanOpenDate.NextPDOIndex = 0;
}



/****************************************************************************
*
*  Function: SendPdoData
*
*  Purpose :    Send PDO when receive sync message
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    : TODO: double check whether only send one PDO each time
*
****************************************************************************/
void SendPdoData(void)
{
    stCanMessage_t PdoMsg = {0};

    /* only support standard CAN ID */
#ifdef CIA_418_OD
    uint32_t u32ReadData = 0;
#endif
    uint16_t u16ReadData = 0;
    uint8_t  ReadLength = 0;

    
    PdoMsg.len = (uint8_t)0x08;
    PdoMsg.rtr = 0;
    stCanOpenDate.NextPDOIndex = 0;
    
    while(stCanOpenDate.NextPDOIndex < MAX_PDO_NUM)
    {

#ifdef CIA_418_OD

        /* get TPDO COB-ID */
        ReadLength = 2;
        ReadOdMap(0x1800 + stCanOpenDate.NextPDOIndex , 0x01, &ReadLength, (uint8_t *)&u16ReadData);
        PdoMsg.cob_id = (uint16_t)(u16ReadData & 0xffff);

        switch(stCanOpenDate.NextPDOIndex)
        {
            case 0u:
                ReadLength = 2u;
                ReadOdMap(0x6010 , 0, &ReadLength, (uint8_t *)&u16ReadData);
                PdoMsg.data[0u] = (uint8_t)(u16ReadData & 0xFF);
                PdoMsg.data[1u] = (uint8_t)(u16ReadData >> 8);

                ReadLength = 1u;
                ReadOdMap(0x6000 , 0, &ReadLength, &PdoMsg.data[2u]);
                break;

            case 1u:
                ReadLength = 2u;
                ReadOdMap(0x6010 , 0, &ReadLength, (uint8_t *)&u16ReadData);
                PdoMsg.data[0u] = (uint8_t)(u16ReadData & 0xFF);
                PdoMsg.data[1u] = (uint8_t)(u16ReadData >> 8);

                ReadLength = 1;
                ReadOdMap(0x6001 , 0, &ReadLength, &PdoMsg.data[2]);

                ReadLength = 4;
                ReadOdMap(0x6060, 0, &ReadLength, (uint8_t *)&u32ReadData);
                PdoMsg.data[3] = (uint8_t)(u32ReadData & 0xFF);
                PdoMsg.data[4] = (uint8_t)((u32ReadData >> 8u) & 0xFF);
                PdoMsg.data[5] = (uint8_t)((u32ReadData >> 16u) & 0xFF);
                PdoMsg.data[6] = (uint8_t)((u32ReadData >> 24u) & 0xFF);
                break;

            case 2u:
                ReadLength = 2u;
                ReadOdMap(0x6070 , 0, &ReadLength, (uint8_t *)&u16ReadData);
                PdoMsg.data[0u] = (uint8_t)(u16ReadData & 0xFF);
                PdoMsg.data[1u] = (uint8_t)(u16ReadData >> 8);

                ReadLength = 1u;
                ReadOdMap(0x6081 , 0, &ReadLength, &PdoMsg.data[2u]);
                break;

            default:
                ReadLength = 2u;
                ReadOdMap(0x6010 , 0, &ReadLength, (uint8_t *)&u16ReadData);
                PdoMsg.data[0u] = (uint8_t)(u16ReadData & 0xFF);
                PdoMsg.data[1u] = (uint8_t)(u16ReadData >> 8);

                ReadLength = 1u;
                ReadOdMap(0x6000 , 0, &ReadLength, &PdoMsg.data[2u]);

                ReadLength = 2;
                ReadOdMap(0x1800 , 0, &ReadLength, (uint8_t *)(uint8_t *)&u16ReadData);
                PdoMsg.cob_id = (uint16_t)(u16ReadData & 0xffff);
                break;
        }


#else
        /* get TPDO COB-ID */
        ReadLength = 2;
        ReadOdMap(0x1800 + stCanOpenDate.NextPDOIndex , 0x01, &ReadLength, (uint8_t *)&u16ReadData);
        PdoMsg.cob_id = (uint16_t)(u16ReadData & 0xffff);

        switch(stCanOpenDate.NextPDOIndex)
        {
            case 0u:
                ReadLength = 1;
                ReadOdMap(0x6001 , 0, &ReadLength, &PdoMsg.data[0]);
                break;

            case 1u:
                ReadLength = 1;
                ReadOdMap(0x6001 , 0, &ReadLength, &PdoMsg.data[0]);
                ReadLength = 2;
                ReadOdMap(0x6052 , 0, &ReadLength, (uint8_t *)&u16ReadData);
                PdoMsg.data[1] = (uint8_t)(u16ReadData & 0xFF);
                PdoMsg.data[2] = (uint8_t)(u16ReadData >> 8);
                break;

            case 2u:
                ReadLength = 1;
                ReadOdMap(0x6001 , 0, &ReadLength, &PdoMsg.data[0]);

                ReadLength = 2;
                ReadOdMap(0x6052 , 0, &ReadLength, (uint8_t *)&u16ReadData);
                PdoMsg.data[1] = (uint8_t)(u16ReadData & 0xFF);
                PdoMsg.data[2] = (uint8_t)(u16ReadData >> 8);

                ReadLength = 1;
                ReadOdMap(0x6080 , 0, &ReadLength, &PdoMsg.data[3]);
                break;

            default:
                ReadLength = 1;
                ReadOdMap(0x6001 , 0, &ReadLength, &PdoMsg.data[0]);

                ReadLength = 2;
                ReadOdMap(0x1800 + stCanOpenDate.NextPDOIndex , 0, &ReadLength, (uint8_t *)&u16ReadData);
                PdoMsg.cob_id = (uint16_t)(u16ReadData & 0xffff);
                break;
        }
        stCanOpenDate.NextPDOIndex++;
#endif
        stCanOpenDate.NextPDOIndex++;
        CAN_RequestSend(&PdoMsg);
    }
}