/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : stm32l0xx_nucleo_bus.h
  * @brief          : header file for the BSP BUS IO driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
*/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32L0XX_NUCLEO_BUS_H
#define STM32L0XX_NUCLEO_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_nucleo_conf.h"
#include "stm32l0xx_nucleo_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L0XX_NUCLEO
  * @{
  */

/** @defgroup STM32L0XX_NUCLEO_BUS STM32L0XX_NUCLEO BUS
  * @{
  */

/** @defgroup STM32L0XX_NUCLEO_BUS_Exported_Constants STM32L0XX_NUCLEO BUS Exported Constants
  * @{
  */

#define BUS_SPI1_INSTANCE SPI1
#define BUS_SPI1_SCK_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_SCK_GPIO_PORT GPIOA
#define BUS_SPI1_SCK_GPIO_PIN GPIO_PIN_5
#define BUS_SPI1_SCK_GPIO_AF GPIO_AF0_SPI1
#define BUS_SPI1_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MISO_GPIO_PIN GPIO_PIN_6
#define BUS_SPI1_MISO_GPIO_PORT GPIOA
#define BUS_SPI1_MISO_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MISO_GPIO_AF GPIO_AF0_SPI1
#define BUS_SPI1_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MOSI_GPIO_PORT GPIOA
#define BUS_SPI1_MOSI_GPIO_PIN GPIO_PIN_7
#define BUS_SPI1_MOSI_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_MOSI_GPIO_AF GPIO_AF0_SPI1

#ifndef BUS_SPI1_POLL_TIMEOUT
  #define BUS_SPI1_POLL_TIMEOUT                   0x1000U
#endif
/* SPI1 Baud rate in bps  */
#ifndef BUS_SPI1_BAUDRATE
   #define BUS_SPI1_BAUDRATE   10000000U /* baud rate of SPIn = 10 Mbps*/
#endif

#define BUS_I2C1_INSTANCE I2C1
#define BUS_I2C1_SCL_GPIO_PORT GPIOB
#define BUS_I2C1_SCL_GPIO_AF GPIO_AF4_I2C1
#define BUS_I2C1_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C1_SCL_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_I2C1_SCL_GPIO_PIN GPIO_PIN_8
#define BUS_I2C1_SDA_GPIO_PIN GPIO_PIN_9
#define BUS_I2C1_SDA_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_I2C1_SDA_GPIO_PORT GPIOB
#define BUS_I2C1_SDA_GPIO_AF GPIO_AF4_I2C1
#define BUS_I2C1_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#ifndef BUS_I2C1_POLL_TIMEOUT
   #define BUS_I2C1_POLL_TIMEOUT                0x1000U
#endif
/* I2C1 Frequency in Hz  */
#ifndef BUS_I2C1_FREQUENCY
   #define BUS_I2C1_FREQUENCY  1000000U /* Frequency of I2Cn = 100 KHz*/
#endif

/**
  * @}
  */

/** @defgroup STM32L0XX_NUCLEO_BUS_Private_Types STM32L0XX_NUCLEO BUS Private types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1U)
typedef struct
{
  pI2C_CallbackTypeDef  pMspInitCb;
  pI2C_CallbackTypeDef  pMspDeInitCb;
}BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1U) */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
typedef struct
{
  pSPI_CallbackTypeDef  pMspInitCb;
  pSPI_CallbackTypeDef  pMspDeInitCb;
}BSP_SPI_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1U) */
/**
  * @}
  */

/** @defgroup STM32L0XX_NUCLEO_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Constants
  * @{
  */

extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c1;

/**
  * @}
  */

/** @addtogroup STM32L0XX_NUCLEO_BUS_Exported_Functions
  * @{
  */

/* BUS IO driver over SPI Peripheral */
HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef* hspi);
int32_t BSP_SPI1_Init(void);
int32_t BSP_SPI1_DeInit(void);
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length);
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
int32_t BSP_SPI1_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI1_RegisterMspCallbacks (BSP_SPI_Cb_t *Callbacks);
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1U) */

/* BUS IO driver over I2C Peripheral */
HAL_StatusTypeDef MX_I2C1_Init(I2C_HandleTypeDef* hi2c);
int32_t BSP_I2C1_Init(void);
int32_t BSP_I2C1_DeInit(void);
int32_t BSP_I2C1_IsReady(uint16_t DevAddr, uint32_t Trials);
int32_t BSP_I2C1_WriteReg(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_WriteReg16(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg16(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_SendRecv(uint16_t DevAddr, uint8_t *pTxdata, uint8_t *pRxdata, uint16_t Length);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1U)
int32_t BSP_I2C1_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C1_RegisterMspCallbacks (BSP_I2C_Cb_t *Callbacks);
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1U) */

int32_t BSP_GetTick(void);

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

#endif /* STM32L0XX_NUCLEO_BUS_H */

