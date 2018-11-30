/**
  ******************************************************************************
  * @file    stm324x9i_eval_fmc_sram.h
  * @author  MCD Application Team
  * @version V1.0.4
  * @date    22-September-2016
  * @brief   This file contains all the functions prototypes for the 
  *          stm324x9i_eval_fmc_sram.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM324X9I_EVAL_SRAM_H
#define __STM324X9I_EVAL_SRAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval.h"

/** @defgroup STM324x9I_EVAL_FMC_SRAM_Private_Defines
  * @{
  */

/**
  * @brief  FMC SRAM bank address
  */   
#define SRAM_BANK_ADDR  ((uint32_t)0x64000000)  
  
/**
  * @brief  FMC SRAM Memory Width
  */  
/* #define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MemoryDataWidth_8b  */
#define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MemoryDataWidth_16b

/**
  * @brief  FMC SRAM Burst Access mode
  */  
#define SRAM_BURSTACCESS    FMC_BurstAccessMode_Disable  
/* #define SRAM_BURSTACCESS    FMC_BurstAccessMode_Enable*/

/**
  * @brief  FMC SRAM Write Burst feature
  */  
#define SRAM_WRITEBURST    FMC_WriteBurst_Disable  
/* #define SRAM_WRITEBURST   FMC_WriteBurst_Enable */

/**
  * @brief  FMC SRAM Continuous Clock feature
  */  
#define CONTINUOUSCLOCK_FEATURE    FMC_CClock_SyncOnly 
/* #define CONTINUOUSCLOCK_FEATURE     FMC_CClock_SyncAsync */

/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_FMC_SDRAM_Exported_Functions
  * @{
  */ 
void  SRAM_Init(void);
void  SRAM_GPIOConfig(void);
void  SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
void  SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);

#ifdef __cplusplus
}
#endif

#endif /* __STM324X9I_EVAL_SRAM_H */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
