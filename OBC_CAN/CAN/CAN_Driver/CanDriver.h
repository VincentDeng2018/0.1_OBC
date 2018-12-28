/******************************************************************************************************
*            CanDriver.h
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
*    FILE NAME: CanDriver.h
*
*    DESCRIPTION: Header file for Can driver
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/11/1 14:37:26
*
*             Version   Author     Modification
*    HISTORY:  V1.0      DYH       Initial Version
******************************************************************************************************/

/**********************************************************************************************
* Includes
**********************************************************************************************/
#ifndef _CANDRIVER_H
#define _CANDRIVER_H

#include "ControlCAN.h"
#include <stdio.h>
#include <process.h>

/* Chip lib include */
/* user include */

/*********************************************************************************
* Macros
*********************************************************************************/
#define CAN_DRIVER_MAJOR_VERSION 1u 
#define CAN_DRIVER_MINOR_VERSION 0u 

#define DEBUG_TEST
/*********************************************************************************
* Data and Structure
*********************************************************************************/
class CanDriver
{
    private:

    public:
    BYTE ConnectAndStart;
    SHORT  ReceiveLength;
    VCI_CAN_OBJ ReceiveData[2500];

    USHORT DeviceBrand;
    DWORD DeviceType;
    DWORD DeviceInd; /* Device index if more than one device is plug-in, otherwise, always 0 */
    DWORD CANInd;    /* CAN channel */
    VCI_INIT_CONFIG DeviceInitConfig;
    
    HINSTANCE__* DllHandler;   /* store CAN dll hander */

    /* CAN API pointer */
    LPVCI_OpenDevice VCI_OpenDevice;
    LPVCI_CloseDevice VCI_CloseDevice;
    LPVCI_InitCan VCI_InitCAN;
    LPVCI_ReadBoardInfo VCI_ReadBoardInfo;

    LPVCI_ReadErrInfo VCI_ReadErrInfo;
    LPVCI_ReadCanStatus VCI_ReadCanStatus;
    LPVCI_GetReference VCI_GetReference;
    LPVCI_SetReference VCI_SetReference;

    LPVCI_GetReceiveNum VCI_GetReceiveNum;
    LPVCI_ClearBuffer VCI_ClearBuffer;
    LPVCI_StartCAN VCI_StartCAN;
    LPVCI_ResetCAN VCI_ResetCAN;

    LPVCI_Transmit VCI_Transmit;
    LPVCI_Receive VCI_Receive;
    LPVCI_GetReference2 VCI_GetReference2;
    LPVCI_SetReference2 VCI_SetReference2;

    LPVCI_ResumeConfig VCI_ResumeConfig;
    LPVCI_ConnectDevice VCI_ConnectDevice;
    LPVCI_UsbDeviceReset VCI_UsbDeviceReset;
    LPVCI_FindUsbDevice VCI_FindUsbDevice;
    FILE *stream;

    CanDriver()
    {
        ConnectAndStart = 0;
        ReceiveLength = 0;
    };

    ~CanDriver()
    {

    };

    USHORT ConfigCanDevice(USHORT BaudTypeCfg, USHORT DeviceBrandCfg, DWORD newDeviceTypeCfg, DWORD DeviceIndCfg, DWORD CANIndCfg);
    void ReleaseCanDevice(void);
    void ReceiveCanMsg(void);
    USHORT TransmitCanMsg(VCI_CAN_OBJ *pMsg);
};

extern CanDriver *CanDriverObj;

/*********************************************************************************
* Global DECLARATIONS
*********************************************************************************/


#endif