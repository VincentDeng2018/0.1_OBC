/******************************************************************************************************
*            CanDriver.cpp
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
*    FILE NAME: CanDriver.cpp
*
*    DESCRIPTION: Initial CAN device and build up CAN operation API
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/11/1 14:42:28
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version
******************************************************************************************************/


/**********************************************************************************************
* Includes
**********************************************************************************************/
/* Standard lib include */
#include <cstdlib>
#include <iostream>
#include <queue>

using namespace std;
#include <string.h>
/* Chip lib include */
/* user include */
#include "CanProtocol.h"
#include "CanTxThread.h"
#include "CanTool_NPS.h"


/********************************************************************************
* Global DECLARATIONS
********************************************************************************/


/********************************************************************************
* Global VARIABLES
********************************************************************************/
CanProtocol *CanProtocolObj;

queue<XcpRequest_t>XcpRequestQueue;

/********************************************************************************
* LOCAL FUNCTION PROTOTYPES
********************************************************************************/
void ReceiveThread(void *param);
void XcpHandleThread(void *param);

/********************************************************************************
* LOCAL VARIABLES
********************************************************************************/
AnsiString asSysteStateString[BMS_INVALID + 1] =
{
    "Initial",
    "Idle",
    "Test Charge",
    "Test Discharge",
    "Charge On",
    "Discharge On",
    "Sleep",
    "Error Off",
    "Precharge",
    "Unknown"
};


void ReceiveThread(void *param)
{
    USHORT i = 0;

    AnsiString str,tmpstr;

    TListBox *box=(TListBox*)param;

    while(1)
    {
        /* Polling CAN Rx buffer */
        CanDriverObj->ReceiveCanMsg();
        if(CanDriverObj->ReceiveLength)
        {
            for(i = 0; i<CanDriverObj->ReceiveLength; i++)
            {
                CanProtocolObj->ParseAndHandleMsg(&CanDriverObj->ReceiveData[i]);
                if((CanDriverObj->ReceiveData[i].ID == 0x420180) || (CanDriverObj->ReceiveData[i].ID == 0x428001))
                {
                    str = "接收到数据帧:  ";
                    if(CanDriverObj->ReceiveData[i].TimeFlag==0)
                    {
                        tmpstr = "时间标识:无  ";
                    }
                    else
                    {
                        tmpstr="时间标识:0x"+IntToHex((int)CanDriverObj->ReceiveData[i].TimeStamp,1)+" ";
                    }
                    str += tmpstr;
                    tmpstr="帧ID:0x"+IntToHex((int)CanDriverObj->ReceiveData[i].ID,1)+" ";
                    str+=tmpstr;
                    str+="帧格式:";
                    if(CanDriverObj->ReceiveData[i].RemoteFlag==0)
                    {
                        tmpstr="数据帧 ";
                    }
                    else
                    {
                        tmpstr="远程帧 ";
                    }
                    str+=tmpstr;
                    str+="帧类型:";
                    if(CanDriverObj->ReceiveData[i].ExternFlag==0)
                    {
                        tmpstr="标准帧 ";
                    }
                    else
                    {
                        tmpstr="扩展帧 ";
                    }

                    str+=tmpstr;
                    box->Items->Add(str);
                    if(CanDriverObj->ReceiveData[i].RemoteFlag==0)
                    {
                        str="数据:";
                        if(CanDriverObj->ReceiveData[i].DataLen>8)
                        {
                            CanDriverObj->ReceiveData[i].DataLen=8;
                        }

                        for(int j=0;j<CanDriverObj->ReceiveData[i].DataLen;j++)
                        {
                            tmpstr=IntToHex((int)CanDriverObj->ReceiveData[i].Data[j],2)+" ";
                            str+=tmpstr;
                        }
                        box->Items->Add(str);
                    }
                }
            }
            box->ItemIndex = box->Items->Count-1;
        }
        Sleep(4);
    }
}


void XcpHandleThread(void *param)
{
    while(1)
    {
        while(!XcpRequestQueue.empty())
        {
            XcpRequest_t XcpRequestTemp = XcpRequestQueue.front();
            
            switch(XcpRequestTemp.CmdCode)
            {
                case CC_GET_ID:
                    break;
    
                case CC_SET_REQUEST:
                    break;
    
                case CC_GET_SEED:
                    break;
    
                case CC_UNLOCK:
                    break;
    
                case CC_SET_MTA:
                    break;
    
                case CC_UPLOAD:
                    break;
    
                case CC_SHORT_UPLOAD:
                    CanProtocolObj->XcpRead();
                    break;
                    
                case CC_USER_CMD:
                    CanProtocolObj->XcpUserCmd(&XcpRequestTemp);
                    break;
    
                default:
                    break;
            }
            XcpRequestQueue.pop();
            
            /* remove 10ms sleep when DSP side support xcp CAN queue */
            Sleep(10);
        }
        
        Sleep(5);
    }
}

void CanProtocol::CreatCanThread(void *param)
{
    _beginthread(ReceiveThread, 0, param);
    _beginthread(XcpHandleThread, 0, param);
}


void CanProtocol::ParseAndHandleMsg(VCI_CAN_OBJ *pCanMsg)
{
    float tempValue = 0.0f;
    UINT32 tempIntValue = 0;
    AnsiString asTemString;
    CANHeader_t CanMsgHeader = {0};

    /* currently only support data frame. Remote frame is not supported */
    if(pCanMsg->RemoteFlag == 0)
    {
        CanMsgHeader.all = pCanMsg->ID;

        //switch(CanMsgHeader.bits.type)

    }
}


void CanProtocol::XcpHandleResponseData(VCI_CAN_OBJ *pCanMsg)
{
    if((pCanMsg->Data[0] == stXcpInfo->ResponseCmdCode) && (pCanMsg->DataLen == stXcpInfo->ResponseLen))
    {
        stXcpInfo->ResponseOk = 1;

        memcpy(&stXcpInfo->XcpResponseData[0], &pCanMsg->Data[0], pCanMsg->DataLen);
    }
}


UINT16 CanProtocol::XcpSendData(VCI_CAN_OBJ *pCanMsg)
{
    RequestCanMsg.ID = 0x420180;
    RequestCanMsg.SendType = 0;
    RequestCanMsg.RemoteFlag = 0;
    RequestCanMsg.ExternFlag = 1;
    return (CanDriverObj->TransmitCanMsg(&RequestCanMsg));
    //return (CanTxThread->SendCanMsg(pCanMsg));
}

UINT16 CanProtocol::XcpWriteRequest(UINT32 McuAddress, UINT16 Length, UINT16 *StoredAddress, UINT8 EepFlag)
{
    XcpRequest_t XcpRequestQueTemp =  {0};

    XcpRequestQueTemp.McuAddress = McuAddress;
    XcpRequestQueTemp.Length = Length;
    
    memcpy(&XcpRequestQueTemp.Buffer[0], StoredAddress, Length);
    
    if(EepFlag)
    {
        XcpRequestQueTemp.CmdCode = CC_USER_CMD;
        XcpRequestQueTemp.CmdSubCode = CC_USER_EEP_WRITE;
    }
    else
    {
        /* TODO: Implement it later */
        XcpRequestQueTemp.CmdSubCode = 0;
    }
    
    XcpRequestQueue.push(XcpRequestQueTemp);

    /* TODO: add error handler later */
    return 1;
}


/* TODO: not verify yet */
void CanProtocol::XcpWrite(void)
{
    UINT16 TimeOut_ms = 0;
    UINT16 result = XCP_CMD_OK;

    if(stXcpInfo->SessionStatus != SS_CONNECTED)
    {
        result = XcpConnectRequest();
    }

    if(XCP_CMD_OK == result)
    {
        /************* Step 1: set MTA when using DOWNLOAD cmd****************/
        result = XcpConfigMtaRequest(XcpRequest.McuAddress);
        
        /************* Step 2: Write Request *************/
        if(XCP_CMD_OK == result)
        {
            RequestCanMsg.SendType = 0;
            RequestCanMsg.RemoteFlag = 0;
            RequestCanMsg.ExternFlag = 1;
            RequestCanMsg.DataLen = 8;

            RequestCanMsg.Data[0] = CC_DOWNLOAD;
            RequestCanMsg.Data[1] = XcpRequest.Length;

            memcpy(&RequestCanMsg.Data[2], XcpRequest.StoredAddress, XcpRequest.Length);


            stXcpInfo->ResponseOk = 0;
            XcpSendData(&RequestCanMsg);
            /* response data should be 1 byte */
            stXcpInfo->ResponseLen = 1u;
            stXcpInfo->ResponseCmdCode = PID_RES;

            /* wait response command */
            while(stXcpInfo->ResponseOk == 0)
            {
                /* wait 2 ms */
                Sleep(2);
                if(++TimeOut_ms > 5)
                {
                    result =  XCP_TIME_OUT;
                }
            }

            if(XCP_CMD_OK == result)
            {
                result = XCP_CMD_OK;
            }

        }
    }
}

UINT16 CanProtocol::XcpConfigMtaRequest(UINT32 McuAddress)
{
    UINT16 TimeOut_ms = 0;
    UINT16 result = XCP_TIME_OUT;
    UINT16 retryTime = 0;

    if(stXcpInfo->SessionStatus == SS_CONNECTED)
    {
        /* setup connection first */
        RequestCanMsg.Data[0] = CC_SET_MTA;
        RequestCanMsg.Data[1] = 0x0;
        RequestCanMsg.Data[2] = 0x0;
        RequestCanMsg.Data[3] = 0x0;
        RequestCanMsg.Data[4] = McuAddress & 0xFF;
        RequestCanMsg.Data[5] = (McuAddress & 0xFF00) >> 8;
        RequestCanMsg.Data[6] = (McuAddress & 0xFF0000) >> 16;
        RequestCanMsg.Data[7] = (McuAddress & 0xFF000000) >> 24;
        RequestCanMsg.DataLen = 8;

        stXcpInfo->ResponseOk = 0;
        /* response data should be 8 byte */
        stXcpInfo->ResponseLen = 1u;
        stXcpInfo->ResponseCmdCode = PID_RES;

        /* retry three time */
        do
        {
            XcpSendData(&RequestCanMsg);

            /* wait response command */
            while(stXcpInfo->ResponseOk == 0)
            {
                /* wait 2 ms */
                Sleep(2);
                if(++TimeOut_ms > 5u)
                {
                    break; /* break from while(stXcpInfo->ResponseOk == 0) */
                }
            }
        }
        while((retryTime++ < 3) && (stXcpInfo->ResponseOk == 0));

        /* there are other errors, add code here */
        if(stXcpInfo->ResponseOk)
        {
            result = XCP_CMD_OK;
        }
        else
        {
            result = XCP_ACCESS_DENIED;
        }
    }

    return result;
}



UINT16 CanProtocol::XcpReadRequest(UINT32 McuAddress, UINT16 Length, TEdit *pTEdit, UINT8 EepFlag)
{
    XcpRequest_t XcpRequestQueTemp =  {0};

    XcpRequestQueTemp.McuAddress = McuAddress;
    XcpRequestQueTemp.Length = Length;
    XcpRequestQueTemp.pTEdit = pTEdit;

    
    if(EepFlag)
    {
        XcpRequestQueTemp.CmdCode = CC_USER_CMD;
        XcpRequestQueTemp.CmdSubCode = CC_USER_EEP_READ;
    }
    else
    {
        XcpRequestQueTemp.CmdCode = CC_SHORT_UPLOAD;
        XcpRequestQueTemp.CmdSubCode = 0;
    }
    
    XcpRequestQueue.push(XcpRequestQueTemp);
    
    /* TODO: add error handler later */
    return 1;
}


UINT16 CanProtocol::XcpResetRequest(UINT16 SectionNo)
{
    XcpRequest_t XcpRequestQueTemp =  {0};

    XcpRequestQueTemp.Section = SectionNo;
    XcpRequestQueTemp.CmdCode = CC_USER_CMD;
    
    /* reset all */
    if(SectionNo == 0xFFFF)
    {
        XcpRequestQueTemp.CmdSubCode = CC_USER_EEP_RESET_ALL;
    }
    /* reset determinated section */
    else
    {
        XcpRequestQueTemp.CmdSubCode = CC_USER_EEP_RESET_SECTION;
    }
    
    XcpRequestQueue.push(XcpRequestQueTemp);
    
    /* TODO: add error handler later */
    return 1;
}


void CanProtocol::XcpRead(void)
{
    UINT16 TimeOut_ms = 0;
    UINT16 result = XCP_CMD_OK;
    UINT16 i = 0;

    if((stXcpInfo->SessionStatus & SS_CONNECTED) == 0u)
    {
        result = XcpConnectRequest();
    }

    if(XCP_CMD_OK == result)
    {
        /* continue if already connected */
        /* Note: for read, only support short upload mode */
        /************* Read with short upload mode ****************/
        RequestCanMsg.DataLen = 8;
        RequestCanMsg.Data[0] = CC_SHORT_UPLOAD;
        RequestCanMsg.Data[1] = XcpRequest.Length;
        RequestCanMsg.Data[2] = 0;
        RequestCanMsg.Data[3] = 0;
        RequestCanMsg.Data[4] = XcpRequest.McuAddress & 0xFF;
        RequestCanMsg.Data[5] = (XcpRequest.McuAddress & 0xFF00) >> 8;
        RequestCanMsg.Data[6] = (XcpRequest.McuAddress & 0xFF0000) >> 16;
        RequestCanMsg.Data[7] = (XcpRequest.McuAddress & 0xFF000000) >> 24;

        stXcpInfo->ResponseOk = 0;
        XcpSendData(&RequestCanMsg);
        /* response data should be 8 byte */
        stXcpInfo->ResponseLen = 8u;
        stXcpInfo->ResponseCmdCode = CC_SHORT_UPLOAD;

        /* wait response command */
        while(stXcpInfo->ResponseOk == 0)
        {
            /* wait 2 ms */
            Sleep(2);
            if(++TimeOut_ms > 5u)
            {
                result =  XCP_TIME_OUT;
                break;
            }
        }

        if(result == XCP_CMD_OK)
        {
            for(i = 0; i < (XcpRequest.Length >> 1); i++)
            {
                /*TODO: */
                XcpRequest.StoredAddress[i] = (stXcpInfo->XcpResponseData[i] << 8)
                                 | stXcpInfo->XcpResponseData[i + 1];
            }
            result = XCP_CMD_OK;
        }
        else
        {
            result = XCP_ACCESS_DENIED;
        }
    }

    XcpRequest.CmdCode = 0;
    //return result;
}

void CanProtocol::XcpUserCmd(XcpRequest_t *pXcpRequest)
{
    UINT16 TimeOut_ms = 0;
    UINT16 result = XCP_CMD_OK;

    if((stXcpInfo->SessionStatus & SS_CONNECTED) == 0u)
    {
        result = XcpConnectRequest();
    }

    if(XCP_CMD_OK == result)
    {
        /* continue if already connected */
        /* Note: for read, only support short upload mode */
        /************* Read with short upload mode ****************/
        RequestCanMsg.DataLen = 8;
        RequestCanMsg.Data[0] = CC_USER_CMD;
        RequestCanMsg.Data[1] = pXcpRequest->CmdSubCode;
        RequestCanMsg.Data[2] = pXcpRequest->McuAddress & 0xFF;
        RequestCanMsg.Data[3] = (pXcpRequest->McuAddress & 0xFF00) >> 8;
        if(pXcpRequest->CmdSubCode == CC_USER_EEP_READ)
        {
            RequestCanMsg.Data[4] = 0;
            RequestCanMsg.Data[5] = 0;
        }
        else if(pXcpRequest->CmdSubCode == CC_USER_EEP_WRITE)
        {
            RequestCanMsg.Data[4] = pXcpRequest->Buffer[0] & 0xFF;
            RequestCanMsg.Data[5] = pXcpRequest->Buffer[0] >> 8;
        }
        else if(pXcpRequest->CmdSubCode == CC_USER_EEP_RESET_SECTION)
        {
            RequestCanMsg.Data[6] = pXcpRequest->Buffer[0] & 0xFF;
            RequestCanMsg.Data[7] = pXcpRequest->Buffer[0] >> 8;
        }
        else if(pXcpRequest->CmdSubCode == CC_USER_EEP_RESET_ALL)
        {
            
        }
        RequestCanMsg.Data[6] = 0;
        RequestCanMsg.Data[7] = 0;

        stXcpInfo->ResponseOk = 0;
        XcpSendData(&RequestCanMsg);
        /* response data should be 8 byte */
        stXcpInfo->ResponseLen = 8u;
        stXcpInfo->ResponseCmdCode = PID_RES;

        /* wait response command */
        while(stXcpInfo->ResponseOk == 0)
        {
            /* wait 2 ms */
            Sleep(2);
            if(++TimeOut_ms > 5u)
            {
                result =  XCP_TIME_OUT;
                break;
            }
        }

        if(result == XCP_CMD_OK)
        {
            if(pXcpRequest->CmdSubCode == CC_USER_EEP_READ)
            {
                pXcpRequest->pTEdit->Text = (UINT16)stXcpInfo->XcpResponseData[5] + ((UINT16)stXcpInfo->XcpResponseData[4] << 8);
            }
            result = XCP_CMD_OK;
        }
        else
        {
            result = XCP_ACCESS_DENIED;
        }
    }
}


UINT16 CanProtocol::XcpConnectRequest(void)
{
    UINT16 TimeOut_ms = 0;
    UINT16 result = XCP_TIME_OUT;

    /* setup connection first */
    RequestCanMsg.Data[0] = CC_CONNECT;
    RequestCanMsg.Data[1] = 0x0;

    stXcpInfo->ResponseOk = 0;
    /* response data should be 8 byte */
    stXcpInfo->ResponseLen = 8u;
    stXcpInfo->ResponseCmdCode = PID_RES;
    RequestCanMsg.DataLen = 1;
    XcpSendData(&RequestCanMsg);

    /* wait response command */
    while(stXcpInfo->ResponseOk == 0)
    {
        /* wait 2 ms */
        Sleep(2);
        if(++TimeOut_ms > 5u)
        {
            break;
        }
    }

    /* there are other errors, add code here */
    if(stXcpInfo->ResponseOk)
    {
        result = XCP_CMD_OK;
        stXcpInfo->SessionStatus |= SS_CONNECTED;
    }
    else
    {
        result = XCP_ACCESS_DENIED;
        stXcpInfo->SessionStatus &= ~CC_CONNECT;
    }

    return result;
}


