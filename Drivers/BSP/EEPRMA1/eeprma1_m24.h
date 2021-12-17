/**
  ******************************************************************************
  * @file    eeprma1_m24.h
  * @author  SRA-SAIL, STM32ODE
  * @brief   header file for X-NUCLEO-EEPRMA1 Expansion driver 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPRMA1_M24_H
#define __EEPRMA1_M24_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "m24xx.h" 
#include "eeprma1_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup X_NUCLEO_EEPRMA1
  * @{
  */

/* Exported types ------------------------------------------------------------*/


/**
 * @brief  EEPROM enumerator definition
 */
  
/**

  * @}
  */
/* Exported constants --------------------------------------------------------*/
/** @defgroup EEPRMA1_M24_Exported_Constants   EEPRMA M24 Exported Constants
  * @{
  */

#define EEPRMA1_M24_INSTANCES_NBR		3U 
#define EEPRMA1_M24C02_0                         0U
#define EEPRMA1_M24256_0                         1U
#define EEPRMA1_M24M01_0                         2U  
#define EEPRMA1_M24xx                            7U


/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup EEPRMA1_M24_Exported_Functions   EEPRMA M24 Exported Functions
  * @{
  */

int32_t EEPRMA1_M24_Init(uint32_t Instance);
int32_t EEPRMA1_M24_DeInit(uint32_t Instance);
int32_t EEPRMA1_M24_IsDeviceReady( uint32_t Instance, const uint32_t Trials );
int32_t EEPRMA1_M24_ReadByte (uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr);
int32_t EEPRMA1_M24_ReadData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA1_M24_ReadPage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA1_M24_WriteByte (uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr);
int32_t EEPRMA1_M24_WriteData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA1_M24_WritePage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA1_M24_WriteID(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size );
int32_t EEPRMA1_M24_ReadID(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size );
int32_t EEPRMA1_M24_LockStatus( uint32_t Instance, uint8_t * pData);
int32_t EEPRMA1_M24_LockID( uint32_t Instance);
void EEPRMA1_M24MemorySizeLocator(uint32_t Instance);
/**
  * @}
  */
//void EEPROMEX_CTRL_HIGH(const uint8_t pin);
//void EEPROMEX_CTRL_LOW(const uint8_t pin);
/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
  }
#endif
#endif /* __X_NUCLEO_EEPRMA_H */

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
