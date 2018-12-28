//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "CanTxThread.h"
#include "CanProtocol.h"
#include "CanTool_NPS.h"


#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall SendCanMsgThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
SendCanMsgThread *CanTxThread;

__fastcall SendCanMsgThread::SendCanMsgThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
    memset(&CanMsgBuf, 0, sizeof(VCI_CAN_OBJ));
    RequestSendFlag = 0;
    Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall SendCanMsgThread::Execute()
{
    //---- Place thread code here ----
    USHORT i = 0;
    
    AnsiString str,tmpstr;
    
    TListBox *box = MainForm->ListBox_CanMsg;
    
    while(1)
    {
        /* Polling RX */
        CanDriverObj->ReceiveCanMsg(NULL);
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
        Sleep(5);
    }
}


UINT16 SendCanMsgThread::SendCanMsg(VCI_CAN_OBJ *pCanMsg)
{
    if(RequestSendFlag == 0)
    {
        memcpy(&CanMsgBuf, pCanMsg, sizeof(VCI_CAN_OBJ));
        RequestSendFlag = 1;
        
        return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------
