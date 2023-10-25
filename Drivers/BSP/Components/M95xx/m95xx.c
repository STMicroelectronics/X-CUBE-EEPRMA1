/**
  ******************************************************************************
  * @file    m95xx.c
  * @author  SRA-SAIL, STM32ODE
  * @brief   file provides functionality of driver functions and to manage communication 
             between BSP and M95xx chip
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
#include "m95xx.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */ 

/** @defgroup M95
  * @brief This file provides set of driver functions to manage communication
  *        between BSP and M95xx chip.
  * @{
  */

/* Global variables ---------------------------------------------------------*/
/** @defgroup M95_Global_Variables
  * @{
  */ 

M95_Drv_t M95_spi_Drv =
{
  M95_spi_Init,
  M95_spi_DeInit,
  M95_spi_IsDeviceReady,
  M95_spi_ReadReg,
  M95_spi_WriteReg,
  M95_spi_ReadByte,
  M95_spi_WriteByte,
  M95_spi_ReadPage,
  M95_spi_WritePage,
  M95_spi_ReadData,
  M95_spi_WriteData,
  M95_spi_WriteID,
  M95_spi_ReadID,
  M95_spi_LockID, 
  M95_spi_LockStatus,
  NULL
};
/**
  * @}
  */ 

/* Public functions ---------------------------------------------------------*/
/** @defgroup M95_Public_Functions
  * @{
  */
/*!
 * @brief IO function pointer structure
 */
/* -------- Static functions prototypes --------------------------------------------- */

int32_t M95_RegisterBusIO(M95_Object_t *pObj, M95_IO_t *pIO)
{
  pObj->IO.Address         = pIO->Address;
  pObj->IO.Init            = pIO->Init;
  pObj->IO.DeInit          = pIO->DeInit;
  pObj->IO.Read 	   = pIO->Read;
  pObj->IO.Write           = pIO->Write;
  pObj->IO.WriteBuffer     = pIO->WriteBuffer;
  pObj->IO.ReadBuffer      = pIO->ReadBuffer;
  pObj->IO.IsReady         = pIO->IsReady;
  pObj->IO.Delay           = pIO->Delay;
  	
  
  if(!pObj->IO.Init)
  {
    return M95_ERROR;
  }
  return M95_OK;
}

/**
  * @brief  Set M95 eeprom Initialization
  * @param  pObj : pointer to memory object
  * @retval M95 status
  */
int32_t M95_spi_Init(M95_Object_t *pObj) 
{
  
    /* Configure the low level interface */
  if (pObj->IO.Init()<0)
  {
    return M95_ERROR;
  }
  return M95_OK;
}

/**
  * @brief  Set M95 eeprom De-Initialization
  * @param  pObj : pointer to memory object
  * @retval M95 status
  */
int32_t M95_spi_DeInit( M95_Object_t *pObj )
{
   if (pObj->IO.DeInit()< 0)
 {
   return M95_ERROR;
 }
  /* Configure the low level interface */
  return M95_OK;
}

/**
  * @brief  Check M95 availability
  * @param  pObj : pointer to memory object
  * @retval M95 status
  */
int32_t M95_spi_IsDeviceReady( M95_Object_t *pObj ) 
{   
   return pObj->IO.IsReady( pObj->IO.Address );
}

/**
  * @brief  Read status registor of selected SPI memory
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store content of status register 
  * @retval M95 status
  */
int32_t M95_spi_ReadReg( M95_Object_t *pObj, uint8_t * pData) 
{
  pObj->IO.IsReady( pObj->IO.Address );
  return pObj->IO.Read( pData, pObj->IO.Address);
}
/**
  * @brief  Write status registor of selected SPI memory
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write to status registor of SPI memory    
  * @retval M95 status
  */
int32_t M95_spi_WriteReg(M95_Object_t *pObj,uint8_t pData)
{

  pObj->IO.IsReady( pObj->IO.Address );
  return pObj->IO.Write( pData, pObj->IO.Address );
}

/**
  * @brief  Read single byte from specified SPI address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store
  * @param  TarAddr : SPI data memory address to read
  * @retval M95 status
  */
int32_t M95_spi_ReadByte(M95_Object_t *pObj, uint8_t * const pData, const uint32_t TarAddr )
{ 
  int32_t status;
  
  pObj->IO.IsReady( pObj->IO.Address );
  /* Condition Matters only for 4Kb SPI ie M95040, for others EEPROMEX_WRITE & EEPROMEX_UPWRITE are same */
  if (pObj->IO.Address == 0xC6U)
  {
    if (TarAddr < 256U) /* Lower Half for 4Kbit  */
      status = pObj->IO.ReadBuffer( pData, TarAddr, pObj->IO.Address, 1 ,EEPROMEX_READ);
    else
      status = pObj->IO.ReadBuffer( pData, TarAddr, pObj->IO.Address, 1 ,EEPROMEX_UPREAD);
  }
  else
    status = pObj->IO.ReadBuffer( pData, TarAddr, pObj->IO.Address, 1 ,EEPROMEX_READ);
  return status;
}

/**
  * @brief  Read full page of the memory
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store
  * @param  TarAddr : SPI data memory address to read
  * @param  PageSize : Size of the page of selected memory
  * @retval M95 status
  */
int32_t M95_spi_ReadPage(M95_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize )
{    		
  int32_t status = M95_OK;
  
  while( pObj->IO.IsReady( pObj->IO.Address ) != M95_OK ) {};
  
  status = M95_spi_ReadData( pObj, pData, TarAddr, PageSize );
  
  return status;
}
/**
  * @brief  Read N bytes starting from specified SPI address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store
  * @param  TarAddr : SPI data memory address to read
  * @param  Size : number of bytes to read
  * @retval M95 status
  */
int32_t M95_spi_ReadData( M95_Object_t *pObj,uint8_t * pData, const uint32_t TarAddr, 
                                          const uint16_t Size )
{ 
  int32_t status = M95_OK;
  uint32_t targetAddress = TarAddr;
  
  pObj->IO.IsReady( pObj->IO.Address );
  
  if (pObj->IO.Address == 0xC6U)     /* Required for 4Kb SPI EEPROM only*/
  {
   if ((TarAddr + Size) <= 256U) /* Lower Half for 4Kbit */
    status = pObj->IO.ReadBuffer( pData, TarAddr,  pObj->IO.Address, Size, EEPROMEX_READ );
   else if (TarAddr > 256U)
     status =  pObj->IO.ReadBuffer( pData, TarAddr,  pObj->IO.Address, Size, EEPROMEX_UPREAD );
   else if (((TarAddr + Size) > 256U)&&(TarAddr <= 256U))
   { 
     uint32_t temp1,temp2;
     temp1=(256U - targetAddress);        /* no. of bytes in lower half */ 
     pObj->IO.ReadBuffer( pData, targetAddress,  pObj->IO.Address, temp1, EEPROMEX_READ );
     targetAddress += temp1;
     temp2=(TarAddr + Size)-256U; /* no. of bytes in upper half */
     status =  pObj->IO.ReadBuffer( &pData[0U+temp1], targetAddress,  pObj->IO.Address, temp2, EEPROMEX_UPREAD );
   }
  }
  
  else
    status =  pObj->IO.ReadBuffer( pData, TarAddr,  pObj->IO.Address, Size, EEPROMEX_READ);
     
  return status;
}

/**
  * @brief  Write a single byte to a specified address of SPI memory
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : SPI data memory address to write
  * @retval M95 status
*/
int32_t M95_spi_WriteByte(M95_Object_t *pObj,uint8_t * pData, const uint32_t TarAddr)
{
  int32_t status;
 
  pObj->IO.IsReady( pObj->IO.Address );
  /* Condition Matters only for 4Kb SPI ie M95040, for others EEPROMEX_WRITE & EEPROMEX_UPWRITE are same */
  if (pObj->IO.Address == 0xC6U)
  {
    if (TarAddr < 256U)
      status = pObj->IO.WriteBuffer( pData, TarAddr, pObj->IO.Address, 1 ,EEPROMEX_WRITE);
    else
      status = pObj->IO.WriteBuffer( pData, TarAddr, pObj->IO.Address, 1 ,EEPROMEX_UPWRITE);
  }
  else
    status = pObj->IO.WriteBuffer( pData, TarAddr, pObj->IO.Address, 1 ,EEPROMEX_WRITE);
  

  pObj->IO.IsReady( pObj->IO.Address );
  return status;
}

/**
  * @brief  Write maximum of pagesize bytes starting from specified SPI Address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : SPI data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to write
  * @retval M95 status
  */
int32_t M95_spi_WritePage( M95_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr, 
                                          const uint16_t PageSize,const uint16_t NByte)
{
  int32_t status = M95_OK;
  while( pObj->IO.IsReady( pObj->IO.Address ) != M95_OK ) {};
    
  status = M95_spi_WriteData(pObj, pData, TarAddr , PageSize, NByte );
  
  return status;
}

/**
  * @brief  Write N data bytes starting from specified SPI Address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : SPI data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  Size : number of bytes to write
  * @retval M95 status
  */
int32_t M95_spi_WriteData(M95_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr ,
                          const uint16_t PageSize, const uint16_t Size )
{
  int32_t status = M95_OK;
  
  uint32_t targetAddress = TarAddr;
  uint32_t remainingSize = Size;
  
  /* Check for invalid inputs */
  if ((pObj == NULL) || (pData == NULL) || (PageSize == 0U) || (remainingSize == 0U)) 
  {
    return M95_ERROR;  /* Return an error code indicating invalid inputs */
  }
  
  /* Calculate the starting page and offset */
  uint32_t startOffset = TarAddr % PageSize;
  uint32_t offset = startOffset;
  
  if (pObj->IO.IsReady( pObj->IO.Address ) !=M95_OK)
  {
    return M95_ERROR;
  } 
  
  /*  Iterate over the pages and write the data */
  while (remainingSize > 0U) 
  {
    uint32_t bytesToWrite = (remainingSize < (PageSize - offset)) ? remainingSize : (PageSize - offset);
    
    /* Condition Matters only for 4Kb SPI ie M95040, for others EEPROMEX_WRITE & EEPROMEX_UPWRITE are same */
    if (pObj->IO.Address == 0xC6U) 
    {          
      if (targetAddress < 256U)
      {
        status = pObj->IO.WriteBuffer( pData, targetAddress, pObj->IO.Address, bytesToWrite ,EEPROMEX_WRITE);
      }
      else
      {
        status = pObj->IO.WriteBuffer( pData, targetAddress, pObj->IO.Address, bytesToWrite ,EEPROMEX_UPWRITE);
      }
    }
    else
    {
      status = pObj->IO.WriteBuffer( pData, targetAddress, pObj->IO.Address, bytesToWrite ,EEPROMEX_WRITE);
    }
    
    pObj->IO.Delay(6);
    
    /* Update the pointers and sizes for the next page */
    pData += bytesToWrite;
    targetAddress += bytesToWrite;
    remainingSize -= bytesToWrite;
    offset = targetAddress % PageSize;
    
    /* Wait for the SPI interface to be ready before proceeding to the next page */
    while( pObj->IO.IsReady( pObj->IO.Address ) != M95_OK ) {};
  }
  return status;     
}

/**
  * @brief  Write Identification Page
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : SPI data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to write
  * @retval M95 status
  */
int32_t M95_spi_WriteID(M95_Object_t *pObj ,uint8_t * pData, const uint32_t TarAddr,
                               const uint16_t PageSize, const uint16_t NbByte)
{
  int32_t status = M95_OK;
  
  uint32_t temp_TarAddr;
  while( pObj->IO.IsReady( pObj->IO.Address ) != M95_OK ) {};
  
  switch(pObj->IO.Address)
  {
  case 0xC6:
    temp_TarAddr = TarAddr & 0x0FU;    
    break;
    
  case 0xC9:
    temp_TarAddr = TarAddr & 0x3FU;
    break;
    
  case 0xCC:
    temp_TarAddr = TarAddr & 0x1FFU;
    break;
    
  default:
    temp_TarAddr = 0x0U;
    status = M95_ERROR; 
    break;
    
  }
  
  if (status == M95_OK)
  {
    if((temp_TarAddr + NbByte) >= PageSize)
    {
      status = M95_ADDR_OVERFLOW;
    }
    else
    {
      status = pObj->IO.WriteBuffer( pData, temp_TarAddr, pObj->IO.Address, NbByte, EEPROMEX_WRID);    
    }
    
  }
  
  return status;
}

/**
  * @brief  Read Identification Page
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to read
  * @param  TarAddr : SPI data memory address to read
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to write
  * @retval EEPROMEX enum status
  */
int32_t M95_spi_ReadID( M95_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                       const uint16_t PageSize, const uint16_t NbByte )
{
  
  int32_t status = M95_OK;
  uint32_t temp_TarAddr;
  while( pObj->IO.IsReady( pObj->IO.Address ) != M95_OK ) {};
  
  switch(pObj->IO.Address)
  {
  case 0xC6:
    temp_TarAddr = TarAddr & 0x0FU;    
    break;
    
  case 0xC9:
    temp_TarAddr = TarAddr & 0x3FU;
    break;
    
  case 0xCC:
    temp_TarAddr = TarAddr & 0x1FFU;
    break;
    
  default:
    temp_TarAddr = 0x0U;
    status =  M95_ERROR;
    break;
    
  }
  
  if (status == M95_OK)
  {
    if((temp_TarAddr + NbByte) >= PageSize)
    {
      status = M95_ADDR_OVERFLOW;
    }
    else
    {
      status = pObj->IO.ReadBuffer( pData, temp_TarAddr, pObj->IO.Address, NbByte, EEPROMEX_RDID);    
    }
    
  } 
  
  return status;
}

/**
  * @brief  Reads the Identification Page lock status
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to read
  * @retval M95 status
  */
int32_t M95_spi_LockStatus( M95_Object_t *pObj, uint8_t * pData )
{
  int32_t status = M95_OK;
  while( pObj->IO.IsReady( pObj->IO.Address ) != M95_OK ) {};
  
  status = pObj->IO.ReadBuffer(pData, ADDRLID_SPI, pObj->IO.Address, 1, EEPROMEX_RDLS);
  
  return status;
}

/**
  * @brief  Locks the Identification page in read-only mode
  * @param  pObj : pointer to memory object
  * @retval M95 status
  */
int32_t M95_spi_LockID(M95_Object_t *pObj)
{
  int32_t status = M95_OK;
  uint8_t lock_data;
  while( pObj->IO.IsReady( pObj->IO.Address ) != M95_OK ) {};
  
  if(pObj->IO.Address == 0xCCU)
  {
    lock_data = LOCKDATA_SPI_M04;    
  }
  else
  {
    lock_data = LOCKDATA_SPI;    
  }
  
  status = pObj->IO.WriteBuffer( &lock_data, ADDRLID_SPI, pObj->IO.Address, 1, EEPROMEX_LID);  

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


