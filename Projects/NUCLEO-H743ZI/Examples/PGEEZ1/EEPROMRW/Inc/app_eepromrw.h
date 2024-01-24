/**
  ******************************************************************************
  * @file    app_eepromrw.h
  * @author  SRA Application Team
  * @brief   This file provides code for the configuration of the
  *      PGEEZ1 instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_EEPROMRW_H
#define __APP_EEPROMRW_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "pgeez1_conf.h"

/* Exported Functions --------------------------------------------------------*/
void MX_EEPROM_Init(void);
void MX_EEPROM_Process(void);

void MX_EEPROM_RW_Init(void);
void MX_EEPROM_RW_Process(void);

int32_t M95P32_TestPage(void);
int32_t M95P32_TestPageErase(void);
int32_t M95P32_TestSectorErase(void);
int32_t M95P32_TestBlockErase(void);
int32_t M95P32_TestWriteEnableDisable(void);
void M95P32_PageProg_with_SafetyRegisterCheck(void);
void M95P32_PageWrite_with_SafetyRegisterCheck(void);
void M95P32_16Word_align(void);
void M95P32_WriteProtectedArea(void);
void M95P32_PageProgram_with_BufferLoad(void);
void PageProgram_with_BufferLoad(uint32_t Instance, uint8_t *pData, uint32_t start_prog_add, uint32_t nb_byte);
int32_t M95P32_TestBufferMode(void);

#ifdef __cplusplus
}
#endif
#endif /* __APP_EEPROMRW_H */

