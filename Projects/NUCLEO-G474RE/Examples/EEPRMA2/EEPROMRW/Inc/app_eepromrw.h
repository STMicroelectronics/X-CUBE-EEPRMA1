/**
  ******************************************************************************
  * @file    app_eepromrw.h
  * @author  SRA Application Team
  * @brief   This file provides code for the configuration of the
  *      EEPRMA2 instances.
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

/* Exported Functions --------------------------------------------------------*/
void MX_EEPROM_Init(void);
void MX_EEPROM_Process(void);

#ifdef __cplusplus
}
#endif
#endif /* __APP_EEPROMRW_H */

