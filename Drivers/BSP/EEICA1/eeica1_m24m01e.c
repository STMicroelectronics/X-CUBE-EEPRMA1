/**
******************************************************************************
* @file    eeica1_m24m01e.c
* @author  SRA-SAIL, STM32ODE
* @brief   This file provides X-NUCLEO-EEICA1 Board 
*          specific functions
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
#include "eeica1_m24m01e.h"

/** @addtogroup BSP
* @{
*/
static M24M01E_EEPROM_CommonDrv_t *M24M01EDrv[EEICA1_M24_INSTANCES_NBR];
void *M24M01ECompObj[EEICA1_M24_INSTANCES_NBR];
/** @defgroup M24 EEPROM Private Function Prototypes
* @{
*/

static int32_t M24M01E_Probe(void);

/* Global variables ----------------------------------------------------------*/

uint8_t M24M01E_CDA_DevSelCode = 0xB0;
uint8_t M24M01E_DTI_DevSelCode = 0xB0;
uint8_t M24M01E_IDPage_DevSelCode = 0xB0;
uint8_t M24M01E_SWP_DevSelCode = 0xB0;
uint8_t M24M01E_Memory_DevSelCode = 0xA0;

uint16_t M24M01E_DTI_DevAddr = M24M01E_DTI_ADDRESS;

/**
* @}
*/

/** @defgroup EEPRMA2_M24_Exported_Functions 
* @{
*/

/**
* @brief  This function gives high on selected write control pin
* @param  None
* @retval None
*/  
void EEPROMEX_M24M01_WC_HIGH(void)
{
  HAL_GPIO_WritePin(EEICA1_M24M01_WC2_PORT, EEICA1_M24M01_WC2_PIN, GPIO_PIN_SET);
}

/**
* @brief  This function gives low on selected write control pin
* @param  None
* @retval None
*/  
void EEPROMEX_M24M01_WC_LOW(void)
{
  HAL_GPIO_WritePin(EEICA1_M24M01_WC2_PORT, EEICA1_M24M01_WC2_PIN, GPIO_PIN_RESET);
}

/**
* @brief  Initializes the I2C EEPROM
* @param  Instance : I2C EEPROM instance to be used
* @retval BSP status
*/
int32_t EEICA1_M24M01E_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  switch (Instance)
  {
  case EEICA1_M24M01E:
    if (M24M01E_Probe() != BSP_ERROR_NONE)
    {
      return BSP_ERROR_NO_INIT;
    }
    break;
  default:
    ret = BSP_ERROR_WRONG_PARAM;
    break;
  }
  
  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }
  return ret;
}  

/**
* @brief  De-initialize the I2C EEPROM
* @param  Instance : I2C EEPROM instance to be used
* @retval BSP status
*/
int32_t EEICA1_M24M01E_DeInit(uint32_t Instance)
{
  int32_t ret;
  
  if(Instance >= EEICA1_M24_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(M24M01EDrv[Instance]->DeInit(M24M01ECompObj[Instance]) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret;
}

/**
* @brief  Checks if the memory is available
* @param  Instance : I2C EEPROM instance to be used
* @param  Trials : Number of trials
* @retval BSP status
*/
int32_t EEICA1_M24M01E_IsDeviceReady(uint32_t Instance, const uint32_t Trials)
{
  int32_t ret;
  
  if(M24M01EDrv[Instance]->IsReady(M24M01ECompObj[Instance], Trials) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  return ret;
}	

/**
* @brief  Reads data in identification page of the memory at specific address
* @param  Instance : I2C EEPROM instance to be used
* @param  pData   : pointer to the data to read
* @param  TarAddr : memory address to read
* @retval BSP status
*/
int32_t EEICA1_M24M01E_ReadByte(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr)
{
  int32_t ret;
  
  if (M24M01EDrv[Instance]->ReadByte(M24M01ECompObj[Instance], pData, TarAddr)!= BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret; 
  
}

/**
* @brief  Reads data in the memory at specific address
* @param  Instance : I2C EEPROM instance to be used
* @param  pData   : pointer to the data to write
* @param  TarAddr : memory address to write
* @param  Size    : Size in bytes of the value to be written
* @retval BSP status
*/
int32_t EEICA1_M24M01E_ReadData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if (M24M01EDrv[Instance]->ReadData(M24M01ECompObj[Instance], pData, TarAddr, Size) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret; 
}

/**
* @brief  Reads complete page from the memory at page start address
* @param  Instance : I2C EEPROM instance to be used
* @param  pData   : pointer to the data to read       
* @param  TarAddr : starting page address to read
* @param  Size    : Size in bytes of the value to be written
* @retval BSP status
*/
int32_t EEICA1_M24M01E_ReadPage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if ((TarAddr + Size)> M24M01E_MEMORYSIZE)
    return BSP_ERROR_WRONG_PARAM;
  
  uint32_t iNumberOfPage = (TarAddr + Size) / M24M01E_PAGESIZE;
  uint32_t iRemainder = (TarAddr + Size) % M24M01E_PAGESIZE;
  
  uint32_t PageAddress = TarAddr * M24M01E_PAGESIZE;        
  uint32_t iPageNumber = TarAddr;
  if(iRemainder != 0U)
  { 
    iNumberOfPage += 1U;   
  }
  
  if(iNumberOfPage <= 1U)
  {
    if(M24M01EDrv[Instance]->ReadPage(M24M01ECompObj[Instance], pData, PageAddress, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }
  else
  {
    for (uint32_t iCounter=0; iCounter<iNumberOfPage; iCounter++)
    {
      uint32_t iPageAddress = iPageNumber * M24M01E_PAGESIZE;
      ret = M24M01EDrv[Instance]->ReadPage(M24M01ECompObj[Instance], &pData[0U + (iCounter*M24M01E_PAGESIZE)], iPageAddress, M24M01E_PAGESIZE);     
      iPageNumber++;
      HAL_Delay(5);  
    }
  }    
  
  return ret; 
}

/**
* @brief  Write Byte in the memory at specific address
* @param  Instance : the device instance
* @param  pData : pointer to the data to write
* @param  TarAddr : I2C data memory address to write
* @retval BSP status
*/
int32_t EEICA1_M24M01E_WriteByte(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr)
{
  int32_t ret;
  
  EEPROMEX_M24M01_WC_LOW();
  
  if(M24M01EDrv[Instance]->WriteByte(M24M01ECompObj[Instance], pData, TarAddr) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  { 
    while(EEICA1_M24M01E_IsDeviceReady(Instance, MIN_TRIALS) != BSP_ERROR_NONE) {};
    ret = BSP_ERROR_NONE;
  }
  
  EEPROMEX_M24M01_WC_HIGH();
  
  return ret; 
  
}

/**
* @brief  Write Data in the memory at specific address
* @param  Instance : the device instance
* @param  pData : pointer to the data to write
* @param  TarAddr : I2C data memory address to write
* @param  Size : Size in bytes of the value to be written
* @retval BSP status
*/
int32_t EEICA1_M24M01E_WriteData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret;
  
  if((TarAddr + Size)> M24M01E_MEMORYSIZE)
    return BSP_ERROR_WRONG_PARAM;
  
  EEPROMEX_M24M01_WC_LOW();
  
  if(M24M01EDrv[Instance]->WriteData(M24M01ECompObj[Instance], pData, TarAddr, M24M01E_PAGESIZE, Size) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  EEPROMEX_M24M01_WC_HIGH();
  
  return ret; 
  
}

/**
* @brief  Write Data in the memory at specific address
* @param  Instance : the device instance
* @param  pData : pointer to the data to write
* @param  TarAddr : I2C data memory address to write
* @param  Size : Size in bytes of the value to be written
* @retval BSP status
*/
int32_t EEICA1_M24M01E_WritePage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if ((TarAddr + Size)> M24M01E_MEMORYSIZE)
    return BSP_ERROR_WRONG_PARAM;
  
  /*Target Address set in pagewrite function is basically the page number */ 
  uint32_t iPageNumber = TarAddr;  
  uint16_t iNumberOfPage = (Size) / M24M01E_PAGESIZE;
  uint16_t iRemainder = (Size) % M24M01E_PAGESIZE;
  /* PageWrite function should be begin at starting address of the page */
  if (iRemainder != 0U)  
  {
    iNumberOfPage += 1U;
  }
  
  if (iNumberOfPage <= 1U)
  {
    /* Absolute address to write, depending on the page number(TarAddr is the pagenumber)  */
    uint32_t iPageAddress = TarAddr * M24M01E_PAGESIZE;
    if (M24M01EDrv[Instance]->WritePage(M24M01ECompObj[Instance], pData, iPageAddress, M24M01E_PAGESIZE, Size) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }
  else
  {
    for (uint32_t iCounter=0; iCounter<iNumberOfPage; iCounter++)
    {
      uint32_t iPageAddress = iPageNumber * M24M01E_PAGESIZE;
      ret = M24M01EDrv[Instance]->WritePage(M24M01ECompObj[Instance], &pData[0U + (iCounter*M24M01E_PAGESIZE)], iPageAddress, M24M01E_PAGESIZE, Size);     
      iPageNumber++;
      HAL_Delay(5);  
    }
  }   
  
  return ret; 
  
}

/**
* @brief  Writes data in identification page of the memory at specific address
* @param  Instance : the device instance
* @param  pData : pointer to the data to write
* @param  TarAddr : I2C data memory address to write
* @param  Size : Size in bytes of the value to be written
* @retval BSP status
*/
int32_t EEICA1_M24M01E_WriteIDPage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size )
{
  int32_t ret;
  
  EEPROMEX_M24M01_WC_LOW();
  
  if(M24M01EDrv[Instance]->WriteIDPage(M24M01ECompObj[Instance],  pData, TarAddr, Size ) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  EEPROMEX_M24M01_WC_HIGH();
  
  return ret; 
  
}

/**
* @brief  Reads data in identification page of the memory at specific address
* @param  Instance : the device instance
* @param  pData   : pointer to the data to write
* @param  TarAddr : memory address to write
* @param  Size    : Size in bytes of the value to be written
* @retval BSP status
*/
int32_t EEICA1_M24M01E_ReadIDPage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret;
  
  if(M24M01EDrv[Instance]->ReadIDPage( M24M01ECompObj[Instance], pData, TarAddr, Size ) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret; 
  
}

/**
* @brief  Lock identification page of the memory
* @param  Instance : memory name to write
* @retval BSP status
*/
int32_t EEICA1_M24M01E_LockIDPage( uint32_t Instance)
{
  int32_t ret;
  
  EEPROMEX_M24M01_WC_LOW();
  
  if (M24M01EDrv[Instance]->LockIDPage(M24M01ECompObj[Instance])!= BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  EEPROMEX_M24M01_WC_HIGH();
  
  return ret; 
}

/**
* @brief  Read CDA register
* @param  Instance : the device instance
* @param  pData   : pointer to the data to write
* @retval BSP status
*/
int32_t EEICA1_M24M01E_ReadCDARegister(uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  
  if ((M24M01EDrv[Instance]->ReadCDARegister(M24M01ECompObj[Instance], pData, M24M01E_CDA_ADDRESS, 1))  != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret;
  
}

/**
* @brief  Read DTI register
* @param  Instance : the device instance
* @param  pData   : pointer to the data to write
* @retval BSP status
*/
int32_t EEICA1_M24M01E_ReadDTIRegister(uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  
  if ((M24M01EDrv[Instance]->ReadDTIRegister(M24M01ECompObj[Instance], pData, M24M01E_DTI_ADDRESS, 1))  != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret;
  
}

/**
* @brief  Write CDA register
* @param  Instance : the device instance
* @param  pData   : pointer to the data to write
* @retval BSP status
*/
int32_t EEICA1_M24M01E_WriteCDARegister(uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  
  EEPROMEX_M24M01_WC_LOW();
  
  if ((M24M01EDrv[Instance]->WriteCDARegister(M24M01ECompObj[Instance], pData, M24M01E_CDA_ADDRESS, 1))  != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    M24M01E_CDA_DevSelCode = (M24M01E_CDA_DevSelCode & 0xF0U) | (*pData);
    M24M01E_DTI_DevSelCode = (M24M01E_DTI_DevSelCode & 0xF0U) | (*pData);
    M24M01E_IDPage_DevSelCode = (M24M01E_IDPage_DevSelCode & 0xF0U) | (*pData);
    M24M01E_SWP_DevSelCode = (M24M01E_SWP_DevSelCode & 0xF0U) | (*pData);
    M24M01E_Memory_DevSelCode = (M24M01E_Memory_DevSelCode & 0xF0U) | (*pData);
    
    ret = BSP_ERROR_NONE;
  }
  
  EEPROMEX_M24M01_WC_HIGH();
  
  return ret;
  
}

/**
* @brief  Read SWP register
* @param  Instance : the device instance
* @param  pData   : pointer to the data to write
* @retval BSP status
*/
int32_t EEICA1_M24M01E_ReadSWPRegister(uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  
  if((M24M01EDrv[Instance]->ReadSWPRegister(M24M01ECompObj[Instance], pData, M24M01E_SWP_ADDRESS, 1))  != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret;
  
}

/**
* @brief  Write SWP register
* @param  Instance : the device instance
* @param  pData   : pointer to the data to write
* @retval BSP status
*/
int32_t EEICA1_M24M01E_WriteSWPRegister(uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  
  EEPROMEX_M24M01_WC_LOW();
  
  if ((M24M01EDrv[Instance]->WriteSWPRegister(M24M01ECompObj[Instance], pData, M24M01E_SWP_ADDRESS, 1))  != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  EEPROMEX_M24M01_WC_HIGH();
  return ret;
  
}

/**
* @brief  Register Bus IOs for instance M24M01E
* @retval BSP status
*/
static int32_t M24M01E_Probe(void)
{
  M24M01E_IO_t io_ctxm01;
  int32_t ret = BSP_ERROR_NONE;
  static M24M01E_Object_t M24M01_obj_0;
  
  io_ctxm01.Address     = M24M01E_CDA_DevSelCode;
  io_ctxm01.Init        = EEICA1_I2C_INIT;
  io_ctxm01.DeInit      = EEICA1_I2C_DEINIT;
  io_ctxm01.ReadReg     = EEICA1_I2C_READREG;
  io_ctxm01.WriteReg    = EEICA1_I2C_WRITEREG;
  io_ctxm01.ReadReg16   = EEICA1_I2C_READREG16;
  io_ctxm01.WriteReg16  = EEICA1_I2C_WRITEREG16;
  io_ctxm01.Transmit    = EEICA1_I2C_SEND;
  io_ctxm01.IsReady     = EEICA1_I2C_ISREADY;
  io_ctxm01.Delay       = EEICA1_M24_DELAY;
  if (M24M01E_RegisterBusIO(&M24M01_obj_0, &io_ctxm01) != M24_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  
  M24M01ECompObj[EEICA1_M24M01E] = &M24M01_obj_0;
  M24M01EDrv[EEICA1_M24M01E] = (M24M01E_EEPROM_CommonDrv_t *)(void *)&M24M01E_i2c_Drv;
  
  if (M24M01EDrv[EEICA1_M24M01E]->Init(M24M01ECompObj[EEICA1_M24M01E]) != M24_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }  
  return ret;
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

