/******************************************************************************************************
*            CAN_HandlerRTOS.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CAN_HandlerRTOS.c
*
*    DESCRIPTION: Portable code for CANopen in freeRTOS + stm32f10x
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/8/2 21:29:16
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Copy from canfestival CANOPEN stack and change some api to make it run in
*                                  freeRTOS and stm32f10x                 2018/8/2 21:31:12
******************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "TIM_Globals.h"
#include "CanOpenBasic.h"

#include "FreeRTOS.h"
#include "semphr.h"

/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Normal
  * @{
  */

/* Private typedef -----------------------------------------------------------*/



/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define     TX_BUF_LEN      10
#define     RX_BUF_LEN      10

/* Private variables ---------------------------------------------------------*/
CanTxMsg CanTxMsgBuf[TX_BUF_LEN];
uint8_t CanTxSaveIndex;
uint8_t CanTxReadIndex;

stCanMessage_t CanRxMsgBuf[TX_BUF_LEN];
uint8_t CanRxSaveIndex;
uint8_t CanRxReadIndex;

static SemaphoreHandle_t recv_sem = NULL;
static SemaphoreHandle_t tran_sem = NULL;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


static void f_CanBufferInit(void)
{
    /* transmit queue init */
    CanTxSaveIndex = 0;
    CanTxReadIndex = 0;
    /* receive  queue init */
    CanRxSaveIndex = 0;
    CanRxReadIndex = 0;
}

/**
  * @brief  can_slave_thread
  * @param  None
  * @retval : None
  */
void CAN_RxHandlerTask(void* parameter)
{
    recv_sem = xSemaphoreCreateBinary();

    if(recv_sem == NULL)
    {
        return;
    }

    f_CanBufferInit();

    TIM_Initial();

    /* Infinite loop*/
    while(1)
    {
        xSemaphoreTake(recv_sem, portMAX_DELAY);
        {
            uint32_t next;
            stCanMessage_t *pmsg;

            next = CanRxReadIndex;
            pmsg = &CanRxMsgBuf[next];

            TIM_DisableTimerInterrupt();
            CanOpenRxCmdHandler(pmsg);
            TIM_EnableTimerInterrupt();

            next++;
            if(next >= RX_BUF_LEN) next = 0;
            CanRxReadIndex = next;
        }
    }
}


void CAN_TxHandlerTask(void *parameter)
{
    tran_sem = xSemaphoreCreateBinary();

    if(tran_sem == NULL)
    {
        return;
    }
    CanOpenInitial();
    
    while(1)
    {
        xSemaphoreTake(tran_sem, portMAX_DELAY);
        {
            uint32_t next;
            uint8_t  mailbox_no;
            CanTxMsg *ptx_msg;

            next = CanTxReadIndex;
            ptx_msg = &CanTxMsgBuf[next];

            mailbox_no = CAN_Transmit(CAN2, ptx_msg);
            if(mailbox_no != CAN_NO_MB)
            {
                next++;
                if(next >= TX_BUF_LEN) next = 0;
                CanTxReadIndex = next;
            }
            else
            {
                //xSemaphoreGive(tran_sem);
            }
        }
    }
}


uint8_t CAN_RequestSend(stCanMessage_t *m)
{
    uint8_t    i;
    uint8_t  next;

    next = CanTxSaveIndex + 1;
    if(next >= TX_BUF_LEN)
    {
        next = 0;
    }

    if(next != CanTxReadIndex)
    {
        CanTxMsg *ptx_msg;

        ptx_msg = &CanTxMsgBuf[CanTxSaveIndex];
        ptx_msg->StdId = m->cob_id;

        if(m->rtr)
        {
            ptx_msg->RTR = CAN_RTR_REMOTE;
        }
        else
        {
            ptx_msg->RTR = CAN_RTR_DATA;
        }

        ptx_msg->IDE = CAN_ID_STD;
        ptx_msg->DLC = m->len;

        for(i = 0; i < m->len; i++)
        {
            ptx_msg->Data[i] = m->data[i];
        }

        CanTxSaveIndex = next;

        xSemaphoreGive(tran_sem);
        return 0x00;
    }

    return 0xFF;
}

void CAN2_RX1_IRQHandler(void)
{
    stCanMessage_t *pmsg;
    CanRxMsg RxMessage;
    uint32_t i, next;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);

    next = (CanRxSaveIndex + 1) ;
    if(next >= RX_BUF_LEN) next = 0;
    if(next == CanRxReadIndex) return;

    pmsg = &CanRxMsgBuf[CanRxSaveIndex];
    pmsg->cob_id = (uint16_t)RxMessage.StdId;      /**< message's ID */
    if(RxMessage.RTR == CAN_RTR_REMOTE)             /**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
    {
        pmsg->rtr = 1;
    }
    else
    {
        pmsg->rtr = 0;
    }

    pmsg->len  = (uint8_t)RxMessage.DLC;                 /**< message's length (0 to 8) */
    for(i = 0; i < pmsg->len; i++)
    {
        pmsg->data[i] = RxMessage.Data[i];
    }

    CanRxSaveIndex = next;

    xSemaphoreGiveFromISR(recv_sem, &xHigherPriorityTaskWoken);
    //portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
