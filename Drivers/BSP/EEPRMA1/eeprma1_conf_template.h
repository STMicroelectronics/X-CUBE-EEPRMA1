/**
 ******************************************************************************
 * @file    eeprma1_conf.h
 * @author  SRA-SAIL STM32ODE Team
 * @brief   This file contains definitions for the EEPROM components bus interfaces
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

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_bus.h"
#include "stm32f4xx_nucleo_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPRMA1_CONF_H__
#define __EEPRMA1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define M24256_I2C_ADDR    0xAAU
#define M24M01_I2C_ADDR    0xACU
#define M24C02_I2C_ADDR    0xA8U 
#define M95M01_SPI_ADDR    0xCCU
#define M95256_SPI_ADDR    0xC9U
#define M95040_SPI_ADDR    0xC6U
  
#define EEPRMA1_I2C_Init           BSP_I2C1_Init
#define EEPRMA1_I2C_DeInit         BSP_I2C1_DeInit
#define EEPRMA1_I2C_ReadReg        BSP_I2C1_ReadReg
#define EEPRMA1_I2C_WriteReg       BSP_I2C1_WriteReg
#define EEPRMA1_I2C_ReadReg16      BSP_I2C1_ReadReg16
#define EEPRMA1_I2C_WriteReg16     BSP_I2C1_WriteReg16
#define EEPRMA1_I2C_IsReady        BSP_I2C1_IsReady
#define EEPRMA1_I2C_Send           BSP_I2C1_Send
#define EEPRMA1_M24_Delay          HAL_Delay

#define EEPRMA1_SPI_Init         BSP_SPI1_Init
#define EEPRMA1_SPI_DeInit       BSP_SPI1_DeInit
//#define EEPRMA1_SPI_ReadReg      BSP_SPI1_Recv
//#define EEPRMA1_SPI_WriteReg     BSP_SPI1_Send  
#define EEPRMA1_SPI_Recv         BSP_SPI1_Recv
#define EEPRMA1_SPI_Send         BSP_SPI1_Send
#define EEPRMA1_SPI_SendRecv     BSP_SPI1_SendRecv
#define EEPRMA1_M95_Delay        HAL_Delay


#ifdef __cplusplus
}
#endif

#endif /* __EEPRMA1_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
