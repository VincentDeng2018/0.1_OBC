/******************************************************************************************************
*            SM_CommonApi.h
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
*    FILE NAME: SM_CommonApi.h
*
*    DESCRIPTION: define all the state-machine configuration parameters. Refer product specification
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/4/19 13:21:54
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/4/19 13:21:56
******************************************************************************************************/

#ifndef _SM_COMMONAPI_H
#define _SM_COMMONAPI_H

#include <stdint.h>

/**********************************************************************************************
* Macros
**********************************************************************************************/

typedef struct
{
    /* Bit0-Bit3*/
    uint32_t BattOV:1;
    uint32_t BattUV:1;
    uint32_t BattAbsUV:1;   /* can't recover */
    uint32_t Reserved1:1;
    
    /* Bit4-Bit7*/
    uint32_t BattOT:1;
    uint32_t BattUT:1;
    uint32_t ChgBoardOT: 1;
    uint32_t RelayFail: 1;
    
    /* Bit8-Bit11*/
    uint32_t FanFail: 1;
    uint32_t ChargeOC: 1;
    uint32_t AcLow: 1;
    uint32_t AcOff: 1;
    
    /* Bit12-Bit15*/
    uint32_t BattConnectOk: 1;
    uint32_t BattConnectReverse: 1;
    uint32_t TestModeFinish: 1;
    uint32_t EnterTest:1;
        
    /* Bit16-Bit31*/
    uint32_t AllowStayChg:1;
    uint32_t AllowEnterChg:1;
    uint32_t Reserved2: 14;
}stSystemStateBits_t;

extern stSystemStateBits_t stSystemStateBits;

typedef struct
{
    int16_t BattAbsUv_Cnt;
    int16_t BattUv_Cnt;
    int16_t BattOv_Cnt;
    int16_t FanFail_Cnt;

    int16_t RelayFail_Cnt;
    int16_t BattOT_Cnt;
    int16_t BattUT_Cnt;
    int16_t ChargeOC_Cnt;
    
    int16_t ChgBoardOT_Cnt;
    
    int16_t AcLow_Cnt;
    int16_t AcOff_Cnt;
    int16_t BattConnectOk_Cnt;
    int16_t BattConnectReverse_Cnt;
}stStatusCounter_t;

extern stStatusCounter_t stStatusCounter;

typedef struct
{
    uint8_t U_Type;
    uint8_t I_Type;
}stProductInfo_t;
extern stProductInfo_t stProductInfo;

extern const uint16_t awdVoltTypeValue[VOLTAGE_TYPE_NUM];
extern const uint16_t awdCurTypeValue[CURRENT_TYPE_NUM];
/**********************************************************************************************
* Global Functions
**********************************************************************************************/
extern void SM_SystemStateBitsUpdate(void);

#endif
