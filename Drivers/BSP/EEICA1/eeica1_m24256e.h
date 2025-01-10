/**
 ******************************************************************************
  * @file    eeica1_m24256e.h
  * @author  SRA-SAIL, STM32ODE
  * @brief   header of eeprma expansion board based on m24256e-f series eeprom
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEICA1_M24256E_H
#define __EEICA1_M24256E_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "m24256e.h"
#include "eeica1_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup X_NUCLEO_EEICA1
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
/** @defgroup EEICA1_M24_Exported_Constants   EEICA1 M24 Exported Constants
  * @{
  */

#define M24256E_PAGESIZE        64U
#define M24256E_MEMORYSIZE      32768U /* in bytes, do not change */
   
#define EEICA1_M24_INSTANCES_NBR	       2U 
#define EEICA1_M24256E                       0U
#define EEICA1_M24xx                           1U
    
    
#define M24256E_CDA_ADDRESS                    0xC000



/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup EEICA1_M24_Exported_Functions   EEICA1 M24 Exported Functions
  * @{
  */

int32_t EEICA1_M24256E_Init(uint32_t Instance);
int32_t EEICA1_M24256E_DeInit(uint32_t Instance);
int32_t EEICA1_M24256E_IsDeviceReady( uint32_t Instance, const uint32_t Trials );
int32_t EEICA1_M24256E_ReadCDARegister( uint32_t Instance, uint8_t * const pData );
int32_t EEICA1_M24256E_WriteCDARegister( uint32_t Instance, uint8_t * const pData );
int32_t EEICA1_M24256E_LockCDARegister( uint32_t Instance, uint8_t * const pData );
int32_t EEICA1_M24256E_ReadByte (uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr);
int32_t EEICA1_M24256E_ReadData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEICA1_M24256E_ReadPage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEICA1_M24256E_WriteByte (uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr);
int32_t EEICA1_M24256E_WriteData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEICA1_M24256E_WritePage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEICA1_M24256E_WriteIDPage(uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t EEICA1_M24256E_ReadIDPage(uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t EEICA1_M24256E_LockIDPage( uint32_t Instance);
void EEPROMEX_M24256_WC_HIGH(void);
void EEPROMEX_M24256_WC_LOW(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
  }
#endif
#endif /* __EEICA1_M24256E_H */

