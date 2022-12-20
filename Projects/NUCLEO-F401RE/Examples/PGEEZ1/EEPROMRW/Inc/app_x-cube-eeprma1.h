
/**
  ******************************************************************************
  * @file           : app_x-cube-eeprma1.h
  * @brief          :  This file provides code for the configuration
  *						of the PGEEZ1 instances.
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
#ifndef __APP_X_CUBE_EEPRMA1_H
#define __APP_X_CUBE_EEPRMA1_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
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
#ifdef USE_QUADSPI
int32_t M95P32_TestBufferMode(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /* __APP_X_CUBE_EEPRMA1_H */

