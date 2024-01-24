/**
  ******************************************************************************
  * @file           : pgeez1_conf.h
  * @author     : SRA Application Team
  * @brief      : This file contains definitions for the PGEEZ1 components bus interfaces
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
#ifndef __PGEEZ1_CONF_H__
#define __PGEEZ1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l0xx_nucleo_bus.h"
#include "stm32l0xx_nucleo_errno.h"

/* Global configuration ---------------------------------------------------*/

/* Defines ---------------------------------------------------------------*/
#define SERIAL_COM_INIT           BSP_SPI1_Init
#define SERIAL_COM_DEINIT         BSP_SPI1_DeInit
#define SERIAL_COM_RECV           BSP_SPI1_Recv
#define SERIAL_COM_SEND           BSP_SPI1_Send
#define SERIAL_COM_SENDRECV         BSP_SPI1_SendRecv
#define PGEEZ1_M95_DELAY          HAL_Delay

#define USE_SPI
#define SPI_INSTANCE            hspi1

#define M95P32_EEPROM_SPI_CS_PORT     GPIOA
#define M95P32_EEPROM_SPI_CS_PIN      GPIO_PIN_9
#define M95P32_EEPROM_SPI_CS_RCC      __HAL_RCC_GPIOA_CLK_ENABLE

#define M95P32_EEPROM_WP_PORT       GPIOB
#define M95P32_EEPROM_WP_PIN        GPIO_PIN_6
#define M95P32_EEPROM_WP_RCC        __HAL_RCC_GPIOB_CLK_ENABLE

#define M95P32_EEPROM_HOLD_PORT       GPIOB
#define M95P32_EEPROM_HOLD_PIN        GPIO_PIN_0
#define M95P32_EEPROM_HOLD_RCC        __HAL_RCC_GPIOB_CLK_ENABLE

#ifdef __cplusplus
}
#endif

#endif /* __PGEEZ1_CONF_H__*/

