
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : pgeez1_conf.h
  * @author  		: SRA Application Team
  * @brief   		: This file contains definitions for the PGEEZ1 components bus interfaces
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PGEEZ1_CONF_H__
#define __PGEEZ1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Global configuration ---------------------------------------------------*/

/* Defines ---------------------------------------------------------------*/

#define SERIAL_COM_INIT						BSP_QUADSPI_Init
#define SERIAL_COM_DEINIT					BSP_QUADSPI_DeInit
#define SERIAL_COM_RECV						BSP_QUADSPI_Recv
#define SERIAL_COM_SEND						BSP_QUADSPI_Send
#define SERIAL_COM_SENDRECV					BSP_QUADSPI_SendRecv
#define PGEEZ1_M95_Delay					HAL_Delay

#define USE_QUADSPI
#define QSPI_INSTANCE						hqspi

#ifdef __cplusplus
}
#endif

#endif /* __PGEEZ1_CONF_H__*/

