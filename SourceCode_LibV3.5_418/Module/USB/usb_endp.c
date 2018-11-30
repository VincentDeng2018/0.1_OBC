/**
  ******************************************************************************
  * @file    usb_endp.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Endpoint routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "hw_config.h"

#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_pwr.h"

#include "stm3210x_eval.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Receive_Buffer[64];
uint8_t Receive_Buffer_Bak[64];
__IO uint8_t PrevXferComplete = 1;

extern uint8_t bRxdProc;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
	uint32_t len;
	uint32_t i;
	uint32_t *pDestData = NULL;
	uint32_t *pSrcData = NULL;
	
	/* Read received data (2 bytes) */  
	len = USB_SIL_Read(EP1_OUT, Receive_Buffer);

	if (64 == len)
	{
		//if ((PrevXferComplete) && (CONFIGURED ==  bDeviceState))
		//{
			#if 0
			/* Write the descriptor through the endpoint */
			USB_SIL_Write(EP1_IN,(uint8_t *)Receive_Buffer,64);
			SetEPTxValid(ENDP1);
			PrevXferComplete = 0;
			#endif
			if (!bRxdProc)
			{
				pSrcData = (uint32_t *)Receive_Buffer;
				pDestData = (uint32_t *)Receive_Buffer_Bak;
				for (i = 0; i < 16; i++)
				{
					*pDestData = *pSrcData;
					pDestData++;
					pSrcData++;
				}
				bRxdProc = TRUE;
			}
		//}
	}

	SetEPRxStatus(ENDP1, EP_RX_VALID);
 
}

/*******************************************************************************
* Function Name  : EP1_IN_Callback.
* Description    : EP1 IN Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
  PrevXferComplete = 1;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

