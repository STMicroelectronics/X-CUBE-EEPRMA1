/**
  ******************************************************************************
  * @file    m24xx.c
  * @author  SRA-SAIL, STM32ODE
  * @brief   file provides functionality of driver functions and to manage communication 
             between BSP and M24xx chip
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
#include "m24xx.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */ 


/** @defgroup M24
  * @brief This file provides set of driver functions to manage communication
  *        between BSP and M24xx chip.
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup M24_Private_Defines
  * @{
  */  
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/


/* Global variables ---------------------------------------------------------*/
/** @defgroup M24_Global_Variables
  * @{
  */

M24_Drv_t M24_i2c_Drv =
{
  M24_i2c_Init,
  M24_i2c_DeInit,
  M24_i2c_IsDeviceReady,
  NULL,
  NULL,
  M24_i2c_ReadByte,
  M24_i2c_WriteByte,
  M24_i2c_ReadPage,
  M24_i2c_WritePage,  
  M24_i2c_ReadData,
  M24_i2c_WriteData,
  M24_i2c_WriteID,
  M24_i2c_ReadID,
  M24_i2c_LockID, 
  M24_i2c_LockStatus,
  NULL
};
/**
  * @}
  */ 

/* Public functions ---------------------------------------------------------*/
/** @defgroup M24_Public_Functions
  * @{
  */

/*!
 * @brief IO function pointer structure
 */

/* -------- Static functions prototypes --------------------------------------------- */
/* Exported functions ---------------------------------------------------------*/
int32_t M24_RegisterBusIO(M24_Object_t *pObj, M24_IO_t *pIO)
{
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
    return M24_ERROR;
  }
  return M24_OK;
}
/**
  * @brief  Set M24 eeprom Initialization
  * @param  pObj : pointer to memory object
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_Init( M24_Object_t *pObj )
{ 
  /* Configure the low level interface */
  if (pObj->IO.Init()<0)
  {
    return M24_ERROR;
  }
  return M24_OK;
}

/**
  * @brief  Set M24 eeprom De-Initialization
  * @param  pObj : pointer to memory object
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_DeInit( M24_Object_t *pObj )
{ 
 if (pObj->IO.DeInit()< 0)
 {
   return M24_ERROR;
 }
  /* Configure the low level interface */
  return M24_OK;
}

/**
  * @brief  Check M24 availability
  * @param  pObj : pointer to memory object
  * @param  Trials : number of max tentative tried
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_IsDeviceReady(M24_Object_t *pObj, const uint32_t Trials )
{
  /* Test i2c with M24 */
  return pObj->IO.IsReady( pObj->IO.Address, Trials );
}

/**
  * @brief  Read N bytes starting from specified I2C address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadByte( M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr)
{  
  /* Before calling this function M24xx must be ready, here is a check to detect an issue */
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
      return M24_TIMEOUT;
    }  
  
  if (pObj->IO.Address == 0xA8U)
    return pObj->IO.ReadReg(pObj->IO.Address, TarAddr, pData, 1);
  else
    return pObj->IO.ReadReg16(pObj->IO.Address, TarAddr, pData, 1);
}

/**
  * @brief  Read N bytes starting from specified I2C address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to read
  * @param  PageSize : memory page size
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadPage( M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize )
{  
  /* Before calling this function M24xx must be ready, here is a check to detect an issue */
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
      return M24_TIMEOUT;
    }
  
  if (pObj->IO.Address == 0xA8U)
    return pObj->IO.ReadReg(pObj->IO.Address, TarAddr, pData, PageSize );
  else
    return pObj->IO.ReadReg16(pObj->IO.Address, TarAddr, pData, PageSize );
}

/**
  * @brief  Read N bytes starting from specified I2C address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to read
  * @param  NbByte : number of bytes to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadData(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr, 
                           const uint16_t NbByte )
{  
  /* Before calling this function M24xx must be ready, here is a check to detect an issue */
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
    return M24_TIMEOUT;
    }
  
  if (pObj->IO.Address == 0xA8U)
    return pObj->IO.ReadReg(pObj->IO.Address, TarAddr, pData, NbByte );
  else
    return pObj->IO.ReadReg16(pObj->IO.Address, TarAddr, pData, NbByte );
}

/**
  * @brief  Write a single byte to a specified address of I2C memory
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @retval EEPROMEX enum status
*/
int32_t M24_i2c_WriteByte( M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr)
{
  int32_t status;
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
      return M24_TIMEOUT;
    }
  
  if (pObj->IO.Address == 0xA8U)
    status = pObj->IO.WriteReg(pObj->IO.Address, TarAddr, pData, 1 );
  else
    status = pObj->IO.WriteReg16(pObj->IO.Address, TarAddr, pData, 1 );
  
  while( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS )  != M24_OK ) {};  
  return status;
}

/**
  * @brief  Write Page
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to write
  * @retval EEPROMEX enum status
*/
int32_t M24_i2c_WritePage(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                          const uint16_t PageSize,const uint16_t NByte)
{
  int32_t status;
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK)
  {
    return M24_TIMEOUT;
  }    
  
  
  if (pObj->IO.Address == 0xA8U)
    status = pObj->IO.WriteReg(pObj->IO.Address, TarAddr, pData, PageSize);
  else
    status = pObj->IO.WriteReg16(pObj->IO.Address, TarAddr, pData, PageSize);
  while( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) 
  {
  };
    return status; 
}

/**
  * @brief  Write N data bytes starting from specified I2C Address
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  Size : number of bytes to write
  * @retval EEPROMEX enum status
  */

int32_t M24_i2c_WriteData(M24_Object_t *pObj, uint8_t *pData, const uint32_t TarAddr,
                          const uint16_t PageSize, const uint16_t Size)
{
    int32_t status = M24_OK;
    
    uint32_t targetAddress = TarAddr;
    uint32_t remainingSize = Size;
    
    /* Check for invalid inputs */
    if ((pObj == NULL) || (pData == NULL) || (PageSize == 0U) || (remainingSize == 0U)) 
    {
        return M24_ERROR;  /* Return an error code indicating invalid inputs */
    }
    
    /* Calculate the starting page and offset */
    uint32_t startOffset = TarAddr % PageSize;
    uint32_t offset = startOffset;
    
    if (pObj->IO.IsReady(pObj->IO.Address, MIN_TRIALS) != M24_OK)
    {
        return M24_TIMEOUT;
    }
    
    /* Iterate over the pages and write the data */
    while (remainingSize > 0U) 
    {
        uint32_t bytesToWrite = (remainingSize < (PageSize - offset)) ? remainingSize : (PageSize - offset);
        
        /* Perform the write operation using the appropriate I2C write function */
        if (pObj->IO.Address == 0xA8U)
        {
            status = pObj->IO.WriteReg(pObj->IO.Address, targetAddress, pData, bytesToWrite);
        }
        else
        {
            status = pObj->IO.WriteReg16(pObj->IO.Address, targetAddress, pData, bytesToWrite);
        }

        pObj->IO.Delay(6);
        
        /* Update the pointers and sizes for the next page */
        pData += bytesToWrite;
        targetAddress += bytesToWrite;
        remainingSize -= bytesToWrite;
        offset = targetAddress % PageSize;
        
        /* Wait for the I2C interface to be ready before proceeding to the next page */
        while (pObj->IO.IsReady(pObj->IO.Address, MIN_TRIALS) != M24_OK)
        {
        }
    }

    return status;
}

/**
  * @brief  Write Identification Page
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to write
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_WriteID(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                        const uint16_t PageSize, const uint16_t NbByte ){
  int32_t status;
  uint16_t  Devaddr;
  uint16_t mem_addr;
  uint16_t  count;
  uint16_t temp;
  uint16_t  bitcount;
  uint16_t mask = 0U ;
  
  /* Check for invalid inputs */
  if (PageSize == 0U) 
  {
    return M24_ERROR;  /* Return an error code indicating invalid inputs */
  }
    
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
  {
    return M24_TIMEOUT;
  }
  uint8_t *pdata_index = (uint8_t *)pData;
  Devaddr = pObj->IO.Address;
  bitcount = BITCOUNT;
  temp = PageSize;
  mem_addr = ( uint16_t )TarAddr;
  Devaddr |= 1U << IDMASK;                  /* Set fifth bit of Device address */
  while ( (uint16_t)(temp / ( (uint16_t)1U << bitcount )) != 0U ){  /* Generate mask for address*/
   mask |= ( (uint16_t)1U << (bitcount - 1U) );
   bitcount++;
  }
  mem_addr &= mask;                        /* Mask address address according to pagesize*/  
  count = PageSize - (mem_addr % PageSize);  /* Calculate available space in the ID page */
  if ( NbByte <= count ){  /* Address byte is not aligned with page and no byte must be less than available byte in ID page*/
    if (pObj->IO.Address == 0xA8U)
        status = pObj->IO.WriteReg(Devaddr, mem_addr,  pdata_index, NbByte );
    else
        status = pObj->IO.WriteReg16(Devaddr, mem_addr,  pdata_index, NbByte );
  }
  else 
    return M24_ERROR;   /* Return error if above two condtions does'nt met */
  while(  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) {};
  return status; 
}

/**
  * @brief  Read Identification Page
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadID(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                       const uint16_t PageSize, const uint16_t NbByte )
{
  uint16_t  Devaddr;
  uint16_t mem_addr;
  uint16_t count;
  uint16_t temp;
  uint16_t bitcount;
  uint16_t mask;
  
  /* Check for invalid inputs */
  if (PageSize == 0U) 
  {
    return M24_ERROR;  /* Return an error code indicating invalid inputs */
  }
  
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK)
  {
    return M24_TIMEOUT;
  }
  mem_addr = ( uint16_t )TarAddr;
  Devaddr = pObj->IO.Address;
  mask = 0U;
  bitcount = BITCOUNT;
  temp = PageSize;
  Devaddr |= 1U << IDMASK ;
  while ( (temp / ( (uint16_t)1U << bitcount )) != 0U )
  {
   mask |= ( (uint16_t)1U << (bitcount - 1U) );
   bitcount++;
  }
  mem_addr &= mask;
  count = PageSize - (mem_addr % PageSize);
  if (NbByte <= count)
    if (pObj->IO.Address == 0xA8U)
      return pObj->IO.ReadReg( Devaddr, TarAddr, pData, NbByte);
    else
      return pObj->IO.ReadReg16( Devaddr, TarAddr, pData, NbByte);
  else
    if (pObj->IO.Address == 0xA8U)
      return pObj->IO.ReadReg(Devaddr, TarAddr,  pData, count);
    else
      return pObj->IO.ReadReg16(Devaddr, TarAddr,  pData, count);
}

/**
  * @brief  Permanentaly lock identification page of memory 
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t M24_i2c_LockID( M24_Object_t *pObj )
{  
  int32_t status = M24_OK;
  uint8_t  Devaddr = pObj->IO.Address | (1U << IDMASK);
  uint8_t lid_cmd = LOCKID;
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
  {
    return M24_TIMEOUT;
  }
  
  if (pObj->IO.Address == 0xA8U)
    status = pObj->IO.WriteReg(Devaddr, ADDRLID_16, &lid_cmd, 1 );
  else
    status = pObj->IO.WriteReg16(Devaddr, ADDRLID_16, &lid_cmd, 1 );
  
  while(  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) {};
  return status;
  
}

/**
  * @brief  Reads the lock status
  * @param  pObj : pointer to memory object
  * @param  pData : pointer of the data to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_LockStatus(M24_Object_t *pObj, uint8_t * pData )
{
  uint8_t dummybyte = 0xAA;
  uint8_t  Devaddr = pObj->IO.Address | (1U << IDMASK);
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
  {
    return M24_TIMEOUT;
  }
  if ( pObj->IO.Transmit(Devaddr, &dummybyte , 1 ) == M24_OK )
  {
    *pData = 0;
  }
  else 
  {
    *pData = 1;  
  } 
  while(  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK );  
  return M24_OK;
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

