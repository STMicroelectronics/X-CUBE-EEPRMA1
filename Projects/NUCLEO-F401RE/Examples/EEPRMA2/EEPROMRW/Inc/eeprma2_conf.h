
/**
  ******************************************************************************
  * @file           : eeprma2_conf.h
  * @author  		: SRA Application Team
  * @brief   		: This file contains definitions for the EEPRMA2 components bus interfaces
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
#ifndef __EEPRMA2_CONF_H__
#define __EEPRMA2_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_bus.h"
#include "stm32f4xx_nucleo_errno.h"

/* Global configuration ---------------------------------------------------*/
#define M24256_I2C_ADDR    0xAAU
#define M24M01_I2C_ADDR    0xACU
#define M24C02_I2C_ADDR    0xA8U
#define M95M04_SPI_ADDR    0xCCU
#define M95256_SPI_ADDR    0xC9U
#define M95040_SPI_ADDR    0xC6U

/******************************************************************************/

/* Defines ---------------------------------------------------------------*/

#define EEPRMA2_I2C_Init         		BSP_I2C1_Init
#define EEPRMA2_I2C_DeInit       		BSP_I2C1_DeInit
#define EEPRMA2_I2C_ReadReg         	BSP_I2C1_ReadReg
#define EEPRMA2_I2C_WriteReg         	BSP_I2C1_WriteReg
#define EEPRMA2_I2C_ReadReg16     		BSP_I2C1_ReadReg16
#define EEPRMA2_I2C_WriteReg16			BSP_I2C1_WriteReg16
#define EEPRMA2_I2C_IsReady             BSP_I2C1_IsReady
#define EEPRMA2_I2C_Send				BSP_I2C1_Send
#define EEPRMA2_M24_Delay          	    HAL_Delay

#define EEPRMA2_SPI_Init         BSP_SPI1_Init
#define EEPRMA2_SPI_DeInit       BSP_SPI1_DeInit
#define EEPRMA2_SPI_Recv         BSP_SPI1_Recv
#define EEPRMA2_SPI_Send         BSP_SPI1_Send
#define EEPRMA2_SPI_SendRecv     BSP_SPI1_SendRecv
#define EEPRMA2_M95_Delay            HAL_Delay

#define M95M04_EEPROM_SPI_CS_PORT             GPIOB
#define M95M04_EEPROM_SPI_CS_PIN              GPIO_PIN_6
#define M95M04_EEPROM_SPI_CS_RCC              __HAL_RCC_GPIOB_CLK_ENABLE

#define M95256_EEPROM_SPI_CS_PORT             GPIOC
#define M95256_EEPROM_SPI_CS_PIN              GPIO_PIN_7
#define M95256_EEPROM_SPI_CS_RCC              __HAL_RCC_GPIOC_CLK_ENABLE

#define M95040_EEPROM_SPI_CS_PORT             GPIOA
#define M95040_EEPROM_SPI_CS_PIN              GPIO_PIN_10
#define M95040_EEPROM_SPI_CS_RCC              __HAL_RCC_GPIOA_CLK_ENABLE

#define EEPRMA2_M95_WP_PORT             GPIOA
#define EEPRMA2_M95_WP_PIN              GPIO_PIN_0
#define EEPRMA2_M95_WP_RCC              __HAL_RCC_GPIOA_CLK_ENABLE

#define EEPRMA2_M95_HOLD_PORT             GPIOB
#define EEPRMA2_M95_HOLD_PIN              GPIO_PIN_0
#define EEPRMA2_M95_HOLD_RCC              __HAL_RCC_GPIOB_CLK_ENABLE

#define EEPRMA2_M24_WC_PORT             GPIOA
#define EEPRMA2_M24_WC_PIN              GPIO_PIN_1
#define EEPRMA2_M24_WC_RCC              __HAL_RCC_GPIOA_CLK_ENABLE

#ifdef __cplusplus
}
#endif

#endif /* __EEPRMA2_CONF_H__*/

