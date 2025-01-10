/**
  ******************************************************************************
  * @file    m24m01e.h
  * @author  SRA-SAIL, STM32ODE
  * @brief   header file of m24m01e-f based eeprom driver 
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
#ifndef __M24M01E_H
#define __M24M01E_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
/** @addtogroup M24M01E
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup M24M01E_Exported_Types
  * @{
  */



/* Exported constants --------------------------------------------------------*/
/** @defgroup M24M01E_Exported_Constants
  * @{
  */
#ifndef NULL
#define NULL      (void *) 0
#endif
#define MIN_TRIALS         2
    
#define M24M01E_DEVICE_ADDR_READ_BIT            0x01U
#define M24M01E_DEVICE_ADDR_WRITE_BIT           0xFEU
#define M24M01E_A16_MEMORY_ADDRESS_MASK         0x0001FFFFU
#define M24M01E_A15_A0_MEMORY_ADDRESS_MASK      0x0FFFFU
#define M24M01E_ID_PAGE_ADDRESS_MASK            0x00FFU
#define M24M01E_LOCK_ID_PAGE_ADDRESS            0x6000U
#define M24M01E_LOCK_ID_PAGE_DATA_BYTE          0x02U


/** @defgroup M24XX error codes
  * @}
  */

#define M24_OK              0
#define M24_ERROR   	   -1
#define M24_BUSY    	   -2
#define M24_TIMEOUT 	   -3
#define M24_ADDR_OVERFLOW  -4

/* External variables --------------------------------------------------------*/

  extern uint16_t DeviceAddr;
  extern uint16_t PageSize;

 /*!
 * @brief Initializes the I2C  object 
 * @param  none
 * @retval Error code: 0 ok, negative KO
 */
typedef int32_t     (*M24M01E_Init_Func)(void);   
typedef int32_t     (*M24M01E_DeInit_Func)(void); 
typedef int32_t     (*M24M01E_WriteReg_Func)( uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24M01E_WriteReg16_Func)( uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24M01E_Transmit_Func)( uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24M01E_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24M01E_ReadReg16_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24M01E_IsReady_Func) (uint16_t, const uint32_t);
typedef void        (*M24M01E_Delay)(uint32_t );   

typedef struct
{
  M24M01E_Init_Func            Init;
  M24M01E_DeInit_Func          DeInit;
  M24M01E_WriteReg_Func        WriteReg;
  M24M01E_WriteReg16_Func      WriteReg16;
  M24M01E_Transmit_Func        Transmit;
  M24M01E_ReadReg_Func	   ReadReg;
  M24M01E_ReadReg16_Func	   ReadReg16;
  M24M01E_IsReady_Func         IsReady;
  M24M01E_Delay                Delay;
  uint8_t                  Address;
} M24M01E_IO_t;

typedef struct
{
  M24M01E_IO_t        IO;
} M24M01E_Object_t;

/**
 * @brief  EEPROMEX COMMON driver structure definition
 */
typedef struct
{
  int32_t       (*Init)( M24M01E_Object_t * );
  int32_t       (*DeInit)( M24M01E_Object_t * );
  int32_t       (*IsReady)(M24M01E_Object_t *, const uint32_t );
  int32_t       (*ReadCDARegister)( M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*WriteCDARegister)(M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*ReadDTIRegister)(M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*ReadSWPRegister)( M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t);
  int32_t       (*WriteSWPRegister)(M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*ReadByte)( M24M01E_Object_t *, uint8_t *, const uint32_t);
  int32_t       (*WriteByte)(M24M01E_Object_t *, uint8_t * , const uint32_t);
  int32_t       (*ReadPage)(M24M01E_Object_t *, uint8_t * , const uint32_t , const uint16_t );
  int32_t       (*WritePage)(M24M01E_Object_t *, uint8_t * , const uint32_t , const uint16_t ,const uint16_t ); 
  int32_t       (*ReadData)( M24M01E_Object_t *, uint8_t * , const uint32_t, const uint16_t);
  int32_t       (*WriteData)(M24M01E_Object_t *, uint8_t *, const uint32_t, const uint16_t, const uint16_t );
  int32_t       (*WriteIDPage)(M24M01E_Object_t *,  uint8_t *, const uint16_t, const uint16_t);
  int32_t       (*ReadIDPage)(M24M01E_Object_t *, uint8_t * , const uint16_t, const uint16_t );
  int32_t       (*LockIDPage)(M24M01E_Object_t *);
  void                          *pData;
} M24M01E_EEPROM_CommonDrv_t;

/**
 * @brief  M24M01E driver structure definition
 */
typedef struct
{
  int32_t       (*Init)( M24M01E_Object_t * );
  int32_t       (*DeInit)( M24M01E_Object_t * );
  int32_t       (*IsReady)(M24M01E_Object_t *, const uint32_t );
  int32_t       (*ReadCDARegister)( M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*WriteCDARegister)(M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*ReadDTIRegister)(M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*ReadSWPRegister)( M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*WriteSWPRegister)(M24M01E_Object_t *, uint8_t *, const uint32_t , const uint16_t );
  int32_t       (*ReadByte)( M24M01E_Object_t *, uint8_t *, const uint32_t);
  int32_t       (*WriteByte)(M24M01E_Object_t *, uint8_t * , const uint32_t);
  int32_t       (*ReadPage)(M24M01E_Object_t *, uint8_t * , const uint32_t , const uint16_t );
  int32_t       (*WritePage)(M24M01E_Object_t *, uint8_t * , const uint32_t , const uint16_t ,const uint16_t ); 
  int32_t       (*ReadData)( M24M01E_Object_t *, uint8_t * , const uint32_t, const uint16_t);
  int32_t       (*WriteData)(M24M01E_Object_t *, uint8_t *, const uint32_t, const uint16_t, const uint16_t );
  int32_t       (*WriteIDPage)(M24M01E_Object_t *,  uint8_t *, const uint16_t, const uint16_t);
  int32_t       (*ReadIDPage)(M24M01E_Object_t *, uint8_t * , const uint16_t , const uint16_t );
  int32_t       (*LockIDPage)(M24M01E_Object_t *);
  void                          *pData;
} M24M01E_Drv_t;


/** @addtogroup M24_Imported_Variables
 * @{
 */

extern M24M01E_Drv_t M24M01E_i2c_Drv;

/* Exported macro ------------------------------------------------------------*/
/* Imported functions ------------------------------------------------------- */
/** @addtogroup M24xx_Imported_Functions
 * @{
 */
int32_t M24M01E_RegisterBusIO(M24M01E_Object_t *pObj, M24M01E_IO_t *pIO);


/** @addtogroup M24xx_Private_Functions
 * @}
 */
/* Private function prototypes -----------------------------------------------*/
int32_t M24M01E_i2c_Init( M24M01E_Object_t * pObj );
int32_t M24M01E_i2c_DeInit(M24M01E_Object_t * pObj );
int32_t M24M01E_i2c_IsDeviceReady(M24M01E_Object_t * pObj, const uint32_t Trials );
int32_t M24M01E_i2c_ReadCDARegister( M24M01E_Object_t * pObj, uint8_t * const, const uint32_t TarAddr,  const uint16_t NbByte);
int32_t M24M01E_i2c_WriteCDARegister(M24M01E_Object_t * pObj, uint8_t * const, const uint32_t TarAddr,  const uint16_t NbByte );
int32_t M24M01E_i2c_ReadDTIRegister(M24M01E_Object_t * pObj, uint8_t * const, const uint32_t TarAddr,  const uint16_t NbByte);
int32_t M24M01E_i2c_ReadSWPRegister( M24M01E_Object_t * pObj, uint8_t * const, const uint32_t TarAddr,  const uint16_t NbByte );
int32_t M24M01E_i2c_WriteSWPRegister(M24M01E_Object_t * pObj, uint8_t * const, const uint32_t TarAddr,  const uint16_t NbByte );
int32_t M24M01E_i2c_ReadByte( M24M01E_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr);
int32_t M24M01E_i2c_ReadPage(M24M01E_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize );
int32_t M24M01E_i2c_ReadData( M24M01E_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t NbByte);
int32_t M24M01E_i2c_WriteByte(M24M01E_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr);
int32_t M24M01E_i2c_WritePage( M24M01E_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr,const uint16_t PageSize,const uint16_t NByte);
int32_t M24M01E_i2c_WriteData(M24M01E_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize, const uint16_t NbByte );
int32_t M24M01E_i2c_WriteIDPage(M24M01E_Object_t * pObj, uint8_t * pData, const uint16_t TarAddr, const uint16_t NbByte );
int32_t M24M01E_i2c_ReadIDPage(M24M01E_Object_t * pObj, uint8_t * pData, const uint16_t TarAddr, const uint16_t NbByte );
int32_t M24M01E_i2c_LockIDPage( M24M01E_Object_t * pObj ); 
 


/* Exported functions ------------------------------------------------------- */

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
#endif /* __M24M01E_H */

