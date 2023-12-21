/**
  ******************************************************************************
  * @file    app_eepromrw.h
  * @author  SRA Application Team
  * @brief   This file provides code for the configuration of the
  *      EEICA1 instances.
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
#ifndef __APP_EEPROMRW_H
#define __APP_EEPROMRW_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported Functions --------------------------------------------------------*/
void MX_EEPROM_Init(void);
void MX_EEPROM_Process(void);

#include "stm32f4xx_nucleo.h"
#include "eeica1_m24256e.h"
#include "eeica1_m24m01e.h"
#include "eeica1_conf.h"
#include "string.h"
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/

void MX_EEPROM_RW_Init(void);
void MX_EEPROM_RW_Process(void);

void EEICA1_Setup_CDA_Register(void);
void EEICA1_Read_CDA_Register(void);
void EEICA1_Write_CDA_Register(void);
void EEICA1_Lock_CDA_Register(void);
void EEICA1_Read_DTI_Register(void);
void EEICA1_Test_M24256E_ID_Page(void);
void EEICA1_Test_M24M01E_ID_Page(void);
void EEICA1_Test_M24256E_LockID_Page(void);
void EEICA1_Test_M24M01E_LockID_Page(void);
void EEICA1_Read_SWP_Register(void);
void EEICA1_Write_SWP_Register(void);
void EEICA1_Test_M24256E_Memory(void);
void EEICA1_Test_M24M01E_Memory(void);

void App_Error_Handler(void);

#ifdef __cplusplus
}
#endif
#endif /* __APP_EEPROMRW_H */

