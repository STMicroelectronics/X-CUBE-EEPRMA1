/**
******************************************************************************
* @file    m24m01e.c
* @author  SRA-SAIL, STM32ODE
* @brief   file provides functionality of driver functions and to manage communication 
between BSP and M24M01E-F chip
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

/* Includes ------------------------------------------------------------------*/
#include "m24m01e.h"


/** @addtogroup BSP
* @{
*/

/** @addtogroup Components
* @{
*/ 


/** @defgroup M24M01E
* @brief This file provides set of driver functions to manage communication
*        between BSP and M24M01E chip.
* @{
*/

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup M24M01E_Private_Defines
* @{
*/  
/**
* @}
*/
/* Private macros ------------------------------------------------------------*/


/* Global variables ----------------------------------------------------------*/
extern uint8_t M24M01E_CDA_DevSelCode;
extern uint8_t M24M01E_DTI_DevSelCode;
extern uint8_t M24M01E_IDPage_DevSelCode;
extern uint8_t M24M01E_SWP_DevSelCode;
extern uint8_t M24M01E_Memory_DevSelCode;

/** @defgroup M24_Global_Variables
* @{
*/

M24M01E_Drv_t M24M01E_i2c_Drv =
{
  M24M01E_i2c_Init,
  M24M01E_i2c_DeInit,
  M24M01E_i2c_IsDeviceReady,
  M24M01E_i2c_ReadCDARegister,
  M24M01E_i2c_WriteCDARegister,
  M24M01E_i2c_ReadDTIRegister,
  M24M01E_i2c_ReadSWPRegister,
  M24M01E_i2c_WriteSWPRegister,
  M24M01E_i2c_ReadByte,
  M24M01E_i2c_WriteByte,
  M24M01E_i2c_ReadPage,
  M24M01E_i2c_WritePage,  
  M24M01E_i2c_ReadData,
  M24M01E_i2c_WriteData,
  M24M01E_i2c_WriteIDPage,
  M24M01E_i2c_ReadIDPage,
  M24M01E_i2c_LockIDPage,
  NULL
};
/**
* @}
*/ 

/* Public functions ----------------------------------------------------------*/
/** @defgroup M24M01E_Public_Functions
* @{
*/

/*!
* @brief IO function pointer structure
*/

/* -------- Static functions prototypes ------------------------------------- */
/* Exported functions --------------------------------------------------------*/
/**
* @brief  Function to connect a component device to the IO bus
* @param  pObj : pointer to M24M01E component object
* @param  pIO : pointer to M24M01E IO object
* @retval EEPROMEX status
*/
int32_t M24M01E_RegisterBusIO(M24M01E_Object_t *pObj, M24M01E_IO_t *pIO)
{
  int32_t status = M24_OK;
  pObj->IO.Address	   = pIO->Address;
  pObj->IO.Init            = pIO->Init;
  pObj->IO.DeInit          = pIO->DeInit;
  pObj->IO.ReadReg 	   = pIO->ReadReg;
  pObj->IO.WriteReg        = pIO->WriteReg;
  pObj->IO.ReadReg16 	   = pIO->ReadReg16;
  pObj->IO.WriteReg16      = pIO->WriteReg16;
  pObj->IO.Transmit        = pIO->Transmit;
  pObj->IO.IsReady	   = pIO->IsReady;
  pObj->IO.Delay           = pIO->Delay;
  
  if(!pObj->IO.Init)
  {
    status =  M24_ERROR;
  }
  return status;
}

/**
* @brief  Set M24M01E eeprom Initialization
* @param  pObj : pointer to M24M01E object
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_Init(M24M01E_Object_t *pObj)
{ 
  int32_t status = M24_OK;
  /* Configure the low level interface */
  if (pObj->IO.Init()<0)
  {
    status =  M24_ERROR;
  }
  return status;
}

/**
* @brief  De-Initialization of M24M01E eeprom 
* @param  pObj : pointer to M24M01E object
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_DeInit(M24M01E_Object_t *pObj)
{ 
  int32_t status = M24_OK;
  {
    status = M24_ERROR;
  }
  return status;
}

/**
* @brief  Check M24 availability
* @param  pObj : pointer to M24M01E object
* @param  Trials : number of max tentative tried
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_IsDeviceReady(M24M01E_Object_t *pObj, const uint32_t Trials)
{
  uint8_t Devaddr = M24M01E_CDA_DevSelCode | M24M01E_DEVICE_ADDR_READ_BIT;
  return pObj->IO.IsReady(Devaddr, Trials);
}

/**
* @brief  Read N bytes starting from specified I2C address
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to store read data
* @param  TarAddr : I2C data memory address to read
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_ReadByte(M24M01E_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr)
{
  int32_t status = M24_OK;
  uint8_t Devaddr;
  uint32_t targetAddress;
  
  if(((TarAddr & M24M01E_A16_MEMORY_ADDRESS_MASK) >> 16U) == 0x01U) /* A16 bit is set in address */
  {
    Devaddr = ((M24M01E_Memory_DevSelCode | 0x02U) | M24M01E_DEVICE_ADDR_READ_BIT); /* Set Bit 1 and Bit 0 in Memory Device Select Code*/
    targetAddress = TarAddr & M24M01E_A15_A0_MEMORY_ADDRESS_MASK; /* Capture only A15 to A0 bits*/
  }
  else
  {
    Devaddr = M24M01E_Memory_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT; 
    targetAddress = TarAddr;
  }
  
  if ( pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status =  M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.ReadReg16(Devaddr, targetAddress, pData, 1);
  }
  return status;
}

/**
* @brief  Read a page starting from specified I2C address
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to store read data
* @param  TarAddr : I2C data memory address to read
* @param  PageSize : Size of the page of selected memory
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_ReadPage(M24M01E_Object_t *pObj, 
                             uint8_t * pData, 
                             const uint32_t TarAddr, 
                             const uint16_t PageSize)
{  
  int32_t status = M24_OK;
  uint8_t Devaddr;
  uint32_t targetAddress;
  
  if(((TarAddr & M24M01E_A16_MEMORY_ADDRESS_MASK) >> 16U) == 0x01U) /* A16 bit is set in address */
  {
    Devaddr = ((M24M01E_Memory_DevSelCode | 0x02U) | M24M01E_DEVICE_ADDR_READ_BIT); /* Set Bit 1 and Bit 0 in Memory Device Select Code*/
    targetAddress = TarAddr & M24M01E_A15_A0_MEMORY_ADDRESS_MASK; /* Capture only A15 to A0 bits*/
  }
  else
  {
    Devaddr = M24M01E_Memory_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT; 
    targetAddress = TarAddr;
  }
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status =  M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.ReadReg16(Devaddr, targetAddress, pData, PageSize);
  }
  return status;
}

/**
* @brief  Read N bytes starting from specified I2C address
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to store
* @param  TarAddr : I2C data memory address to read
* @param  NbByte : number of bytes to read
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_ReadData(M24M01E_Object_t *pObj, 
                             uint8_t * pData, 
                             const uint32_t TarAddr, 
                             const uint16_t NbByte)
{
  int32_t status = M24_OK;    
  uint8_t Devaddr;
  uint32_t targetAddress;
  
  if(((TarAddr & M24M01E_A16_MEMORY_ADDRESS_MASK) >> 16U) == 0x01U) /* A16 bit is set in address */
  {
    Devaddr = ((M24M01E_Memory_DevSelCode | 0x02U) | M24M01E_DEVICE_ADDR_READ_BIT); /* Set Bit 1 in Memory Device Select Code*/
    targetAddress = TarAddr & M24M01E_A15_A0_MEMORY_ADDRESS_MASK; /* Capture only A15 to A0 bits*/
  }
  else
  {
    Devaddr = M24M01E_Memory_DevSelCode | M24M01E_DEVICE_ADDR_READ_BIT; 
    targetAddress = TarAddr;
  }
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status =  M24_TIMEOUT;
  }
  else
  {
    status =  pObj->IO.ReadReg16(Devaddr, targetAddress, pData, NbByte);
  }
  return status;
}

/**
* @brief  Write a single byte to a specified address of I2C memory
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_WriteByte(M24M01E_Object_t *pObj, 
                              uint8_t * pData, 
                              const uint32_t TarAddr)
{
  int32_t status = M24_OK;
  uint8_t Devaddr;
  uint32_t targetAddress;
  
  if(((TarAddr & M24M01E_A16_MEMORY_ADDRESS_MASK) >> 16U) == 0x01U) /* A16 bit is set in address */
  {
    Devaddr = ((M24M01E_Memory_DevSelCode | 0x02U) & M24M01E_DEVICE_ADDR_WRITE_BIT); /* Set Bit 1 in Memory Device Select Code*/
    targetAddress = TarAddr & M24M01E_A15_A0_MEMORY_ADDRESS_MASK; /* Capture only A15 to A0 bits*/
  }
  else
  {
    Devaddr = M24M01E_Memory_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT; 
    targetAddress = TarAddr;
  }
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status =  M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.WriteReg16(Devaddr, targetAddress, pData, 1);
    pObj->IO.Delay(5);
  }
    
  return status;
}

/**
* @brief  Write Page
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @param  PageSize : Size of the page of selected memory
* @param  NbByte : number of bytes to write
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_WritePage(M24M01E_Object_t *pObj, 
                              uint8_t * pData, 
                              const uint32_t TarAddr, 
                              const uint16_t PageSize, 
                              const uint16_t NByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr;
  uint32_t targetAddress;
  
  if(((TarAddr & M24M01E_A16_MEMORY_ADDRESS_MASK) >> 16U) == 0x01U) /* A16 bit is set in address */
  {
    Devaddr = ((M24M01E_Memory_DevSelCode | 0x02U) & M24M01E_DEVICE_ADDR_WRITE_BIT); /* Set Bit 1 and Bit 0 in Memory Device Select Code*/
    targetAddress = TarAddr & M24M01E_A15_A0_MEMORY_ADDRESS_MASK; /* Capture only A15 to A0 bits*/
  }
  else
  {
    Devaddr = M24M01E_Memory_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT; 
    targetAddress = TarAddr;
  }
  
  if (pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status = M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.WriteReg16(Devaddr, targetAddress, pData, PageSize);
    pObj->IO.Delay(5);
  }
  
  return status; 
}

/**
* @brief  Write N data bytes starting from specified I2C Address
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @param  PageSize : Size of the page of selected memory
* @param  NbByte : number of bytes to write
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_WriteData(M24M01E_Object_t *pObj, 
                              uint8_t *pData, 
                              const uint32_t TarAddr, 
                              const uint16_t PageSize, 
                              const uint16_t NbByte)
{   
  int32_t status = M24_OK;
  uint32_t remainingSize = NbByte;
  uint32_t bytesToWrite;
  
  uint8_t Devaddr;
  uint32_t targetAddress;
  
  if((TarAddr >> 16U) == 0x01U) /* A16 bit is set in address */
  {
    Devaddr = ((M24M01E_Memory_DevSelCode | 0x02U) & M24M01E_DEVICE_ADDR_WRITE_BIT); /* Set Bit 1 in Memory Device Select Code*/
    targetAddress = TarAddr & M24M01E_A15_A0_MEMORY_ADDRESS_MASK; /* Capture only A15 to A0 bits*/
  }
  else
  {
    Devaddr = M24M01E_Memory_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT; 
    targetAddress = TarAddr;
  }
  
  /* Calculate the starting page and offset */
  uint32_t startOffset = TarAddr % PageSize;
  uint32_t offset = startOffset;
  
  
  /* Check for invalid inputs */
  if((pObj == NULL) || (pData == NULL) || (PageSize == 0U) || (remainingSize == 0U))
  {
    return M24_ERROR;
  }    
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    return M24_TIMEOUT;
  }
  
  /* Iterate over the pages and write the data */
  while(remainingSize > 0U) 
  {
    bytesToWrite = (remainingSize < (PageSize - offset)) ? remainingSize : (PageSize - offset);
    
    status = pObj->IO.WriteReg16(Devaddr, targetAddress, pData, bytesToWrite);
    
    
    pObj->IO.Delay(5);
    if(status == M24_OK)
    {
      
      /* Update the pointers and sizes for the next page */
      pData += bytesToWrite;
      targetAddress += bytesToWrite;
      remainingSize -= bytesToWrite;
      
      if(((targetAddress & M24M01E_A16_MEMORY_ADDRESS_MASK) >> 16U) == 0x01U) /* A16 bit is set in address */
      {
        Devaddr = (M24M01E_Memory_DevSelCode | 0x02U); /* Set Bit 1 in Memory Device Select Code*/
        targetAddress = targetAddress & M24M01E_A15_A0_MEMORY_ADDRESS_MASK; /* Capture only A15 to A0 bits*/        
      }
      offset = targetAddress % PageSize;
      
      /* Wait for the I2C interface to be ready before proceeding to the next page */
      while(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK){}
    }
    else
    {
      status = M24_ERROR;
      break;
    }
  }
  
  return status;
}

/**
* @brief  Write Identification Page
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @param  NbByte : number of bytes to write
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_WriteIDPage(M24M01E_Object_t *pObj, 
                                uint8_t * pData, 
                                const uint16_t TarAddr, 
                                const uint16_t NbByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_IDPage_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT;
  uint16_t temp_TarAddr = TarAddr & M24M01E_ID_PAGE_ADDRESS_MASK;
  
  if (pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status = M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.WriteReg16(Devaddr, temp_TarAddr, pData, NbByte);
    pObj->IO.Delay(5);
  }
  return status;
}

/**
* @brief  Read Identification Page
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to store
* @param  TarAddr : I2C data memory address to write
* @param  NbByte : number of bytes to read
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_ReadIDPage(M24M01E_Object_t *pObj, 
                               uint8_t * pData, 
                               const uint16_t TarAddr, 
                               const uint16_t NbByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_IDPage_DevSelCode | M24M01E_DEVICE_ADDR_READ_BIT;
  uint16_t temp_TarAddr = TarAddr & M24M01E_ID_PAGE_ADDRESS_MASK;
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status =  M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.ReadReg16(Devaddr, temp_TarAddr, pData, NbByte);
  }
  return status;
}

/**
* @brief  Permanentaly lock identification page of memory
* @param  pObj : pointer to M24M01E object
* @retval BSP status
*/
int32_t M24M01E_i2c_LockIDPage(M24M01E_Object_t *pObj)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_IDPage_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT;
  uint16_t temp_TarAddr = M24M01E_LOCK_ID_PAGE_ADDRESS;
  uint8_t data_byte = M24M01E_LOCK_ID_PAGE_DATA_BYTE; /* B1 bit to be set, other don't care*/
  
  if (pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status =  M24_TIMEOUT;
  }
  else
  {
    status =  pObj->IO.WriteReg16(Devaddr, temp_TarAddr, &data_byte, 1);
    pObj->IO.Delay(5);
  }
  return status;
}

/**
* @brief  Read CDA Register
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @param  NbByte : number of bytes to read
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_ReadCDARegister(M24M01E_Object_t *pObj, 
                                    uint8_t * const pData,
                                    const uint32_t TarAddr, 
                                    const uint16_t NbByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_CDA_DevSelCode | M24M01E_DEVICE_ADDR_READ_BIT;
  
  if (pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status = M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.ReadReg16(Devaddr, TarAddr, pData, NbByte);
  }
  return status;
  
}

/**
* @brief  Write CDA Register
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @param  NbByte : number of bytes to write
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_WriteCDARegister(M24M01E_Object_t *pObj, 
                                     uint8_t * const pData,
                                     const uint32_t TarAddr, 
                                     const uint16_t NbByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_CDA_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT;
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status =  M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.WriteReg16( Devaddr, TarAddr, pData, NbByte);
    pObj->IO.Delay(5);
  }
  return status;
  
}

/**
* @brief  Read DTI Register
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @param  NbByte : number of bytes to read
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_ReadDTIRegister(M24M01E_Object_t *pObj, 
                                    uint8_t * const pData,
                                    const uint32_t TarAddr, 
                                    const uint16_t NbByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_DTI_DevSelCode | M24M01E_DEVICE_ADDR_READ_BIT;
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status = M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.ReadReg16( Devaddr, TarAddr, pData, NbByte);
  }
  return status;
  
}

/**
* @brief  Read SWP Register
* @param  pObj : pointer to M24M01E object
* @param  pData : pointer of the data to write
* @param  TarAddr : I2C data memory address to write
* @param  NbByte : number of bytes to read
* @retval EEPROMEX status
*/
int32_t M24M01E_i2c_ReadSWPRegister(M24M01E_Object_t *pObj, 
                                    uint8_t * const pData,
                                    const uint32_t TarAddr, 
                                    const uint16_t NbByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_SWP_DevSelCode | M24M01E_DEVICE_ADDR_READ_BIT;
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status = M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.ReadReg16(Devaddr, TarAddr, pData, NbByte);
  }
  return status;
  
}

/**
* @brief  Write SWP register 
* @param  pData : pointer of the data to store
* @param  TarAddr : I2C data memory address to write
* @param  PageSize : Size of the page of selected memory
* @param  DeviceAddr : Device Address of selected memory
* @param  NbByte : number of bytes to read
* @retval EEPROMEX enum status
*/
int32_t M24M01E_i2c_WriteSWPRegister(M24M01E_Object_t *pObj, 
                                     uint8_t * const pData,
                                     const uint32_t TarAddr, 
                                     const uint16_t NbByte)
{
  int32_t status = M24_OK;
  uint8_t Devaddr = M24M01E_SWP_DevSelCode & M24M01E_DEVICE_ADDR_WRITE_BIT;
  
  if(pObj->IO.IsReady(Devaddr, MIN_TRIALS) != M24_OK)
  {
    status = M24_TIMEOUT;
  }
  else
  {
    status = pObj->IO.WriteReg16(Devaddr, TarAddr, pData, NbByte);
    pObj->IO.Delay(5);
  }
  return status;
  
}

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

