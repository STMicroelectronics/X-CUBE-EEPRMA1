/**
  ******************************************************************************
  * @file       : eeica1_conf.h
  * @author     : SRA Application Team
  * @brief      : This file contains definitions for the EEICA1 components bus interfaces
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
#ifndef __EEICA1_CONF_H__
#define __EEICA1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_nucleo_bus.h"
#include "stm32g4xx_nucleo_errno.h"

/* Global configuration ---------------------------------------------------*/
#define M24256_I2C_ADDR    0xAAU
#define M24M01_I2C_ADDR    0xACU

/******************************************************************************/

/* Defines ---------------------------------------------------------------*/
#define EEICA1_I2C_DEINIT   BSP_I2C1_DeInit
#define EEICA1_I2C_INIT     BSP_I2C1_Init
#define EEICA1_I2C_DEINIT   BSP_I2C1_DeInit
#define EEICA1_I2C_READREG    BSP_I2C1_ReadReg
#define EEICA1_I2C_WRITEREG   BSP_I2C1_WriteReg
#define EEICA1_I2C_READREG16  BSP_I2C1_ReadReg16
#define EEICA1_I2C_WRITEREG16 BSP_I2C1_WriteReg16
#define EEICA1_I2C_ISREADY    BSP_I2C1_IsReady
#define EEICA1_I2C_SEND     BSP_I2C1_Send
#define EEICA1_M24_DELAY    HAL_Delay

#define EEICA1_M24256_WC1_PORT             GPIOA
#define EEICA1_M24256_WC1_PIN              GPIO_PIN_1
#define EEICA1_M24256_WC_RCC              __HAL_RCC_GPIOA_CLK_ENABLE

#define EEICA1_M24M01_WC2_PORT             GPIOB
#define EEICA1_M24M01_WC2_PIN              GPIO_PIN_0
#define EEICA1_M24M01_WC2_RCC              __HAL_RCC_GPIOB_CLK_ENABLE

#define EEICA1_NUCLEO_LED_PORT             GPIOA
#define EEICA1_NUCLEO_LED_PIN              GPIO_PIN_5

#ifdef __cplusplus
}
#endif

#endif /* __EEICA1_CONF_H__*/

