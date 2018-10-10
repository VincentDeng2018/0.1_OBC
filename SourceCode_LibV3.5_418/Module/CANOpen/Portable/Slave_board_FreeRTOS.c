/******************************************************************************************************
*            Slave_board_FreeRTOS.c
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: Slave_board_FreeRTOS.h
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
#include "platform_config.h"

/* test_master */
#include "canfestival.h"
#include "Master.h"
#include "Slave.h"
#include "TestMasterSlave.h"

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
/* Private variables ---------------------------------------------------------*/
CanTxMsg TxMessage;
CanRxMsg RxMessage;

/* Private function prototypes -----------------------------------------------*/
static int  test_slave(void);

extern void TIM5_start(void);

#define     RX_BUF_LEN      10
Message   rx_msg_buf[RX_BUF_LEN];
uint32_t    rx_save, rx_read;

#define TX_BUF_LEN      10
uint32_t tx_save, tx_read;
CanTxMsg tx_msg_buf[TX_BUF_LEN];

static SemaphoreHandle_t recv_sem = NULL;
static SemaphoreHandle_t tran_sem = NULL;



/* Private functions ---------------------------------------------------------*/

/**
  * @brief  can_slave_thread
  * @param  None
  * @retval : None
  */
void can_slave_init(void)
{
    /* transmit queue init */
    tx_save = 0;
    tx_read = 0;
    /* receive  queue init */
    rx_save = 0;
    rx_read = 0;
}

/**
  * @brief  can_slave_thread
  * @param  None
  * @retval : None
  */
void can_recv_thread(void* parameter)
{
    recv_sem = xSemaphoreCreateBinary();
    tran_sem = xSemaphoreCreateBinary();

    if((recv_sem == NULL) || (tran_sem == NULL))
    {
        return;
    }

    can_slave_init();

    test_slave();

    /* Infinite loop*/
    while(1)
    {
        xSemaphoreTake(recv_sem, portMAX_DELAY);
        {
            uint32_t next;
            Message *pmsg;

            next = rx_read;
            pmsg = &rx_msg_buf[next];

            /* Disable the Interrupt sources */
            TIM5->DIER &= (uint16_t)~TIM_IT_CC1;
            canDispatch(&TestSlave_Data, pmsg);
            /* Enable the Interrupt sources */
            TIM5->DIER |= TIM_IT_CC1;

            next++;
            if(next >= RX_BUF_LEN) next = 0;
            rx_read = next;
        }
    }
}

void can_send_thread(void *parameter)
{
    //setState(&TestSlave_Data, Preparing);
    while(1)
    {
        xSemaphoreTake(tran_sem, portMAX_DELAY);
        {
            uint32_t next;
            uint8_t  mailbox_no;
            CanTxMsg *ptx_msg;

            next = tx_read;
            ptx_msg = &tx_msg_buf[next];

            mailbox_no = CAN_Transmit(CAN2, ptx_msg);
            if(mailbox_no != CAN_NO_MB)
            {
                next++;
                if(next >= TX_BUF_LEN) next = 0;
                tx_read = next;
            }
            else
            {
                xSemaphoreGive(tran_sem);
            }
        }
    }
}


#include <applicfg.h>
unsigned char canSend(CAN_PORT notused, Message *m)
{
    uint32_t    i;
    uint32_t  next;

    next = tx_save + 1;
    if(next >= TX_BUF_LEN)
    {
        next = 0;
    }
    
    if(next != tx_read)
    {
        CanTxMsg *ptx_msg;

        ptx_msg = &tx_msg_buf[tx_save];
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
        
        tx_save = next;

        xSemaphoreGive(tran_sem);
        return 0x00;
    }

    return 0xFF;
}

void CAN2_RX1_IRQHandler(void)
{
    Message *pmsg;
    uint32_t i, next;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);

    next = (rx_save + 1) ;
    if(next >= RX_BUF_LEN) next = 0;
    if(next == rx_read) return;

    pmsg = &rx_msg_buf[rx_save];
    pmsg->cob_id = (UNS16)RxMessage.StdId;      /**< message's ID */
    if(RxMessage.RTR == CAN_RTR_REMOTE)             /**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
    {
        pmsg->rtr = 1;
    }
    else
    {
        pmsg->rtr = 0;
    }

    pmsg->len  = (UNS8)RxMessage.DLC;                 /**< message's length (0 to 8) */
    for(i = 0; i < pmsg->len; i++)
    {
        pmsg->data[i] = RxMessage.Data[i];
    }

    rx_save = next;

    xSemaphoreGiveFromISR(recv_sem, &xHigherPriorityTaskWoken);
    //portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

static TimerCallback_t init_callback;

s_BOARD SlaveBoard  = {"0", "1M"};


/***************************  INIT  *****************************************/
void InitNodes(CO_Data* d, UNS32 id)
{
    /****************************** INITIALISATION SLAVE *******************************/
    if(strcmp(SlaveBoard.baudrate, "none")) {
        setNodeId(&TestSlave_Data, 0x02);

        /* init */
        setState(&TestSlave_Data, Initialisation);
    }
}

static StartTimerLoop(TimerCallback_t _init_callback)
{
    init_callback = _init_callback;

    SetAlarm(NULL, 0, init_callback, 0, 0);
    TIM5_start();
}

/***************************  EXIT  *****************************************/
void Exit(CO_Data* d, UNS32 id)
{

}

/****************************************************************************/
/*****************************  test_salve  *********************************/
/****************************************************************************/
static int test_slave(void)
{
    if(strcmp(SlaveBoard.baudrate, "none"))
    {
        TestSlave_Data.heartbeatError = TestSlave_heartbeatError;
        TestSlave_Data.initialisation = TestSlave_initialisation;
        TestSlave_Data.preOperational = TestSlave_preOperational;
        TestSlave_Data.operational = TestSlave_operational;
        TestSlave_Data.stopped   = TestSlave_stopped;
        TestSlave_Data.post_sync = TestSlave_post_sync;
        TestSlave_Data.post_TPDO = TestSlave_post_TPDO;
        TestSlave_Data.storeODSubIndex = TestSlave_storeODSubIndex;
        TestSlave_Data.post_emcy = TestSlave_post_emcy;
    }

    // Start timer thread
    StartTimerLoop(&InitNodes);

    return 0;
}

