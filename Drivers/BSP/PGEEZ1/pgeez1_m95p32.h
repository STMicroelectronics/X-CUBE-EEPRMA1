/**
******************************************************************************
* @file    pgeez1_m95p32.h
* @author  SRA-SAIL
* @brief   header of pgeez1 expansion board based on m95p32 series eeprom
******************************************************************************
* @attention
*
* Copyright (c) 2022 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PGEEZ1_M95P32_H
#define __PGEEZ1_M95P32_H

#ifdef __cplusplus
 extern "C" {
#endif

   
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "m95p32.h"
#include "pgeez1_conf.h"
   
/* Exported macro ------------------------------------------------------------*/
#define PGEEZ1_M95P32_0                 1U   

#ifdef  USE_SPI
   void EEPROMEX_CTRL_HIGH(void);
   void EEPROMEX_CTRL_LOW(void);
   void Transmit_Data_polling(void);
#endif

int32_t M95P32_RegisterBusIO(M95_Object_t *pObj, M95_IO_t *pIO);

int32_t PGEEZ1_M95P32_Init(uint32_t Instance);
int32_t PGEEZ1_M95P32_WriteEnable(void);
int32_t PGEEZ1_M95P32_WriteDisable(void);
int32_t PGEEZ1_M95P32_StatusRegRead(uint8_t * pData);
int32_t PGEEZ1_M95P32_StatusConfigRegWrite(uint32_t Instance, uint8_t * pData, uint32_t Size);
int32_t PGEEZ1_M95P32_WritePage(uint32_t Instance, uint8_t *pData, uint32_t TarAddr,  uint32_t Size);
int32_t PGEEZ1_M95P32_ProgramPage(uint32_t Instance, uint8_t * pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_ReadPage(uint32_t Instance, uint8_t *pData, uint32_t TarAddr,  uint32_t Size);
int32_t PGEEZ1_M95P32_FastRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_FastDRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_FastQRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_ErasePage(uint32_t TarAddr);
int32_t PGEEZ1_M95P32_EraseSector(uint32_t TarAddr);
int32_t PGEEZ1_M95P32_EraseBlock(uint32_t TarAddr);
int32_t PGEEZ1_M95P32_EraseChip(void);
int32_t PGEEZ1_M95P32_ReadID(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_FastReadID(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_WriteID(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_DeepPowerDown(void);
int32_t PGEEZ1_M95P32_DeepPowerDownRel(void);
int32_t PGEEZ1_M95P32_VolRegRead(uint8_t *pData);
int32_t PGEEZ1_M95P32_VolRegWrite(uint8_t regVal);
int32_t PGEEZ1_M95P32_PageProgBuffer(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_ConfSafetyRegRead(uint32_t Instance, uint8_t *pData, uint8_t Size);
int32_t PGEEZ1_M95P32_ClearSafetyFlag(void);
int32_t PGEEZ1_M95P32_SFDPRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size);
int32_t PGEEZ1_M95P32_JEDECRead(uint32_t Instance, uint8_t *pData, uint32_t Size);
int32_t PGEEZ1_M95P32_EnableReset(void);
int32_t PGEEZ1_M95P32_SoftReset(void);

#ifdef USE_QUADSPI
int32_t BSP_QUADSPI_Init  (void);
int32_t BSP_QUADSPI_DeInit(void);

extern QSPI_HandleTypeDef QSPI_INSTANCE;
#endif

/**
 * @}
 */

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
#endif /* __PGEEZ1_M95P32_H */

