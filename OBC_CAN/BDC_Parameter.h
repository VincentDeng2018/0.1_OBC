//----------------------------------------------------------------------------
#ifndef __BDC_PARAMETER_H
#define __BDC_PARAMETER_H

typedef enum
{
    ID_Boost_CurrentGain_x100 = 0u,
    ID_Boost_VoltGain_x100,
    ID_Boost_VoltDiffGain_x1000,
    ID_Buck_CurrentGain_x100,
    ID_Buck_VoltGain_x100,
    ID_Buck_VoltDiffGain_x1000,
    ID_DC_AmpDiffGain_x1000,
    ID_U_HvTarget_100mV,
    ID_U_LvTarget_100mV,
    ID_U_BattCvTarget_100mV,
    ID_F_LvLlcStable_kHz,
    ID_F_LvLlcStart_kHz,
    ID_Duty_LvLlcStart_Percent,
    ID_t_LvLlcRamp_ms,
    ID_t_LvV48Ramp_ms,
    ID_F_HvLlcStable_kHz,
    ID_F_HvLlcStart_kHz,
    ID_Duty_HvLlcStart_Percent,
    ID_t_HvLlcRamp_ms,
    ID_U_BattCcTarget_100mA,
    ID_PowerDischargeRating_W,
    ID_PMaxOutPowerShortTerm_W,
    ID_PowerChargeRating_W,
    ID_END
}enBdcParameter_t;

typedef struct
{
    UINT32 EepAddres;
    UINT16 CurrentValue;
}stBdcParaValue_t;
//----------------------------------------------------------------------------
#endif    
