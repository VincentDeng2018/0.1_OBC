/******************************************************************************************************
*            CanOpenSync.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CanOpenSync.c
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

void SyncInitial(void)
{
    /* TODO: map to OD when get OD ready */
    stCanOpenDate.SyncState = SYNC_STOP;
    stCanOpenDate.SyncWindowTime_ms = 5;
    stCanOpenDate.SyncCyclicTime_ms = 2000;
}


/****************************************************************************
*
*  Function: SyncHandlder
*
*  Purpose :    Send sync packet when there is any other node
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   As default, the synchronous window is 5ms, cyclic time 2s.
*                     Call every 1-ms
*
****************************************************************************/
void SyncHandlder(void)
{
    if(stCanOpenDate.SyncState == SYNC_START)
    {
        if(++stCanOpenDate.SyncCyclicCounter_ms >= stCanOpenDate.SyncCyclicTimer_ms)
        {
            if(stCanOpenDate.ReceivePdoFlag)
            {
                SendSyncMessage();
                stCanOpenDate.SyncCyclicCounter_ms = 0;
                stCanOpenDate.ReceivePdoFlag = 0;
            }
        }
        
        /* didn't receive PDO data within window time, then stop sync */
        if(  (stCanOpenDate.SyncCyclicCounter_ms >= stCanOpenDate.SyncWindowTime_ms)
          && (stCanOpenDate.ReceivePdoFlag == 0)
          )
        {
            
        }
    }
}


/****************************************************************************
*
*  Function: SyncHandlder
*
*  Purpose :    Receive sync packet trigger to send PDO
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   When recieve sync, then we should response within 1ms
*
****************************************************************************/
uint8_t ProcessSyncData(uint8_t counter)
{
    static uint8_t LastSyncCouter = 0xFF;
    
    /* don't response same sync request */
    /* TODO: Per CANOPEN, only response sync request when in operation mode.
       But delta product response in any mode. Double check it later*/
    if(counter == LastSyncCouter)
    {
        SendPdoData();
    }
    LastSyncCouter = counter;
}