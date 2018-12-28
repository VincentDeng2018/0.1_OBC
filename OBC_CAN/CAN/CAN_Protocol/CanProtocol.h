/******************************************************************************************************
*            CanProtocol.h
******************************************************************************************************
******************************************************************************************************
*
* THIS INFORMATION IS PROPRIETARY TO BYD Corporation
*
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CanProtocol.h
*
*    DESCRIPTION: Header file for Can protocol
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/11/3 16:45:09
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version
******************************************************************************************************/

/**********************************************************************************************
* Includes
**********************************************************************************************/
#ifndef _CANPROTOCOL_H
#define _CANPROTOCOL_H


#include "CanDriver.h"

/* Chip lib include */
/* user include */
#include "XcpBasic.h"

/*********************************************************************************
* Macros
*********************************************************************************/
#define CAN_PROTOCOL_MAJOR_VERSION 1u 
#define CAN_PROTOCOL_MINOR_VERSION 0u 

/************************** XCP Macro ********************************/
#define XCP_CMD_OK              0x0
#define XCP_TIME_OUT            0x01
#define XCP_ACCESS_DENIED       0x02

typedef enum
{
    METER0_TYPE_ID = 0u,
    METER1_TYPE_ID,
    METER2_TYPE_ID,
    METER3_TYPE_ID,
    METER4_TYPE_ID,
    METER5_TYPE_ID,
    METER6_TYPE_ID,
    METER7_TYPE_ID,
    METER8_TYPE_ID,
    METER9_TYPE_ID,
    METER10_TYPE_ID,
    METER11_TYPE_ID,
    METER12_TYPE_ID,
    METER13_TYPE_ID,
    METER14_TYPE_ID,
    METER15_TYPE_ID,
    METER16_TYPE_ID,
    METER17_TYPE_ID,
    METER18_TYPE_ID,
    METER19_TYPE_ID,
    METER_TYPE_ID_END = 0x1f
}enMeterTypeId;

/**************** Status CAN frame, type range from 20-3f *******************/
typedef enum
{
    STATE0_TYPE_ID = 0x20u,
    STATE1_TYPE_ID,
    STATE2_TYPE_ID,
    STATE3_TYPE_ID,
    STATE4_TYPE_ID,
    STATE_TYPE_ID_END = 0x3f
}enStateTypeId;

/**************** special purpose CAN frame, type range from 40-4f *******************/
typedef enum
{
    RTC_SYNC_TYPE_ID = 0x40u,
    PRG_TYPE_ID = 0x41u,
    XCP_TYPE_ID = 0x42u,
    HWTEST_TYPE_ID = 0x43u,
    CANID_AUTO_TYPE_ID = 0x44u,
    OPEN_LOOP_ENABLE = 0x45u,
    SPECIAL_TYPE_ID_END = 0x4f
}enSpecialTypeId;

/**************** Controller parameter and vars, type range from 50-5f *******************/
typedef enum
{
    CTRL0_TYPE_ID = 0x50u,
    CTRL1_TYPE_ID,
    CTRL2_TYPE_ID,
    CTRL3_TYPE_ID,
    CTRL4_TYPE_ID,
    CTRL5_TYPE_ID,
    CTRL6_TYPE_ID,
    CTRL7_TYPE_ID,
    CTRL8_TYPE_ID,
    CTRL9_TYPE_ID,
    CTRL10_TYPE_ID,
    CTRL11_TYPE_ID,
    CTRL_TYPE_ID_END = 0x5f
}enCtrlTypeId;

/**************** BMS information, type range from 60-6f *******************/
typedef enum
{
    INFOR0_TYPE_ID = 0x60u,
    INFOR1_TYPE_ID,
    INFOR2_TYPE_ID,
    INFOR3_TYPE_ID,
    INFOR_TYPE_ID_END = 0x6f
}enBMSInfoTypeId;

/**************** BMS statistics, type range from 70-7f *******************/
typedef enum
{
    STATISTICS0_TYPE_ID = 0x70u,
    STATISTICS1_TYPE_ID,
    STATISTICS2_TYPE_ID,
    STATISTICS3_TYPE_ID,
    STATISTICS_TYPE_ID_END = 0x7f
}enStatisticsTypeId;

/**************** CAN frame priority: lower number has higher priority *******************/
typedef enum
{
    CAN_PRI_HIGH = 0x0,
    CAN_PRI_MED,
    CAN_PRI_LOW
}enCanPriority;

struct stCANIdBits
{
  UINT32 src: 8;

  UINT32 dst: 8;

  UINT32 type: 11; /* function code */

  UINT32 priority: 2; /* 0 has highest priority */
  
  UINT32 reserved: 3;
};

typedef union CANHeader
{
  struct stCANIdBits bits;
  UINT32 all;
} CANHeader_t;

typedef struct
{
    UINT8  CmdCode;
    UINT8  CmdSubCode;
    UINT32 McuAddress;
    UINT16 Length;
    UINT16 Section;
    UINT16 *StoredAddress;
    UINT16 Buffer[4];
    TEdit  *pTEdit;
}XcpRequest_t;


/*********************************************************************************
* Data and Structure
*********************************************************************************/
typedef struct
{
    UINT8        XcpRequestData[8];
    UINT8        XcpResponseData[8];
    
    UINT8 ResponseLen;
    UINT8 ResponseOk;
    UINT8 ResponseCmdCode;
    UINT16 RequestLen;

    /* SHORT_UPLOAD */

    UINT16 SessionStatus;
    
    UINT16 ProtectionStatus;
    
    
    UINT32 Mta;
    
    UINT8 SendStatus;
}stXcpInfo_t;



class CanProtocol
{
    private:

    public:
    VCI_CAN_OBJ RequestCanMsg;
    TQueue *pXcpRequestQueue;

    stXcpInfo_t *stXcpInfo;
    XcpRequest_t XcpRequest; 

    CanProtocol()
    {
        stXcpInfo = new stXcpInfo_t;
        stXcpInfo->ResponseLen = 0;
        stXcpInfo->ResponseOk = 0;
        stXcpInfo->ResponseCmdCode = 0;
        stXcpInfo->RequestLen = 0;
        stXcpInfo->SessionStatus = 0;
        stXcpInfo->ProtectionStatus = 0;
        stXcpInfo->Mta = 0;
        stXcpInfo->SendStatus = 0;

        pXcpRequestQueue  = new TQueue;
    };

    ~CanProtocol()
    {
        delete stXcpInfo;
        delete pXcpRequestQueue;
    };
    
    void ParseAndHandleMsg(VCI_CAN_OBJ *pCanMsg);
    void XcpHandleResponseData(VCI_CAN_OBJ *pCanMsg);
    UINT16 XcpWriteRequest(UINT32 McuAddress, UINT16 Length, UINT16 *StoredAddress, UINT8 EepFlag);
    UINT16 XcpConfigMtaRequest(UINT32 McuAddress);
    UINT16 XcpReadRequest(UINT32 McuAddress, UINT16 Length, TEdit *pTEdit, UINT8 EepFlag);
    UINT16 XcpConnectRequest(void);
    UINT16 XcpSendData(VCI_CAN_OBJ *pCanMsg);
    
    void CreatCanThread(void *param);
    void XcpRead(void);
    void XcpWrite(void);
    void XcpUserCmd(XcpRequest_t *pXcpRequest);
    UINT16 XcpResetRequest(UINT16 SectionNo);
};

extern CanProtocol *CanProtocolObj;

/*********************************************************************************
* Global DECLARATIONS
*********************************************************************************/


#endif