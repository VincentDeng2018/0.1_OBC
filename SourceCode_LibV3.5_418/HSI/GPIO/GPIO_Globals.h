/******************************************************************************************************
*            GPIO_Globals.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: GPIO_Globals.h
*
*    DESCRIPTION: Header file for task define
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:48:19
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                      2018/5/24 21:48:23
******************************************************************************************************/

#ifndef _GPIO_GLOBALS_H
#define _GPIO_GLOBALS_H

#include "stm32f10x_gpio.h"
#include "GPIO_Config.h"
/**********************************************************************************************
* Macros
**********************************************************************************************/
#define RELAY_ON   1u
#define RELAY_OFF  0u


#define POWER_ON   1u
#define POWER_OFF  0u


#define FAN_ON   1u
#define FAN_OFF  0u

typedef enum
{
    LED_OFF,
    LED_ON,
    LED_TOGGLE
}enLedState;

extern uint8_t CTRL_RelayCmd;
extern uint8_t CTRL_OutRelayCmd;
extern uint8_t CTRL_ChgPowerCmd;
extern uint8_t CTRL_FanCmd;
extern uint8_t CTRL_MosCmd;

/**********************************************************************************************
* Global Functions
**********************************************************************************************/
static inline uint8_t AcLowActive(void)
{
    return (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0u);
}



static inline uint8_t AcOffActive(void)
{
    return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) == 1u);
}


static inline void ControlRelay(uint8_t CloseRelay)
{
    if(CloseRelay)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_3);
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_3);
    }

    CTRL_RelayCmd = CloseRelay;
}

static inline void RedLedControl(enLedState newState)
{
    if(newState == LED_OFF)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_2);
    }
    else if(newState == LED_ON)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_2);
    }
    else
    {
        GPIO_TogglePin(GPIOB, GPIO_Pin_2);
    }
}


static inline void YellowLedControl(enLedState newState)
{
    if(newState == LED_OFF)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    }
    else if(newState == LED_ON)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_10);
    }
    else
    {
        GPIO_TogglePin(GPIOB, GPIO_Pin_10);
    }
}

static inline void GreenLedControl(enLedState newState)
{
    if(newState == LED_OFF)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    }
    else if(newState == LED_ON)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_11);
    }
    else
    {
        GPIO_TogglePin(GPIOB, GPIO_Pin_11);
    }
}


static inline void ControlOutRelay(uint8_t relayState)
{
    if(relayState == RELAY_OFF)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_15);
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_15);
    }
    
    CTRL_OutRelayCmd = relayState;
}


static inline void ControlChgPowerOn(uint8_t powerState)
{
    if(powerState == POWER_OFF)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_6);
    }
    else
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_6);
    }
    
    CTRL_ChgPowerCmd = powerState;
}


static inline void ControlMos(uint8_t MosState)
{
    if(MosState == RELAY_OFF)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    }
    else
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_8);
    }
    
    CTRL_MosCmd = MosState;
}


static inline uint8_t BatteryNormalConnected(void)
{
    return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0);
}


static inline uint8_t BatteryReverseConnected(void)
{
    return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0);
}

static inline void ControlFan(uint8_t FanState)
{
    if(FanState == FAN_OFF)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_4);
    }
    
    CTRL_FanCmd = FanState;
}

extern void GPIO_Initial(void);


#endif
