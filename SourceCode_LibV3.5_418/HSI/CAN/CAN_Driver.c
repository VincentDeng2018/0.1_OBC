/******************************************************************************************************
*            CAN_Driver.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CAN_Driver.c
*
*    DESCRIPTION: Define task related API, array, etc
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:51:20
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/5/24 21:51:24
******************************************************************************************************/

#include "CAN_Globals.h"
#include "stm32f10x_conf.h"

/*********************************************************************************
* Global Functions
*********************************************************************************/


/*********************************************************************************
* Task Call back Functions
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
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;

void CAN_Initial(void)
{
    /* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
    CAN2_Config(SET_CAN_SJW,SET_CAN_BS1,SET_CAN_BS2,SET_CAN_PRES);
}


/**
  * @brief  Initializes a Rx Message.
  * @param  CanRxMsg *RxMessage.
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
    uint8_t i = 0;

    RxMessage->StdId = 0;
    RxMessage->ExtId = 0;
    RxMessage->IDE = CAN_ID_STD;
    RxMessage->DLC = 0;
    RxMessage->FMI = 0;
    for (i = 0; i < 8; i++)
    {
        RxMessage->Data[i] = 0;
    }
}



/****************************************************************************
*
*  Function: CAN1_Config()
*
*  Purpose :    Initial CAN1
*
*  Parms Passed   :   sjw, bs1, bs2, pres: use these to determin baud rate
*
*  Returns        :   Nothing
*
*  Description    :
*
****************************************************************************/
void CAN1_Config(uint8_t sjw, uint8_t bs1, uint8_t bs2,uint16_t pres)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ∩辰?aGPIO那㊣?車?⊿AFIO那㊣?車㏒?CAN那㊣?車 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);


    /* CAN1 RX PB8 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* CAN1 TX PB9 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);  // CAN1 remap

    /* CAN1 Enabling interrupt */
    NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);

    CAN_InitStructure.CAN_TTCM=DISABLE;
    CAN_InitStructure.CAN_ABOM=DISABLE;
    CAN_InitStructure.CAN_AWUM=DISABLE;
    CAN_InitStructure.CAN_NART=DISABLE;
    CAN_InitStructure.CAN_RFLM=DISABLE;
    CAN_InitStructure.CAN_TXFP=DISABLE;
    CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
    //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW=sjw;
    CAN_InitStructure.CAN_BS1=bs1;
    CAN_InitStructure.CAN_BS2=bs2;
    CAN_InitStructure.CAN_Prescaler=pres;


    CAN_Init(CAN1, &CAN_InitStructure);	// CAN1

    CAN_FilterInitStructure.CAN_FilterNumber=0;
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // ㊣那那?﹞??芍㊣????㏒那?
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32??1y???¯
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			// 1y???¯㊣那那?﹞?
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 1y???¯?芍㊣?㊣那那?﹞?
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0???辰1y???¯
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  // CAN1
}


/****************************************************************************
*
*  Function: CAN1_Config()
*
*  Purpose :    Initial CAN1
*
*  Parms Passed   :   sjw, bs1, bs2, pres: use these to determin baud rate
*
*  Returns        :   Nothing
*
*  Description    :
*
****************************************************************************/
void CAN2_Config(uint8_t sjw,uint8_t bs1,uint8_t bs2,uint16_t pres)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ∩辰?aGPIO那㊣?車?⊿AFIO那㊣?車㏒?CAN那㊣?車 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    /*Also enable CAN1 clock, because CAN1 clock also control CAN2 rx filter */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    /* CAN2 RX PB12 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* CAN2 TX PB13 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* CAN2 Enabling interrupt */
    NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX1_IRQn;	// FIFO_1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
    CAN_DeInit(CAN2);
    CAN_StructInit(&CAN_InitStructure);

    CAN_InitStructure.CAN_TTCM=DISABLE;
    CAN_InitStructure.CAN_ABOM=DISABLE;
    CAN_InitStructure.CAN_AWUM=DISABLE;
    CAN_InitStructure.CAN_NART=DISABLE;
    CAN_InitStructure.CAN_RFLM=DISABLE;
    CAN_InitStructure.CAN_TXFP=DISABLE;
    CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
    //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW=sjw;
    CAN_InitStructure.CAN_BS1=bs1;
    CAN_InitStructure.CAN_BS2=bs2;
    CAN_InitStructure.CAN_Prescaler=pres;

    CAN_Init(CAN2,&CAN_InitStructure);   // CAN2

    /* CAN2 rx filter start from 14 */
    CAN_FilterInitStructure.CAN_FilterNumber = 14;	//
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	 // ㊣那那?﹞??芍㊣????㏒那?
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;   // 32??1y???¯
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;			// 1y???¯㊣那那?﹞?
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;		// 1y???¯?芍㊣?㊣那那?﹞?
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO1;	 // FIFO1???辰1y???¯
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);  // CAN2
}



/****************************************************************************
*
*  Function: CAN1_Config()
*
*  Purpose :    Initial CAN1
*
*  Parms Passed   :   sjw, bs1, bs2, pres: use these to determin baud rate
*
*  Returns        :   Nothing
*
*  Description    :
*
****************************************************************************/
void CAN_SendData(CAN_TypeDef* CANx,CanTxMsg* CanData)
{
    uint8_t retrys = 0;
    uint8_t mailbox = 0;

    do
    {
        mailbox = CAN_Transmit(CANx,CanData);
        retrys++;
    }
    while((mailbox == CAN_TxStatus_NoMailBox) && (retrys < 0xFE));

    retrys = 0;
}
