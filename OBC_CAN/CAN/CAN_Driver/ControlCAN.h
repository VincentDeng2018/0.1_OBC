#ifndef CONTROLCAN_H
#define CONTROLCAN_H

////文件版本：v2.00 20150920
//#include <cvidef.h>   //使用CVI平台开发，请使用该语句。
#include <vcl.h>
/* ERROR Type */
#define DEVICE_INIT_OK  0
#define DLL_NOT_EXIST  1
#define DEVICE_OPEN_FAIL  2     /* Device not correct */
#define DEVICE_INITIAL_FAIL  3  /* Device correct, but channel not ok */
#define DEVICE_START_FAIL  4  /* Device correct, but channel not ok */

//接口卡类型定义

#define ZLG_DEVICE      0
#define ZHCX_DEVICE     1

#define VCI_USBCAN1     3
#define VCI_USBCAN2     4
#define VCI_USBCAN2A        4

#define VCI_USBCAN_E_U      20
#define VCI_USBCAN_2E_U     21



//函数调用返回状态值
#define STATUS_OK                   1
#define STATUS_ERR                  0
    
/*------------------------------------------------兼容ZLG的函数及数据类型------------------------------------------------*/
//1.ZLGCAN系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
        USHORT  hw_Version;
        USHORT  fw_Version;
        USHORT  dr_Version;
        USHORT  in_Version;
        USHORT  irq_Num;
        BYTE    can_Num;
        CHAR    str_Serial_Num[20];
        CHAR    str_hw_Type[40];
        USHORT  Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
    UINT    ID;
    UINT    TimeStamp;
    BYTE    TimeFlag;
    BYTE    SendType;
    BYTE    RemoteFlag;//是否是远程帧
    BYTE    ExternFlag;//是否是扩展帧
    BYTE    DataLen;
    BYTE    Data[8];
    BYTE    Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义初始化CAN的数据类型
typedef struct _VCI_INIT_CONFIG{
    DWORD   AccCode;
    DWORD   AccMask;
    DWORD   Reserved;
    UCHAR   Filter;
    UCHAR   Timing0;    
    UCHAR   Timing1;    
    UCHAR   Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

///////// new add struct for filter /////////
typedef struct _VCI_FILTER_RECORD{
    DWORD ExtFrame; //是否为扩展帧
    DWORD Start;
    DWORD End;
}VCI_FILTER_RECORD,*PVCI_FILTER_RECORD;

//#if 0
//#define EXTERNC     extern "C"
//
//EXTERNC DWORD __stdcall VCI_OpenDevice(DWORD DeviceType,DWORD DeviceInd,DWORD Reserved);
//EXTERNC DWORD __stdcall VCI_CloseDevice(DWORD DeviceType,DWORD DeviceInd);
//EXTERNC DWORD __stdcall VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);
//
//EXTERNC DWORD __stdcall VCI_ReadBoardInfo(DWORD DeviceType,DWORD DeviceInd,PVCI_BOARD_INFO pInfo);
//
//EXTERNC DWORD __stdcall VCI_SetReference(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);
//
//EXTERNC ULONG __stdcall VCI_GetReceiveNum(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//EXTERNC DWORD __stdcall VCI_ClearBuffer(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//
//EXTERNC DWORD __stdcall VCI_StartCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//EXTERNC DWORD __stdcall VCI_ResetCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//
//EXTERNC ULONG __stdcall VCI_Transmit(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,ULONG Len);
//EXTERNC ULONG __stdcall VCI_Receive(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime);
//#endif

/*------------------------------------------------其他补充函数及数据结构描述------------------------------------------------*/

//USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
typedef  struct  _VCI_BOARD_INFO1{
    USHORT  hw_Version;
    USHORT  fw_Version;
    USHORT  dr_Version;
    USHORT  in_Version;
    USHORT  irq_Num;
    BYTE    can_Num;
    BYTE    Reserved;
    CHAR    str_Serial_Num[8];
    CHAR    str_hw_Type[16];
    CHAR    str_Usb_Serial[4][4];
} VCI_BOARD_INFO1,*PVCI_BOARD_INFO1;

//USB-CAN总线适配器板卡信息的数据类型2，该类型为VCI_FindUsbDevice函数的返回参数。为扩展更多的设备
typedef  struct  _VCI_BOARD_INFO2{
    USHORT  hw_Version;
    USHORT  fw_Version;
    USHORT  dr_Version;
    USHORT  in_Version;
    USHORT  irq_Num;
    BYTE    can_Num;
    BYTE    Reserved;
    CHAR    str_Serial_Num[8];
    CHAR    str_hw_Type[16];
    CHAR    str_Usb_Serial[10][4];
} VCI_BOARD_INFO2,*PVCI_BOARD_INFO2;

//定义错误信息的数据类型。    
typedef struct _ERR_INFO{     
    UINT	ErrCode;             //#if 0
    BYTE	Passive_ErrData[3];  //#define EXTERNC     extern "C"
    BYTE	ArLost_ErrData;      //
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
    UCHAR	ErrInterrupt;
    UCHAR	regMode;
    UCHAR	regStatus;
    UCHAR	regALCapture;
    UCHAR	regECCapture;
    UCHAR	regEWLimit;
    UCHAR	regRECounter;
    UCHAR	regTECounter;
    DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//定义常规参数类型
typedef struct _REF_NORMAL{
    BYTE Mode;		 //工作模式
    BYTE Filter;		 //滤波方式
    DWORD AccCode;		//接收滤波验收码
    DWORD AccMask;		//接收滤波屏蔽码
    BYTE kBaudRate;		//波特率索引号，0-SelfDefine,1-5Kbps(未用),2-18依次为：10kbps,20kbps,40kbps,50kbps,80kbps,100kbps,125kbps,200kbps,250kbps,400kbps,500kbps,666kbps,800kbps,1000kbps,33.33kbps,66.66kbps,83.33kbps
    BYTE Timing0;
    BYTE Timing1;
    BYTE CANRX_EN;		//保留，未用
    BYTE UARTBAUD;		//保留，未用
}VCI_REF_NORMAL,*PVCI_REF_NORMAL;

//定义波特率设置参数类型
typedef struct _BAUD_TYPE{
    DWORD Baud;		//存储波特率实际值
    BYTE SJW;		//同步跳转宽度，取值1-4
    BYTE BRP;		//预分频值，取值1-64
    BYTE SAM;		//采样点，取值0=采样一次，1=采样三次
    BYTE PHSEG2_SEL;	//相位缓冲段2选择位，取值0=由相位缓冲段1时间决定,1=可编程
    BYTE PRSEG;		//传播时间段，取值1-8
    BYTE PHSEG1;		//相位缓冲段1，取值1-8
    BYTE PHSEG2;		//相位缓冲段2，取值1-8
}VCI_BAUD_TYPE,*PVCI_BAUD_TYPE;

//定义Reference参数类型
typedef struct _REF_STRUCT{
    VCI_REF_NORMAL RefNormal;
    BYTE Reserved;
    VCI_BAUD_TYPE BaudType;
}VCI_REF_STRUCT,*PVCI_REF_STRUCT;

//EXTERNC DWORD __stdcall VCI_GetReference2(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD Reserved,BYTE *pData);
//EXTERNC DWORD __stdcall VCI_SetReference2(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,BYTE *pData);
//
//
//EXTERNC DWORD __stdcall VCI_ConnectDevice(DWORD DevType,DWORD DevIndex);
//EXTERNC DWORD __stdcall VCI_UsbDeviceReset(DWORD DevType,DWORD DevIndex,DWORD Reserved);
//EXTERNC DWORD __stdcall VCI_FindUsbDevice(PVCI_BOARD_INFO1 pInfo);
//EXTERNC DWORD __stdcall VCI_FindUsbDevice2(PVCI_BOARD_INFO2 pInfo);
//#endif

/* typedef CAN driver API function call */
typedef DWORD (CALLBACK*  LPVCI_OpenDevice)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_CloseDevice)(DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_InitCan)(DWORD,DWORD,DWORD,PVCI_INIT_CONFIG);

typedef DWORD (CALLBACK*  LPVCI_ReadBoardInfo)(DWORD,DWORD,PVCI_BOARD_INFO);
typedef DWORD (CALLBACK*  LPVCI_ReadErrInfo)(DWORD,DWORD,DWORD,PVCI_ERR_INFO);
typedef DWORD (CALLBACK*  LPVCI_ReadCanStatus)(DWORD,DWORD,DWORD,PVCI_CAN_STATUS);

typedef DWORD (CALLBACK*  LPVCI_GetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);
typedef DWORD (CALLBACK*  LPVCI_SetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);

typedef ULONG (CALLBACK*  LPVCI_GetReceiveNum)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_ClearBuffer)(DWORD,DWORD,DWORD);

typedef DWORD (CALLBACK*  LPVCI_StartCAN)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_ResetCAN)(DWORD,DWORD,DWORD);

typedef ULONG (CALLBACK*  LPVCI_Transmit)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ,ULONG);
typedef ULONG (CALLBACK*  LPVCI_Receive)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ,ULONG,INT);

typedef DWORD (CALLBACK*  LPVCI_GetReference2)(DWORD,DWORD,DWORD,DWORD,PVCI_REF_STRUCT);
typedef DWORD (CALLBACK*  LPVCI_SetReference2)(DWORD,DWORD,DWORD,DWORD,PVOID);
typedef DWORD (CALLBACK*  LPVCI_ResumeConfig)(DWORD,DWORD,DWORD);

typedef DWORD (CALLBACK*  LPVCI_ConnectDevice)(DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_UsbDeviceReset)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_FindUsbDevice)(PVCI_BOARD_INFO1);

#endif
