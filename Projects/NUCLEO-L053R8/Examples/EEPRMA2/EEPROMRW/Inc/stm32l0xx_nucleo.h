/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file  : stm32l0xx_nucleo.h
  * @brief : header file for the BSP Common driver
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
#ifndef __STM32L0XX_NUCLEO_H
#define __STM32L0XX_NUCLEO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_nucleo_conf.h"
#include "stm32l0xx_nucleo_errno.h"
#include "main.h"

#if (USE_BSP_COM_FEATURE > 0)
  #if (USE_COM_LOG > 0)
    #if defined(__ICCARM__) || defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* For IAR and ARM Compiler 5 and 6*/
      #include <stdio.h>
    #endif
  #endif
#endif
/** @addtogroup BSP
 * @{
 */

/** @defgroup STM32L0XX_NUCLEO
 * @{
 */

/** @defgroup STM32L0XX_NUCLEO_LOW_LEVEL
 * @{
 */

/** @defgroup STM32L4XX_NUCLEO_LOW_LEVEL_Exported_Constants LOW LEVEL Exported Constants
  * @{
  */
/**
 * @brief STM32L0XX NUCLEO BSP Driver version number V1.0.0
 */
#define __STM32L0XX_NUCLEO_BSP_VERSION_MAIN   (uint32_t)(0x01) /*!< [31:24] main version */
#define __STM32L0XX_NUCLEO_BSP_VERSION_SUB1   (uint32_t)(0x00) /*!< [23:16] sub1 version */
#define __STM32L0XX_NUCLEO_BSP_VERSION_SUB2   (uint32_t)(0x00) /*!< [15:8]  sub2 version */
#define __STM32L0XX_NUCLEO_BSP_VERSION_RC     (uint32_t)(0x00) /*!< [7:0]  release candidate */
#define __STM32L0XX_NUCLEO_BSP_VERSION        ((__STM32L0XX_NUCLEO_BSP_VERSION_MAIN << 24)\
                                                    |(__STM32L0XX_NUCLEO_BSP_VERSION_SUB1 << 16)\
                                                    |(__STM32L0XX_NUCLEO_BSP_VERSION_SUB2 << 8 )\
                                                    |(__STM32L0XX_NUCLEO_BSP_VERSION_RC))

/** @defgroup STM32L0XX_NUCLEO_LOW_LEVEL_Exported_Types STM32L0XX_NUCLEO LOW LEVEL Exported Types
 * @{
 */

 /**
  * @brief Define for STM32L0XX_NUCLEO board
  */
#if !defined (USE_STM32L0XX_NUCLEO)
 #define USE_STM32L0XX_NUCLEO
#endif
#ifndef USE_BSP_COM_FEATURE
   #define USE_BSP_COM_FEATURE                  0U
#endif

/**
 * @}
 */

/** @defgroup STM32L0XX_NUCLEO_LOW_LEVEL_COM STM32L0XX_NUCLEO LOW LEVEL COM
 * @{
 */
/**
 * @brief Definition for COM portx, connected to USART2
 */

#define BUS_USART2_INSTANCE USART2
#define BUS_USART2_TX_GPIO_AF GPIO_AF4_USART2
#define BUS_USART2_TX_GPIO_PIN GPIO_PIN_2
#define BUS_USART2_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_USART2_TX_GPIO_PORT GPIOA
#define BUS_USART2_TX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_USART2_RX_GPIO_PORT GPIOA
#define BUS_USART2_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_USART2_RX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_USART2_RX_GPIO_PIN GPIO_PIN_3
#define BUS_USART2_RX_GPIO_AF GPIO_AF4_USART2

/**
 * @}
 */

/** @defgroup STM32L0XX_NUCLEO_LOW_LEVEL_Exported_Types LOW LEVEL Exported Types
  * @{
  */
#ifndef USE_BSP_COM
  #define USE_BSP_COM                           0U
#endif

#ifndef USE_COM_LOG
  #define USE_COM_LOG                           1U
#endif

#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  COM1 = 0U,
  COMn
}COM_TypeDef;

typedef enum
{
 COM_WORDLENGTH_8B     =   UART_WORDLENGTH_8B,
 COM_WORDLENGTH_9B     =   UART_WORDLENGTH_9B,
}COM_WordLengthTypeDef;

typedef enum
{
 COM_STOPBITS_1     =   UART_STOPBITS_1,
 COM_STOPBITS_2     =   UART_STOPBITS_2,
}COM_StopBitsTypeDef;

typedef enum
{
 COM_PARITY_NONE     =  UART_PARITY_NONE,
 COM_PARITY_EVEN     =  UART_PARITY_EVEN,
 COM_PARITY_ODD      =  UART_PARITY_ODD,
}COM_ParityTypeDef;

typedef enum
{
 COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
 COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
 COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
 COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
}COM_HwFlowCtlTypeDef;

typedef struct
{
  uint32_t             BaudRate;
  COM_WordLengthTypeDef  WordLength;
  COM_StopBitsTypeDef  StopBits;
  COM_ParityTypeDef    Parity;
  COM_HwFlowCtlTypeDef HwFlowCtl;
}COM_InitTypeDef;
#endif

#define MX_UART_InitTypeDef          COM_InitTypeDef
#define MX_UART_StopBitsTypeDef      COM_StopBitsTypeDef
#define MX_UART_ParityTypeDef        COM_ParityTypeDef
#define MX_UART_HwFlowCtlTypeDef     COM_HwFlowCtlTypeDef
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1U)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
} BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1U) */

/**
 * @}
 */

#define COMn                             1U
#define COM1_UART                        USART2

#define COM_POLL_TIMEOUT                 1000
extern UART_HandleTypeDef hcom_uart[COMn];
#define  huart2 hcom_uart[COM1]

/**
 * @}
 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32L0XX_NUCLEO_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32L0XX_NUCLEO_LOW_LEVEL_Exported_Functions STM32L0XX_NUCLEO LOW LEVEL Exported Functions
 * @{
 */

int32_t  BSP_GetVersion(void);
#if (USE_BSP_COM_FEATURE > 0)
int32_t  BSP_COM_Init(COM_TypeDef COM);
int32_t  BSP_COM_DeInit(COM_TypeDef COM);
#endif

#if (USE_COM_LOG > 0)
int32_t  BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1U)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM , BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

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

#endif /* __STM32L0XX_NUCLEO__H */

