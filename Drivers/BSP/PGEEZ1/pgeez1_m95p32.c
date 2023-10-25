/**
******************************************************************************
* @file    pgeez1_m95p32.c
* @author  SRA-SAIL
* @brief   This file provides X-NUCLEO-PGEEZ1 Board 
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
#include "pgeez1_m95p32.h"

/** @addtogroup BSP
* @{
*/

/* -------- Static functions prototypes --------------------------------------------- */
static M95P32_PGEEZ1_CommonDrv_t *M95P32Drv[PGEEZ1_M95P32_INSTANCES_NBR];
void *M95P32CompObj[PGEEZ1_M95P32_INSTANCES_NBR];
static int32_t M95P32_Probe(void);

/**
  * @brief  Initializes the SPI EEPROMs
  * @param  Instance : SPI EEPROMs instance to be used
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_Init(uint32_t Instance)
{
  int32_t ret = M95_OK;
  
  switch (Instance)
  {
  case PGEEZ1_M95P32_0:
    if (M95P32_Probe() != M95_OK)
    {
      return M95_ERROR;
    }
    break;  
  default:
    ret = M95_ERROR;
    break;
  }   
  
  return ret;  
}

/**
 * @brief  Function pointer to WRITE_ENABLE function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @retval BSP status
 */
int32_t PGEEZ1_M95P32_WriteEnable(uint32_t Instance)
{
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->WriteEnable(M95P32CompObj[Instance]));
  
  return ret; 
}

/**
 * @brief  Function pointer to WRITE_DISABLE function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @retval BSP status
 */
int32_t PGEEZ1_M95P32_WriteDisable(uint32_t Instance)
{
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->WriteDisable(M95P32CompObj[Instance]));
  
  return ret; 
}

/**
  * @brief  Function pointer to Read_StatusReg function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read          
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_StatusRegRead(uint32_t Instance, uint8_t * pData)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->StatusRegRead(M95P32CompObj[Instance], pData));
  
  return ret; 
  
}

/**
  * @brief  Function pointer to Write_StatusConfigReg function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to the data to write
  * @param  Size : number of bytes to write           
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_StatusConfigRegWrite(uint32_t Instance, uint8_t * pData, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->StatusConfigRegWrite(M95P32CompObj[Instance], pData, Size));
  
  return ret; 
}

/**
  * @brief  Function pointer to Single_Read function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  TarAddr : starting address to read
  * @param  Size : number of bytes to read    
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_ReadPage(uint32_t Instance, uint8_t * pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->ReadPage(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
}

/**
  * @brief  Function pointer to FAST_Read function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  TarAddr : starting address to read
  * @param  Size : number of bytes to read    
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_FastRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr,uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->FastRead(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
}

/**
  * @brief  Function pointer to FAST_DRead function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  TarAddr : starting address to read
  * @param  Size : number of bytes to read    
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_FastDRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr,uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->FastDRead(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
}

/**
  * @brief  Function pointer to FAST_QRead function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  TarAddr : starting address to read
  * @param  Size : number of bytes to read    
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_FastQRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr,uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->FastQRead(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
}

/**
  * @brief  Function pointer to Page_Prog_BUFF function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to the data to write
  * @param  TarAddr : starting address to write
  * @param  Size : number of bytes to write           
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_PageProgBuffer(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->PageProgBuffer(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
  
}

/**
  * @brief  Function pointer to Page_Write function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to the data to write
  * @param  TarAddr : starting address to write
  * @param  Size : number of bytes to write           
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_WritePage(uint32_t Instance, uint8_t * pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->WritePage(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret; 
}

/**
  * @brief  Function pointer to Page_Prog function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to the data to write
  * @param  TarAddr : starting address to write
  * @param  Size : number of bytes to write     
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_ProgramPage(uint32_t Instance, uint8_t * pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->ProgramPage(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret; 
}

/**
  * @brief  Function pointer to Page_Erase function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  TarAddr : starting address of page           
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_ErasePage(uint32_t Instance, uint32_t TarAddr)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->ErasePage(M95P32CompObj[Instance], TarAddr));
  
  return ret;
}

/**
  * @brief  Function pointer to Sector_Erase function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  TarAddr : starting address of sector
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_EraseSector(uint32_t Instance, uint32_t TarAddr)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->EraseSector(M95P32CompObj[Instance], TarAddr));
  
  return ret;
}

/**
  * @brief  Function pointer to Block_Erase function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  TarAddr : starting address of block
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_EraseBlock(uint32_t Instance, uint32_t TarAddr)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->EraseBlock(M95P32CompObj[Instance], TarAddr));
  
  return ret;
}

/**
  * @brief  Function pointer to Chip_Erase function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_EraseChip(uint32_t Instance)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->EraseChip(M95P32CompObj[Instance]));
  
  return ret;
}

/**
  * @brief  Function pointer to Read_ID function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  TarAddr : starting address to read
  * @param  Size : number of bytes to read    
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_ReadID(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->ReadID(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
  
}

/**
  * @brief  Function pointer to FAST_Read_ID function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  TarAddr : starting address to read
  * @param  Size : Number of bytes to read             
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_FastReadID(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->FastReadID(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
  
}

/**
  * @brief  Function pointer to Write_ID function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to the data to write
  * @param  TarAddr : starting address to write
  * @param  Size : number of bytes to write         
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_WriteID(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->WriteID(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
  
}

/**
 * @brief  Function pointer to Deep_Power_Down function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @retval BSP status
 */
int32_t PGEEZ1_M95P32_DeepPowerDown(uint32_t Instance)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->DeepPowerDown(M95P32CompObj[Instance]));
  
  return ret;
  
}

/**
 * @brief  Function pointer to Deep_Power_Down_Release function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @retval BSP status
 */
int32_t PGEEZ1_M95P32_DeepPowerDownRel(uint32_t Instance)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->DeepPowerDownRel(M95P32CompObj[Instance]));
  
  return ret;
  
}

/**
 * @brief  Function pointer to Read_JEDEC function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @param  pData : pointer to store data read
 * @param  Size : number of bytes to read    
 * @retval BSP : status
 */
int32_t PGEEZ1_M95P32_JEDECRead(uint32_t Instance, uint8_t *pData, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->JEDECRead(M95P32CompObj[Instance], pData, Size));
  
  return ret;
  
}

/**
  * @brief  Function pointer to ReadConfigReg function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  Size : number of bytes to read            
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_ConfSafetyRegRead(uint32_t Instance, uint8_t *pData, uint8_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->ConfSafetyRegRead(M95P32CompObj[Instance], pData, Size));
  
  return ret;
  
}

/**
 * @brief  Function pointer to ReadVolatileReg function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @param  pData : pointer to store data read
 * @retval BSP status
 */
int32_t PGEEZ1_M95P32_VolRegRead(uint32_t Instance, uint8_t *pData)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->VolRegRead(M95P32CompObj[Instance], pData));
  
  return ret;
}

/**
 * @brief  Function pointer to WriteVolatileRegister function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @param  regVal : register value to write            
 * @retval BSP status
 */
int32_t PGEEZ1_M95P32_VolRegWrite(uint32_t Instance, uint8_t regVal)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->VolRegWrite(M95P32CompObj[Instance], regVal));
  
  return ret;
}

/**
 * @brief  Function pointer to ClearSafetyFlag function in PGEEZ1 driver structure
 * @param  Instance : SPI EEPROMs instance to be used
 * @retval BSP status
 */
int32_t PGEEZ1_M95P32_ClearSafetyFlag(uint32_t Instance)
{
  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->ClearSafetyFlag(M95P32CompObj[Instance]));
  
  return ret;
  
}

/**
  * @brief  Function pointer to Read_StatusReg function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @param  pData : pointer to store data read
  * @param  TarAddr : starting address to read
  * @param  Size : number of bytes to read
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_SFDPRead(uint32_t Instance, uint8_t *pData, uint32_t TarAddr, uint32_t Size)
{ 
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->SFDPRegRead(M95P32CompObj[Instance], pData, TarAddr, Size));
  
  return ret;
  
}

/**
  * @brief  Function pointer to Reset_Enable function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_EnableReset(uint32_t Instance)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->EnableReset(M95P32CompObj[Instance]));
  
  return ret;
  
}

/**
  * @brief  Function pointer to Soft_Reset function in PGEEZ1 driver structure
  * @param  Instance : SPI EEPROMs instance to be used
  * @retval BSP status
  */
int32_t PGEEZ1_M95P32_SoftReset(uint32_t Instance)
{  
  int32_t ret = M95_OK;
  
  ret = (M95P32Drv[Instance]->SoftReset(M95P32CompObj[Instance]));
  
  return ret;
  
}

/**
  * @brief  Register Bus IOs for instance M95P32 if component ID is OK
  * @param  None
  * @retval BSP status
  */
static int32_t M95P32_Probe(void)
{
  M95_IO_t io_ctxm95p32;
  int32_t ret = M95_OK;
  static M95_Object_t M95P32_obj_0;
  
  io_ctxm95p32.Init                = SERIAL_COM_INIT;
  io_ctxm95p32.DeInit              = SERIAL_COM_DEINIT;
  io_ctxm95p32.Delay               = PGEEZ1_M95_Delay;
  
#ifdef USE_SPI
  io_ctxm95p32.Read                = SERIAL_COM_RECV;
  io_ctxm95p32.Write               = SERIAL_COM_SEND;
  io_ctxm95p32.SendRecv            = SERIAL_COM_SENDRECV;
#endif
  
  if (M95P32_RegisterBusIO(&M95P32_obj_0,&io_ctxm95p32) != M95_OK)
  {
    ret = M95_ERROR;
  }
  
  M95P32CompObj[PGEEZ1_M95P32_0] = &M95P32_obj_0;
  M95P32Drv[PGEEZ1_M95P32_0] = (M95P32_PGEEZ1_CommonDrv_t *)(void *)&M95P32_spi_Drv;
  
  if (M95P32Drv[PGEEZ1_M95P32_0]->Init(M95P32CompObj[PGEEZ1_M95P32_0]) != M95_OK)
  {
    ret = M95_ERROR;
  }
  else
  {
    ret = M95_OK;
  }  
  return ret;
}

#ifdef USE_QUADSPI

/**
  * @brief  Initialize QSPI
  * @retval status
  */
int32_t BSP_QUADSPI_Init(void)
{
  HAL_QSPI_MspInit(&QSPI_INSTANCE);
  return M95_OK;
}

/**
  * @brief  De-Initialize QSPI
  * @retval status
  */
int32_t BSP_QUADSPI_DeInit(void)
{
  HAL_QSPI_MspDeInit(&QSPI_INSTANCE);
  return M95_OK;
}

#endif
/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

