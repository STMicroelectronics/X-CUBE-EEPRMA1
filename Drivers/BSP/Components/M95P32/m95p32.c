/**
******************************************************************************
* @file    m95p32.c
* @author  SRA-SAIL
* @brief   file provides functionality of driver functions and to manage communication 
		   between BSP and M95P32 chip
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
#include "m95p32.h"
#include "string.h"


uint8_t VolatileRegVal;
#if defined (USE_QUADSPI)
extern QSPI_HandleTypeDef QSPI_INSTANCE;
#endif

#ifdef USE_SPI

uint8_t CmdBuff[SIZE610];
uint8_t RxCom[SIZE512]; 


/**
  * @brief  This function gives high on selected control pin
  * @param  None
  * @retval None
  */  
void EEPROMEX_CTRL_HIGH(void)
{
  HAL_GPIO_WritePin(M95P32_EEPROM_SPI_CS_PORT,M95P32_EEPROM_SPI_CS_PIN,GPIO_PIN_SET );
}

/**
  * @brief  This function gives low on selected control pin
  * @param  None
  * @retval None
  */  
void EEPROMEX_CTRL_LOW(void)
{
  HAL_GPIO_WritePin(M95P32_EEPROM_SPI_CS_PORT,M95P32_EEPROM_SPI_CS_PIN,GPIO_PIN_RESET );
}

/**
  * @brief  This function polls WIP bit of status register
  * @param  pObj : pointer to memory object
  * @retval None
  */ 
void Transmit_Data_polling(M95_Object_t *pObj)
{
  /* read status register until WIP bit become 0 */
  int32_t ret;
  CmdBuff[0] = CMD_READ_STATUS_REG;
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  if(ret != M95_OK)
  {
    Error_Handler();
  }
  RxCom[0] = 1;
  while ((RxCom[0] & 0x01U) != 0U)
  {
    pObj->IO.Read(RxCom, INSTRUCTION_LEN_1_BYTE);
    if(ret != M95_OK)
    {
      EEPROMEX_CTRL_HIGH();
      Error_Handler();
    }
  }
  EEPROMEX_CTRL_HIGH();
}
#endif


M95P32_Drv_t M95P32_spi_Drv =
{
  M95P32_spi_Init,
  WRITE_ENABLE,
  WRITE_DISABLE,
  Read_StatusReg,
  Single_Read,
  FAST_Read,
  FAST_DRead,
  FAST_QRead,
  Page_Write,
  Page_Prog,
  Page_Erase,
  Sector_Erase,
  Block_Erase,
  Chip_Erase,
  Read_ID,
  FAST_Read_ID,
  ReadVolatileReg,
  WriteVolatileRegister,
  Page_Prog_BUFF,
  ReadConfigReg,
  Write_StatusConfigReg,
  ClearSafetyFlag,
  Read_SFDP,
  Write_ID,
  Deep_Power_Down,
  Deep_Power_Down_Release,
  Read_JEDEC,
  Reset_Enable,
  Soft_Reset
    
};


int32_t M95P32_RegisterBusIO(M95_Object_t *pObj, M95_IO_t *pIO)
{
  pObj->IO.Init            = pIO->Init;
  pObj->IO.DeInit          = pIO->DeInit;
  pObj->IO.Read 	   = pIO->Read;
  pObj->IO.Write           = pIO->Write;
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
  * @retval M95 : status
  */
int32_t M95P32_spi_Init(M95_Object_t *pObj) 
{
  
  /* Configure the low level interface */
  if (pObj->IO.Init()<0)
  {
    return M95_ERROR;
  }
  return M95_OK;
}

/**
  * @brief  Set M95 eeprom De-initialization
  * @param  pObj : pointer to memory object
  * @retval M95 status
  */
int32_t M95P32_spi_DeInit( M95_Object_t *pObj )
{
  if (pObj->IO.DeInit()< 0)
  {
    return M95_ERROR;
  }
  /* Configure the low level interface */
  return M95_OK;
}

/**
  * @brief  Write enable sets the write enable latch (WEL) bit in the Status 
  *               register to a 1
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t WRITE_ENABLE(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_WriteEnable(&QSPI_INSTANCE);
  
  if( status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }
  
#elif defined (USE_SPI)
  EEPROMEX_CTRL_LOW();
  CmdBuff[0] = CMD_WREN;
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  Write disable resets the write enable latch (WEL) bit in the Status
  *               register to a 0
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t WRITE_DISABLE(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_WriteDisable(&QSPI_INSTANCE);
  if( status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }
  
#elif defined (USE_SPI)
  EEPROMEX_CTRL_LOW();
  CmdBuff[0] = CMD_WRDI;
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif  

  return ret;
}

/**
  * @brief  Read status register allow the 8-bit Status registers to be read
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @retval BSP status
  */
int32_t Read_StatusReg(M95_Object_t *pObj, uint8_t *pData)
{
  
  
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status =  QSPI_ReadSatusReg(&QSPI_INSTANCE, pData);
  if( status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_READ_STATUS_REG;
  
  EEPROMEX_CTRL_LOW();
  
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, INSTRUCTION_LEN_1_BYTE);
    EEPROMEX_CTRL_HIGH();
  }
  
  /* Clear command buffer */
  memset(&CmdBuff, 0, sizeof(CmdBuff));
  
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  Read data allows one or more data bytes to be sequentially read from
  *               the memory
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the read command
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
  
int32_t Single_Read(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Read(&QSPI_INSTANCE, pData, TarAddr, Size);
  
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_READ_DATA;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  
  EEPROMEX_CTRL_LOW();
  
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_4_BYTE);
  
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, Size);
    EEPROMEX_CTRL_HIGH();
  }
  memset(&CmdBuff, 0, sizeof(CmdBuff));
  
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  Fast_Read allows one or more data bytes to be sequentially read from
  *               the memory but addition of eight dummy clocks after the 24-bit
  *               address it can operate at the highest possible frequency
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the read command
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t FAST_Read(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_FAST_Read(&QSPI_INSTANCE, pData, TarAddr, Size);
  
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_FAST_READ_SINGLE;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  CmdBuff[4] = DUMMY_DATA;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_5_BYTE);
  
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, Size);
    EEPROMEX_CTRL_HIGH();
  }
  memset(&CmdBuff, 0, sizeof(CmdBuff));
  
#else
  /* Select SPI or QUADSPI interface */  
#endif

  return ret;
}

/**
  * @brief  Dual_Fast_Read allows one or more data bytes to be sequentially read 
  *               from the memory but addition of eight dummy clocks after the 
  *               24-bit address it can operate at the highest possible frequency. 
  *               The data is output on pins DQ0 and DQ1
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the read command
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t FAST_DRead(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  
  HAL_StatusTypeDef status = HAL_OK; 
  
  status = QSPI_DRead(&QSPI_INSTANCE, pData, TarAddr, Size );
  
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pData);
  UNUSED(TarAddr);
  UNUSED(Size);
#else
  /* Select SPI or QUADSPI interface */  
#endif

  return ret;
}

/**
  * @brief  Quad_Fast_Read allows one or more data bytes to be sequentially read 
  *               from the memory but addition of eight dummy clocks after the 
  *               24-bit address it can operate at the highest possible frequency. 
  *               The data is output on four pins (DQ0, DQ1, DQ2 and DQ3)
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the read command
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t FAST_QRead(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
  #if defined (USE_QUADSPI)
    
  HAL_StatusTypeDef status = HAL_OK;
  
  status = QSPI_QRead(&QSPI_INSTANCE, pData, TarAddr, Size);
  
  if( status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pData);
  UNUSED(TarAddr);
  UNUSED(Size);
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   Page write allows data to be written in a 
  *               single instruction (auto erase + program) leaving the other
  *               bytes of the page unchanged
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the write command
  * @param  Size : Number of Bytes of data to be written
  * @retval BSP status
  */
int32_t Page_Write(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t status = M95_OK;
  uint32_t remainingSize = Size;
  uint32_t targetAddress = TarAddr;
  uint8_t statusReg = 0;
  uint32_t bytesToWrite;
  
  /* Calculate the starting page and offset */
  uint32_t startOffset = TarAddr % M95P32_PAGESIZE;
  uint32_t offset = startOffset;
  
  
  /* Check for invalid inputs */
  if((pObj == NULL) || (pData == NULL) || (M95P32_PAGESIZE == 0U) || (remainingSize == 0U))
  {
    return M95_ERROR;
  }    
  
  /* Check WIP status bit*/
  do
  {
  Read_StatusReg(pObj, &statusReg);
  }while((statusReg & 0x01U) != 0U);
  
  /* Iterate over the pages and write the data */
  while(remainingSize > 0U) 
  {
    bytesToWrite = (remainingSize < (M95P32_PAGESIZE - offset)) ? remainingSize : (M95P32_PAGESIZE - offset);
    
    WRITE_ENABLE(pObj);
    
    #if defined (USE_QUADSPI)
    HAL_StatusTypeDef ret = HAL_OK;
    ret = QSPI_Write(&QSPI_INSTANCE, pData, targetAddress, bytesToWrite);
    if( ret != HAL_OK)
    {
      status = M95_ERROR;
    }
    else
    {
      status = M95_OK;
    }
      
    #elif defined (USE_SPI)
    CmdBuff[0] = CMD_WRITE_PAGE;
    CmdBuff[1] = (uint8_t)((targetAddress & MSK_BYTE3) >> SHIFT_16BIT);
    CmdBuff[2] = (uint8_t)((targetAddress & MSK_BYTE2) >> SHIFT_8BIT);
    CmdBuff[3] = (uint8_t)(targetAddress & MSK_BYTE1);
    for(uint32_t loop = 0; loop < bytesToWrite; loop++)
    {
      CmdBuff[loop + 4U] = pData [loop];
    }   
    
    EEPROMEX_CTRL_LOW();
    status = pObj->IO.Write(CmdBuff, bytesToWrite + INSTRUCTION_LEN_4_BYTE);
    EEPROMEX_CTRL_HIGH();
    
    #else
    /* Select SPI or QUADSPI interface */      
    #endif
    
    pObj->IO.Delay(5);
    if(status == M95_OK)
    {
      /* Update the pointers and sizes for the next page */
      pData += bytesToWrite;
      remainingSize -= bytesToWrite;
      targetAddress += bytesToWrite;
      offset = targetAddress % M95P32_PAGESIZE;
      
      /* Check WIP status bit*/
      do
      {
        Read_StatusReg(pObj, &statusReg);
      }while((statusReg & 0x01U) != 0U);
      
    }
    else
    {
      status = M95_ERROR;
      break;
    }
  }
  
  return status;
  
}

/**
  * @brief   The Page program allows data initially in the 
  *               erased state (FFh), to be written
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the write command
  * @param  Size : Number of Bytes of data to be written
  * @retval BSP status
  */
int32_t Page_Prog(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t status = M95_OK;
  uint32_t remainingSize = Size;
  uint32_t targetAddress = TarAddr;
  uint8_t statusReg = 0;
  uint32_t bytesToWrite;
  
  /* Calculate the starting page and offset */
  uint32_t startOffset = TarAddr % M95P32_PAGESIZE;
  uint32_t offset = startOffset;
  
  
  /* Check for invalid inputs */
  if((pObj == NULL) || (pData == NULL) || (M95P32_PAGESIZE == 0U) || (remainingSize == 0U))
  {
    return M95_ERROR;
  }    
  
  /* Check WIP status bit*/
  do
  {
  Read_StatusReg(pObj, &statusReg);
  }while((statusReg & 0x01U) != 0U);
  
  /* Iterate over the pages and write the data */
  while(remainingSize > 0U) 
  {
    bytesToWrite = (remainingSize < (M95P32_PAGESIZE - offset)) ? remainingSize : (M95P32_PAGESIZE - offset);
    
    WRITE_ENABLE(pObj);
    
    #if defined (USE_QUADSPI)
    HAL_StatusTypeDef ret = HAL_OK;
    
    ret = QSPI_Prog(&QSPI_INSTANCE, targetAddress, bytesToWrite, pData);
    if( ret != HAL_OK)
    {
      status = M95_ERROR;
    }
    else
    {
      status = M95_OK;
    }
      
    #elif defined (USE_SPI)
    CmdBuff[0] = CMD_PROG_PAGE;
    CmdBuff[1] = (uint8_t)((targetAddress & MSK_BYTE3) >> SHIFT_16BIT);
    CmdBuff[2] = (uint8_t)((targetAddress & MSK_BYTE2) >> SHIFT_8BIT);
    CmdBuff[3] = (uint8_t)(targetAddress & MSK_BYTE1);
    for(uint32_t loop = 0; loop < bytesToWrite; loop++)
    {
      CmdBuff[loop + 4U] = pData [loop];
    }   
    
    EEPROMEX_CTRL_LOW();
    status = pObj->IO.Write(CmdBuff, bytesToWrite + INSTRUCTION_LEN_4_BYTE);
    EEPROMEX_CTRL_HIGH();
    
    #else
    /* Select SPI or QUADSPI interface */      
    #endif
    
    pObj->IO.Delay(5);
    if(status == M95_OK)
    {
      /* Update the pointers and sizes for the next page */
      pData += bytesToWrite;
      remainingSize -= bytesToWrite;
      targetAddress += bytesToWrite;
      offset = targetAddress % M95P32_PAGESIZE;
      
      /* Check WIP status bit*/
      do
      {
        Read_StatusReg(pObj, &statusReg);
      }while((statusReg & 0x01U) != 0U);
      
    }
    else
    {
      status = M95_ERROR;
      break;
    }
  }
  
  return status;
}

/**
  * @brief   Page erase sets a page of 512 bytes within the device to the
  *               erased state of all 1s (FFh)
  * @param  pObj : pointer to memory object
  * @param  TarAddr : Starting address of the erase command
  * @retval BSP status
  */
int32_t Page_Erase(M95_Object_t *pObj, uint32_t TarAddr)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_PageErase(&QSPI_INSTANCE,TarAddr);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_ERASE_PAGE;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_4_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  Transmit_Data_polling(pObj);
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  Sector erase sets all memory bits within a specified sector 
  *               (4 Kbytes) to the erased state of all 1s(FFh)
  * @param  pObj : pointer to memory object
  * @param  TarAddr : Starting address of the erase command
  * @retval BSP status
  */
int32_t Sector_Erase(M95_Object_t *pObj, uint32_t TarAddr)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_SectorErase(&QSPI_INSTANCE,TarAddr);
  
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_ERASE_SECTOR;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_4_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  Transmit_Data_polling(pObj);
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  Block erase sets all memory bits within a specified block 
  *               (64 Kbytes) to the erased state of all 1s(FFh)
  * @param  pObj : pointer to memory object
  * @param  TarAddr : Starting address of the erase command
  * @retval BSP status
  */
int32_t Block_Erase(M95_Object_t *pObj, uint32_t TarAddr)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_BlockErase(&QSPI_INSTANCE,TarAddr);
  
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_ERASE_BLOCK;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_4_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  Transmit_Data_polling(pObj);
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}
  
/**
  * @brief  Chip erase sets all memory bits within the device to the erased 
  *               state of all 1s(FFh)
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t Chip_Erase(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_ChipErase(&QSPI_INSTANCE);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  CmdBuff[0] = CMD_ERASE_CHIP;
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  Transmit_Data_polling(pObj);
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   Read identification allows one or more data bytes in the two 
  *               identification pages (512 bytes each) to be sequentially read
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the read command
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t Read_ID(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Read_ID(&QSPI_INSTANCE, TarAddr, Size, pData);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_READ_ID_PAGE;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_4_BYTE);
  
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, Size);
    EEPROMEX_CTRL_HIGH();
  }
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   Fast read identification allows one or more data bytes in the two 
  *               identification pages to be sequentially read but by addition 
  *               of eight dummy clocks after the 24-bit address it can operate
  *               at the highest possible frequency
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the read command
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t FAST_Read_ID(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_FAST_Read_ID(&QSPI_INSTANCE,TarAddr,Size,pData);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_FAST_READ_ID_PAGE;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  CmdBuff[4] = DUMMY_DATA;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_5_BYTE);
  
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, Size);
    EEPROMEX_CTRL_HIGH();
  }
  memset(&CmdBuff, 0, sizeof(CmdBuff));
  
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   The Read Volatile register allow the 8-bit Volatile register to be
  *               read
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @retval BSP status
  */
int32_t ReadVolatileReg(M95_Object_t *pObj, uint8_t *pData)
{ 
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status =  QSPI_ReadVolatileReg(&QSPI_INSTANCE, pData);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }
#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_READ_VOLATILE_REG;
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, INSTRUCTION_LEN_1_BYTE);
    EEPROMEX_CTRL_HIGH();
  }
  memset(&CmdBuff, 0, sizeof(CmdBuff));
  
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  
  return ret;
}

/**
  * @brief   The Write volatile register allows the volatile register to be
  *               written. The writable volatile register bits include BUFEN 
  *               and BUFLD
  * @param  pObj : pointer to memory object
  * @param  regVal : Value to be written in volatile register
  * @retval BSP status
  */
int32_t WriteVolatileRegister(M95_Object_t *pObj, uint8_t regVal)
{ 
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_WriteVolatileRegister(&QSPI_INSTANCE,regVal);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_WRITE_VOLATILE_REG;
  CmdBuff[1] = regVal;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_2_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  Transmit_Data_polling(pObj);
  memset(&CmdBuff, 0, sizeof(CmdBuff));

#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  
  return ret;
}

/**
  * @brief   The Page program with buffer load allows one to 512 bytes data 
  *               initially in the erased state (FFh) to be written and allows,
  *               during a page program execution to load the buffer of 512 data
  *               bytes for the next page program operation
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the write command
  * @param  Size : Number of Bytes of data to be written
  * @retval BSP status
  */
int32_t Page_Prog_BUFF(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;  
  
#if defined (USE_QUADSPI)
  
  uint32_t start_add = TarAddr ,temp;
  uint32_t end_add = TarAddr + Size - 0x01U;
  
  HAL_StatusTypeDef status = HAL_OK;
  
  if((TarAddr % 0x200U) != 0U)
  { 
    temp = TarAddr %(0x200U);
    status = QSPI_Prog(&QSPI_INSTANCE,TarAddr,(0x200U-temp),pData);
    if(status != HAL_OK)
    {
      ret =  M95_ERROR;
      return ret;
    }
    start_add = TarAddr + 0x200U - temp;
  }
  
  if(((end_add % (0x1FFU))- (end_add/0x200U)) != 0U)
  {	
    temp = (end_add % 0x1FFU)- (end_add/0x200U);
    end_add = end_add - temp;
    status = QSPI_Prog(&QSPI_INSTANCE, end_add, temp + 1U, pData);
    if(status != HAL_OK)
    {
      ret =  M95_ERROR;
      return ret;
    }
  }
  
  while( start_add < end_add )
  {
    ReadVolatileReg(pObj, &VolatileRegVal);
    
    if( VolatileRegVal == 02U)
    {
      status = QSPI_Prog(&QSPI_INSTANCE,start_add,0x200,pData);
      if(status != HAL_OK)
      {
        ret = M95_ERROR;
        return ret;
      }
      start_add = start_add + 0x200U;
    }
  }
  

#elif defined (USE_SPI)
  
  uint32_t start_add = TarAddr ,temp;
  uint32_t end_add = TarAddr + Size - 0x01U;
  
  int32_t status = M95_OK;
  
  if((TarAddr % 0x200U) != 0U)
  { 
    temp = TarAddr %(0x200U);
    status = Page_Prog(pObj, pData, TarAddr, (0x200U-temp));
    if(status != M95_OK)
    {
      ret = M95_ERROR;
      return ret;
    }
    start_add = TarAddr + 0x200U - temp;
  }
  
  if(((end_add % (0x1FFU)) - (end_add/0x200U)) != 0U)
  {	
    temp = (end_add % (0x1FFU)) - (end_add/0x200U);
    end_add = end_add - temp;
    status = Page_Prog(pObj, pData, end_add, temp+1U);
    if(status != M95_OK)
    {
      ret = M95_ERROR;
      return ret;
    }
  }
  
  while( start_add < end_add )
  {
    ReadVolatileReg(pObj, &VolatileRegVal);
    if( VolatileRegVal == 02U)
    {
      status = Page_Prog(pObj, pData, start_add, 0x200U);
      if(status != M95_OK)
      {
        ret = M95_ERROR;
        return ret;
      }
      start_add = start_add + 0x200U;
    }
  }
  
#else
  /* Select SPI or QUADSPI interface */  
#endif
  return ret;
}

/**
  * @brief   The RDCR reads the two bytes of Configuration and Safety registers 
  *               (one for each register)
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  Size : Number of Bytes of data to be written
  * @retval BSP status
  */
int32_t ReadConfigReg(M95_Object_t *pObj, uint8_t *pData, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_ReadConfigReg(&QSPI_INSTANCE, (uint8_t)Size, pData);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_READ_CONF_SAFETY_REG;
  EEPROMEX_CTRL_LOW();
  
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, Size);
    EEPROMEX_CTRL_HIGH();
  }
  
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  Write status register allows the status and configuration register 
  *               to be written
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t Write_StatusConfigReg(M95_Object_t *pObj, uint8_t *pData, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_WriteStatusConfigRegister(&QSPI_INSTANCE, pData, Size);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  uint32_t loop = 0;
  
  CmdBuff[0] = CMD_WRITE_STATUS_CONF_REG;
  
  for (loop=0; loop < Size; loop++)
  {
    CmdBuff[loop + 1U] = pData [loop];
  }
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, (Size + INSTRUCTION_LEN_1_BYTE));
  EEPROMEX_CTRL_HIGH();
  
  Transmit_Data_polling(pObj);
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  The Clear Safety register resets all the bits of the Safety register
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t ClearSafetyFlag(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_ClearSafetyFlag(&QSPI_INSTANCE);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_CLEAR_SAFETY_REG;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   The Read SFDP allows the SFDP register format to be read
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the write command
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t Read_SFDP(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Read_SFDP(&QSPI_INSTANCE, TarAddr, Size, pData);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_READ_SFDP;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  CmdBuff[4] = DUMMY_DATA;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_5_BYTE);
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, Size);
    EEPROMEX_CTRL_HIGH();
  }
  
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   Write identification page instruction (WRID) allows the 
  *               identification page to be written
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  TarAddr : Starting address of the read command
  * @param  Size : Number of Bytes of data to be written
  * @retval BSP status
  */
int32_t Write_ID(M95_Object_t *pObj, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Write_ID(&QSPI_INSTANCE, TarAddr, Size, pData);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  uint32_t loop = 0;
  
  CmdBuff[0] = CMD_WRITE_ID_PAGE;
  CmdBuff[1] = (uint8_t)((TarAddr & MSK_BYTE3) >> SHIFT_16BIT);
  CmdBuff[2] = (uint8_t)((TarAddr & MSK_BYTE2) >> SHIFT_8BIT);
  CmdBuff[3] = (uint8_t)(TarAddr & MSK_BYTE1);
  
  for (loop = 0; loop < Size; loop++)
  {
    CmdBuff[loop + 4U] = pData [loop];
  }
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, (Size + INSTRUCTION_LEN_4_BYTE));
  EEPROMEX_CTRL_HIGH();
  
  Transmit_Data_polling(pObj);
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  The deep power-down enter allows to put the device in a very low 
  *         consumption state in which a limited number of commands are 
  *         available
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t Deep_Power_Down(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Deep_Power_Down(&QSPI_INSTANCE);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_DEEP_POWER_DOWN;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief  The deep power-down release allows to release the device from the 
  *         deep power-down state to a standby-mode state
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t Deep_Power_Down_Release(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Deep_Power_Down_Release(&QSPI_INSTANCE);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_DEEP_POWER_DOWN_RELEASE;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   The JEDEC identification allows to read in loop mode the three 
  *               device identification bytes in loop mode
  * @param  pObj : pointer to memory object
  * @param  pData : pointer to data buffer
  * @param  Size : Number of Bytes of data to be read
  * @retval BSP status
  */
int32_t Read_JEDEC(M95_Object_t *pObj, uint8_t *pData, uint32_t Size)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Read_JEDEC(&QSPI_INSTANCE, (uint8_t)Size, pData);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_READ_JEDEC;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  if(ret != M95_OK)
  {
    EEPROMEX_CTRL_HIGH();
  }
  else
  {
    ret = pObj->IO.Read(pData, Size);
    EEPROMEX_CTRL_HIGH();
  }
  
  memset(&CmdBuff, 0, sizeof(CmdBuff));
  
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   The enable reset initiate the reset the device
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t Reset_Enable(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Reset_Enable(&QSPI_INSTANCE);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_ENABLE_RESET;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}

/**
  * @brief   The Software reset initiate the reset the device
  * @param  pObj : pointer to memory object
  * @retval BSP status
  */
int32_t Soft_Reset(M95_Object_t *pObj)
{
  int32_t ret = M95_OK;
  
#if defined (USE_QUADSPI)
  HAL_StatusTypeDef status = HAL_OK;
  status = QSPI_Soft_Reset(&QSPI_INSTANCE);
  if(status != HAL_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }

#elif defined (USE_SPI)
  
  CmdBuff[0] = CMD_SOFT_RESET;
  
  EEPROMEX_CTRL_LOW();
  ret = pObj->IO.Write(CmdBuff, INSTRUCTION_LEN_1_BYTE);
  EEPROMEX_CTRL_HIGH();
  
  memset(&CmdBuff, 0, sizeof(CmdBuff));
#else
  /* Select SPI or QUADSPI interface */  
#endif
  
  return ret;
}


#ifdef USE_QUADSPI

/** @addtogroup BSP
* @{
*/

/** @addtogroup Components
* @{
*/ 

/** @defgroup M95
* @brief This file provides set of driver functions to manage communication
*        between BSP and M95P32 chip.
* @{
*/

/* -------- Static functions prototypes --------------------------------------------- */

/**
  * @brief  Write enable sets the write enable latch (WEL) bit in the Status 
  *               register to a 1
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  */
HAL_StatusTypeDef QSPI_WriteEnable(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_WREN;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  return status;
  
}

/**
  * @brief  Write disable resets the write enable latch (WEL) bit in the Status
  *               register to a 0
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_WriteDisable(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_WRDI;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  return status;
}

/**
  * @brief  Read status register allow the 8-bit Status registers to be read
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_ReadSatusReg(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_READ_STATUS_REG;
  sCommand.Address		= 0x00;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles          = 0;
  sCommand.InstructionMode      = QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode          = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode    = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode             = QSPI_DATA_1_LINE;
  sCommand.NbData      		= 1;
  sCommand.DdrMode              = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle     = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode             = QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  if(status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Write status register allows the status and configuration register 
  *               to be written
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  * @param  nb_data Number of Bytes of data to be read
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_WriteStatusConfigRegister(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData, uint32_t nb_data)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_WRITE_STATUS_CONF_REG;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.NbData      	     = nb_data;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if(status != HAL_OK)
  { 
    return status;
  }
  else
  {
    status  = HAL_QSPI_Transmit(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  if(status != HAL_OK)
  {
    return status;
  }
  else
  { 
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    
    status =  HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
    
  }
  
  return status;
}

/**
  * @brief  Read data allows one or more data bytes to be sequentially read from
  *               the memory
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  * @param  pData pointer to data buffer
  * @param  TarAddr Starting address of the read command
  * @param  Size Number of Bytes of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Read(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_READ_DATA;
  sCommand.Address		= TarAddr;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= Size;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if(status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  return status;
}

/**
  * @brief  Fast_Read allows one or more data bytes to be sequentially read from
  *               the memory but addition of eight dummy clocks after the 24-bit
  *               address it can operate at the highest possible frequency
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  * @param  pData pointer to data buffer
  * @param  TarAddr Starting address of the read command
  * @param  Size Number of Bytes of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_FAST_Read(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_FAST_READ_SINGLE;
  sCommand.Address		= TarAddr;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 8;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= Size;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if(status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  return status;
}

/**
  * @brief  Dual_Fast_Read allows one or more data bytes to be sequentially read 
  *               from the memory but addition of eight dummy clocks after the 
  *               24-bit address it can operate at the highest possible frequency
  *               The data is output on pins DQ0 and DQ1
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  * @param  TarAddr Starting address of the read command
  * @param  Size Number of Bytes of data to be read
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_DRead(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_FAST_READ_DUAL;
  sCommand.Address		= TarAddr;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 8;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_2_LINES;
  sCommand.NbData      		= Size;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if(status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  return status;
}

/**
  * @brief  Quad_Fast_Read allows one or more data bytes to be sequentially read 
  *               from the memory but addition of eight dummy clocks after the 
  *               24-bit address it can operate at the highest possible frequency.
  *               The data is output on four pins (DQ0, DQ1, DQ2 and DQ3)
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  * @param  TarAddr Starting address of the read command
  * @param  Size Number of Bytes of data to be read
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_QRead(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_FAST_READ_QUAD;
  sCommand.Address		= TarAddr;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 8;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_4_LINES;
  sCommand.NbData      		= Size;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if(status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  return status;
}

/**
  * @brief  Page write allows one to 512 bytes of data to be written in a 
  *         single instruction (auto erase + program) leaving the other
  *         bytes of the page unchanged
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  pData pointer to data buffer
  * @param  TarAddr Starting address of the write command
  * @param  Size Number of Bytes of data to be written
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Write(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_WRITE_PAGE;
  sCommand.Address		= TarAddr;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= Size;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if( status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Transmit(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  if( status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    sCommand.AddressMode    = QSPI_ADDRESS_NONE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  return status;
}

/**
  * @brief  The Page program allows one to 512 bytes of data initially in the 
  *         erased state (FFh), to be written
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  add Starting address of the write command
  * @param  nb_data Number of Bytes of data to be written
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Prog(QSPI_HandleTypeDef *local_hqspi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------  */
  sCommand.Instruction 		= CMD_PROG_PAGE;
  sCommand.Address		= add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= nb_data;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Transmit(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  if( status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    sCommand.AddressMode    = QSPI_ADDRESS_NONE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Page erase sets a page of 512 bytes within the device to the
  *         erased state of all 1s (FFh)
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  Add Starting address of the erase command
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_PageErase(QSPI_HandleTypeDef *local_hqspi,uint32_t Add)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_ERASE_PAGE;
  sCommand.Address		= Add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_NONE;
  sCommand.NbData      		= 0;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    sCommand.AddressMode    = QSPI_ADDRESS_NONE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Sector erase sets all memory bits within a specified sector 
  *         (4 Kbytes) to the erased state of all 1s(FFh)
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  Add Starting address of the erase command
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_SectorErase(QSPI_HandleTypeDef *local_hqspi,uint32_t Add)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_ERASE_SECTOR;
  sCommand.Address		= Add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_NONE;
  sCommand.NbData      		= 0;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Block erase sets all memory bits within a specified block 
  *         (64 Kbytes) to the erased state of all 1s(FFh)
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  Add Starting address of the erase command
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_BlockErase(QSPI_HandleTypeDef *local_hqspi,uint32_t Add)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_ERASE_BLOCK;
  sCommand.Address		= Add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_NONE;
  sCommand.NbData      		= 0;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Chip erase sets all memory bits within the device to the erased 
  *         state of all 1s(FFh)
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  Add Starting address of the erase command
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_ChipErase(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;
  HAL_StatusTypeDef       status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_ERASE_CHIP;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Read identification allows one or more data bytes in the two 
  *         identification pages (512 bytes each) to be sequentially read
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  add Starting address of the read command
  * @param  nb_data Number of Bytes of data to be read
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Read_ID(QSPI_HandleTypeDef *local_hqspi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  QSPI_CommandTypeDef     sCommand;
  HAL_StatusTypeDef       status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_READ_ID_PAGE;
  sCommand.Address		= add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= nb_data;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Fast read identification allows one or more data bytes in the two 
  *         identification pages to be sequentially read but by addition 
  *         of eight dummy clocks after the 24-bit address it can operate
  *         at the highest possible frequency
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  add Starting address of the read command
  * @param  nb_data Number of Bytes of data to be read
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_FAST_Read_ID(QSPI_HandleTypeDef *local_hqspi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_FAST_READ_ID_PAGE;
  sCommand.Address		= add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 8;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= nb_data;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  Write identification page instruction (WRID) allows the 
  *         identification page to be written
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  add Starting address of the read command
  * @param  nb_data Number of Bytes of data to be written
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Write_ID(QSPI_HandleTypeDef *local_hqspi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_WRITE_ID_PAGE;
  sCommand.Address		= add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= nb_data;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Transmit(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  The deep power-down enter allows to put the device in a very low 
  *         consumption state in which a limited number of commands are 
  *         available
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Deep_Power_Down(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_DEEP_POWER_DOWN;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  return status;
}

/**
  * @brief   The deep power-down release allows to release the device from the 
  *               deep power-down state to a standby-mode state
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module
  */
HAL_StatusTypeDef QSPI_Deep_Power_Down_Release(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_DEEP_POWER_DOWN_RELEASE;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  return status;
}

/**
  * @brief  The JEDEC identification allows to read in loop mode the three 
  *         device identification bytes in loop mode
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  nb_data Number of Bytes of data to be read
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Read_JEDEC(QSPI_HandleTypeDef *local_hqspi,uint8_t nb_data,uint8_t *pData)
{
  
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_READ_JEDEC;
  sCommand.Address		= 0x00;
  sCommand.AddressSize      	= 0;
  sCommand.DummyCycles       	= 0;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= nb_data;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  The RDCR reads the two bytes of Configuration and Safety registers 
  *         (one for each register)
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  nb_reg Number of Bytes of data to be written
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_ReadConfigReg(QSPI_HandleTypeDef *local_hqspi,uint8_t nb_reg, uint8_t *pData)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_READ_CONF_SAFETY_REG;
  sCommand.Address		= 0x00;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles          = 0;
  sCommand.InstructionMode      = QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode          = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode    = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode             = QSPI_DATA_1_LINE;
  sCommand.NbData      		= nb_reg;
  sCommand.DdrMode              = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle     = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode             = QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  The Read Volatile register allow the 8-bit Volatile register to be
  *         read
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_ReadVolatileReg(QSPI_HandleTypeDef *local_hqspi, uint8_t *pData )
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_READ_VOLATILE_REG;
  sCommand.Address		= 0x00;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles          = 0;
  sCommand.InstructionMode      = QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode          = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode    = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode             = QSPI_DATA_1_LINE;
  sCommand.NbData      		= 1;
  sCommand.DdrMode              = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle     = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode             = QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  return status;
}

/**
  * @brief  The Write volatile register allows the volatile register to be
  *         written. The writable volatile register bits include BUFEN 
  *         and BUFLD
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  regVal Value to be written in volatile register
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_WriteVolatileRegister(QSPI_HandleTypeDef *local_hqspi,uint8_t regVal)
{
  QSPI_CommandTypeDef           sCommand;
  QSPI_AutoPollingTypeDef       sConfig;
  HAL_StatusTypeDef             status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_WRITE_VOLATILE_REG;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.NbData      	     = 1;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  if ( status != HAL_OK)
  { 
    return status;
  }
  else
  {
    status = HAL_QSPI_Transmit(&QSPI_INSTANCE, &regVal, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  
  if ( status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match           = 0x00;
    sConfig.Mask            = 0x01;
    sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval        = 0x10;
    sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    
    sCommand.Instruction    = CMD_READ_STATUS_REG;
    sCommand.DataMode       = QSPI_DATA_1_LINE;
    
    status = HAL_QSPI_AutoPolling(&QSPI_INSTANCE, &sCommand, &sConfig, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  The Clear Safety register resets all the bits of the Safety register
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_ClearSafetyFlag(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_CLEAR_SAFETY_REG;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  return status;
}

/**
  * @brief  The Read SFDP allows the SFDP register format to be read.
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @param  add Starting address of the write command
  * @param  nb_data Number of Bytes of data to be read
  * @param  pData pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Read_SFDP(QSPI_HandleTypeDef *local_hqspi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.Instruction 		= CMD_READ_SFDP;
  sCommand.Address		= add;
  sCommand.AddressSize      	= QSPI_ADDRESS_24_BITS;
  sCommand.DummyCycles       	= 8;
  sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressMode       	= QSPI_ADDRESS_1_LINE;
  sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          	= QSPI_DATA_1_LINE;
  sCommand.NbData      		= nb_data;
  sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         	= QSPI_SIOO_INST_EVERY_CMD;
  
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  if ( status != HAL_OK)
  {
    return status;
  }
  else
  {
    status = HAL_QSPI_Receive(&QSPI_INSTANCE, pData, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  }
  return status;
}

/**
  * @brief  The enable reset initiate the reset the device
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Reset_Enable(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_ENABLE_RESET;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  return status;
}

/**
  * @brief  The Software reset initiate the reset the device
  * @param  local_hqspi pointer to a QSPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module
  * @retval HAL status
  */
HAL_StatusTypeDef QSPI_Soft_Reset(QSPI_HandleTypeDef *local_hqspi)
{
  QSPI_CommandTypeDef   sCommand;
  HAL_StatusTypeDef     status;
  
  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = CMD_SOFT_RESET;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  
  status = HAL_QSPI_Command(&QSPI_INSTANCE, &sCommand, M95P32_QPSI_TIMEOUT_DEFAULT_VALUE);
  
  return status;
}
#endif
