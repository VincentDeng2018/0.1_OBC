//---------------------------------------------------------------------------

#ifndef CanTxThreadH
#define CanTxThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include "CanDriver.h"


//---------------------------------------------------------------------------
class SendCanMsgThread : public TThread
{
private:
    VCI_CAN_OBJ CanMsgBuf;
    UINT8 RequestSendFlag;
protected:
    void __fastcall Execute();
public:
    //TQueue    pCanTxQueue;
    __fastcall SendCanMsgThread(bool CreateSuspended);
    UINT16 SendCanMsg(VCI_CAN_OBJ *pCanMsg);
};

extern SendCanMsgThread *CanTxThread;
//---------------------------------------------------------------------------
#endif
