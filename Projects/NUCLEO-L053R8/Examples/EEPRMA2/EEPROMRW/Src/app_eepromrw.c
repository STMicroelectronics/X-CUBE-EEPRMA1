/**
  ******************************************************************************
  * @file           : app_eepromrw.c
  * @brief          : This file provides code for the configuration
  *       of the STMicroelectronics.X-CUBE-EEPRMA1.5.1.0 instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "app_eepromrw.h"
#include "stm32l0xx_nucleo.h"
#include "eeprma2_m24.h"
#include "eeprma2_m95.h"
#include "eeprma2_conf.h"

/** @defgroup EEPROM_NUCLEO
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/*Source address of one node should be destination for other node & vice-versa*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define COUNTOF(a)      (sizeof(a) / sizeof(*(a)))

/*size of data to be read/write */
#define SIZE10     (COUNTOF(tx10)-1)
#define SIZE16     (COUNTOF(tx16)-1)
#define SIZE43     (COUNTOF(tx43)-1)
#define SIZE128    (COUNTOF(tx128)-1)
#define SIZE256    (COUNTOF(tx256)-1)
#define SIZE260    (COUNTOF(tx260)-1)
#define SIZE495    (COUNTOF(tx495)-1)
#define SIZE512    (COUNTOF(tx512)-1)
#define SIZE20     (COUNTOF(tx20)-1)
#define BIT0        0x01U

/*Sample data for R/W */
uint8_t tx10[] = "STM32F4/L4";
uint8_t tx20[] = "NUCLEO-L0/F4/G4 ODE ";
uint8_t tx16[] = "HelloSTM32 12345";
uint8_t tx43[] = "HelloSTM32 12345678911 HelloSTM32 12345678911";
uint8_t tx128[] = "At power-up, the device does not respond to any instruction until VCC has reached the "
                  "internal reset threshold voltage.Thank you";
uint8_t tx256[] = "abcdefghE-EEPROM-Expansion Firmware library EEPROM driver example : This firmware provides "
                  "a basic example of how to use the X-Nucleo-eXpansion firmware library. This block of data is "
                  "specially written to test the data write function of EEPROM (SPI/I2C)  ";
uint8_t tx260[] = "abcdefghE-EEPROM-Expansion Firmware library EEPROM driver example : This firmware provides"
                  "a basic example of how to use the X-Nucleo-eXpansion firmware library. This block of data "
                  "is specially written to test the data write function of EEPROM (SPI/I2C)  IC   ";
uint8_t tx495[] = "abcdefghE-EEPROM- Expansion Firmware library EEPROM driver example : This firmware provides "
                  "a basic example of how to use the X-Nucleo-eXpansion firmware library. This block of data "
                  "is specially written to test the data write function of EEPROM (SPI/I2C)   "
                  "abcdefghE-EEPROM-Expansion Firmware library EEPROM driver example : This firmware provides "
                  "a basic example of how to use the X-Nucleo-eXpansion firmware library. "
                  "This block of data is specially written to test the data write function of E";
uint8_t tx512[] = "abcdefghE-EEPROM- Expansion Firmware library EEPROM driver example : This firmware provides "
                  "a basic example of how to use the X-Nucleo-eXpansion firmware library. "
                  "This block of data is specially written to test the data write function of EEPROM (SPI/I2C)   "
                  "abcdefghE-EEPROM-Expansion Firmware library EEPROM driver example : This firmware provides "
                  "a basic example of how to use the X-Nucleo-eXpansion firmware library. "
                  "This block of data is specially written to test the data write function of EEPROM SPI/I2C IC";

/*Rx buffer*/
uint8_t rx10[SIZE10];
uint8_t rx16[SIZE16];
uint8_t rx43[SIZE43];
uint8_t rx128[SIZE128];
uint8_t rx256[SIZE256];
uint8_t rx260[SIZE260];
uint8_t rx495[SIZE495];
uint8_t rx512[SIZE512];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern void SystemClock_Config(void);
void M24_TestSingleByte(unsigned int TargetAddress, int32_t TargetName, int32_t expWriteStatus, int32_t expReadStatus);
void M95_TestSingleByte(unsigned int TargetAddress, uint32_t TargetName, int32_t expWriteStatus,
                        int32_t expReadStatus);
void M24_TestData(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t size,
                  int32_t expWriteStatus, int32_t expReadStatus);
void M95_TestData(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t Size,
                  int32_t expWriteStatus, int32_t expReadStatus);
void M24_TestPage(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t size,
                  int32_t expWriteStatus, int32_t expReadStatus);
void M95_TestPage(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t size,
                  int32_t expWriteStatus, int32_t expReadStatus);
void TestData_WriteProtect(unsigned int TargetAddress, uint8_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t Size,
                           int32_t expWriteStatus, int32_t expReadStatus);
int32_t WriteEnableReadStatus(uint32_t TargetName);
int32_t WriteDisableReadStatus(uint32_t TargetName);
int32_t TestData_BlockProtect(uint8_t BlockProtect, unsigned int TargetAddress, uint8_t TargetName, uint8_t tx[],
                              uint8_t rx[], uint16_t Size);
void MX_EEPROM_RW_Init(void);
void MX_EEPROM_RW_Process(void);
int32_t M95_ReadID(uint32_t TargetName, const unsigned int TargetAddress);
int32_t M95_LockStatus(uint32_t TargetName);
int32_t M95_WriteID(uint32_t TargetName, const uint32_t TargetAddress);
int32_t M95_LockID(uint32_t TargetName);

/* Exported functions ------------------------------------------------------- */

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/**
  * @brief  Initialize the EEPROM example
  * @param  None
  * @retval None
  */
void MX_EEPROM_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN M24_M95_Library_Init_PreTreatment */

  /* USER CODE END M24_M95_Library_Init_PreTreatment */

  /* Initialize the peripherals and the M24 and M95 components */

  MX_EEPROM_RW_Init();

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN M24_M95_Library_Init_PostTreatment */

  /* USER CODE END M24_M95_Library_Init_PostTreatment */

}

/**
  * @brief  EEPROM process
  * @param  None
  * @retval None
  */
void MX_EEPROM_Process(void)
{
  /* USER CODE BEGIN M24_M95_Library_Process */
  MX_EEPROM_RW_Process();

  /* USER CODE END M24_M95_Library_Process */
}

/**
  * @brief  Initialize the EEPROM Read Write Example
  * @retval None
  */

void MX_EEPROM_RW_Init(void)
{

  EEPRMA2_GPIO_Init();
  /* Init UART for display message on console */
  BSP_COM_Init(COM1);
  while (EEPRMA2_M24_Init(EEPRMA2_M24C02_0) != BSP_ERROR_NONE);
  while (EEPRMA2_M24_Init(EEPRMA2_M24256_0) != BSP_ERROR_NONE);
  while (EEPRMA2_M24_Init(EEPRMA2_M24M01_0) != BSP_ERROR_NONE);
  while (EEPRMA2_M95_Init(EEPRMA2_M95M04_0) != BSP_ERROR_NONE);
  while (EEPRMA2_M95_Init(EEPRMA2_M95256_0) != BSP_ERROR_NONE);
  while (EEPRMA2_M95_Init(EEPRMA2_M95040_0) != BSP_ERROR_NONE);

  /* Write Enable, Write Disable and Read Status only for SPI EEPROM  */
  printf(" \r\n ");
  printf("--SPI EEPROM READ STATUS-- \r\n");
  WriteDisableReadStatus(EEPRMA2_M95256_0);
  HAL_Delay(5);
  WriteEnableReadStatus(EEPRMA2_M95256_0);
  HAL_Delay(5);
  WriteDisableReadStatus(EEPRMA2_M95040_0);
  HAL_Delay(5);
  WriteEnableReadStatus(EEPRMA2_M95040_0);
  HAL_Delay(5);
  WriteDisableReadStatus(EEPRMA2_M95M04_0);
  HAL_Delay(5);
  WriteEnableReadStatus(EEPRMA2_M95M04_0);

  /* Writing Single Byte in I2C EEPROM */
  printf(" \r\n ");
  printf("-- I2C/SPI EEPROM SINGLE BYTE-- \r\n");
  M24_TestSingleByte(0x00, EEPRMA2_M24C02_0, BSP_ERROR_NONE, BSP_ERROR_NONE);
  M24_TestSingleByte(0xC8, EEPRMA2_M24256_0, BSP_ERROR_NONE, BSP_ERROR_NONE);
  M24_TestSingleByte(0x05, EEPRMA2_M24M01_0, BSP_ERROR_NONE, BSP_ERROR_NONE);
  M95_TestSingleByte(0x01, EEPRMA2_M95040_0, BSP_ERROR_NONE, BSP_ERROR_NONE);
  M95_TestSingleByte(0x01, EEPRMA2_M95M04_0, BSP_ERROR_NONE, BSP_ERROR_NONE);
  /*EEPROM I2C : Multiple Bytes read/write*/
  printf(" \r\n ");
  printf("-- I2C/SPI EEPROM DATA -- \r\n");
  /* Write Data into I2C EEPROM memory from begin to end and then read */
  M24_TestData(0x00, EEPRMA2_M24C02_0, tx256, rx256, SIZE256, BSP_ERROR_NONE, BSP_ERROR_NONE);
  M95_TestData(0x00, EEPRMA2_M95040_0, tx256, rx256, SIZE256, BSP_ERROR_NONE, BSP_ERROR_NONE);

  /*EEPROM I2C : Page read/write*/
  printf(" \r\n ");
  printf("--I2C EEPROM PAGE-- \r\n");
  M24_TestPage(0x00, EEPRMA2_M24C02_0, tx16, rx16, SIZE16, BSP_ERROR_NONE, BSP_ERROR_NONE);

  /*EEPROM SPI : Page read/write*/
  printf(" \r\n ");
  printf("--SPI EEPROM PAGE-- \r\n");
  M95_TestPage(0x00, EEPRMA2_M95M04_0, tx512, rx512, SIZE512, BSP_ERROR_NONE, BSP_ERROR_NONE);

  /* Read Page ID */
  printf(" \n\n\r ");
  printf("--SPI M95M04 EEPROM READ ID PAGE-- \r\n");
  M95_ReadID(EEPRMA2_M95M04_0, 0x00);
  M95_WriteID(EEPRMA2_M95M04_0, 0x00);
  M95_ReadID(EEPRMA2_M95M04_0, 0x00);
  M95_LockStatus(EEPRMA2_M95M04_0);

  /*
  Uncomment this function to permanently Lock ID Page on EEPRMA2_M95M04_0

  M95_LockID(EEPRMA2_M95M04_0);

  */

}

void MX_EEPROM_RW_Process(void)
{
}

/**
  * @brief  Write Enable(WREN) test examples for SPI_EEPROM memory .
  * @param  TargetName : SPI_EEPROM memory name to write.
  * @retval EEPROMEX enum status
  */
int32_t WriteEnableReadStatus(uint32_t TargetName)
{

  uint8_t rx[1];
  int32_t readstatus = BSP_ERROR_NONE;
  switch (TargetName)
  {
    case EEPRMA2_M95M04_0:
      if (EEPRMA2_M95_WriteCmd(EEPROMEX_WREN, M95M04_SPI_ADDR) == BSP_ERROR_NONE)
      {
        readstatus = EEPRMA2_SPI_ReadReg(rx, M95M04_SPI_ADDR);
      }
      else
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
    case EEPRMA2_M95256_0:
      if (EEPRMA2_M95_WriteCmd(EEPROMEX_WREN, M95256_SPI_ADDR) == BSP_ERROR_NONE)
      {
        readstatus = EEPRMA2_SPI_ReadReg(rx, M95256_SPI_ADDR);
      }
      else
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
    case EEPRMA2_M95040_0:
      if (EEPRMA2_M95_WriteCmd(EEPROMEX_WREN, M95040_SPI_ADDR) == BSP_ERROR_NONE)
      {
        readstatus = EEPRMA2_SPI_ReadReg(rx, M95040_SPI_ADDR);
      }
      else
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
  }

  if (TargetName == EEPRMA2_M95M04_0)
  {
    printf("target: %s|rx: %02x \r\n", "M95M04", rx[0]);
  }
  else if (TargetName == EEPRMA2_M95256_0)
  {
    printf("target: %s|rx: %02x \r\n", "M95256", rx[0]);
  }
  else if (TargetName == EEPRMA2_M95040_0)
  {
    printf("target: %s|rx: %02x \r\n", "M95040", rx[0]);
  }

  return readstatus;

}

/**
  * @brief  Write Disable(WRDI) test examples for SPI_EEPROM memory .
  * @param  TargetName : SPI_EEPROM memory name to write.
  * @retval EEPROMEX enum status
  */
int32_t WriteDisableReadStatus(uint32_t TargetName)
{
  uint8_t rx[1];
  int32_t readstatus = BSP_ERROR_NONE;
  switch (TargetName)
  {
    case EEPRMA2_M95M04_0:
      if (EEPRMA2_M95_WriteCmd(EEPROMEX_WRDI, M95M04_SPI_ADDR) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        readstatus = EEPRMA2_SPI_ReadReg(rx, M95M04_SPI_ADDR);
      }
      break;
    case EEPRMA2_M95256_0:
      if (EEPRMA2_M95_WriteCmd(EEPROMEX_WRDI, M95256_SPI_ADDR) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        readstatus = EEPRMA2_SPI_ReadReg(rx, M95256_SPI_ADDR);
      }
      break;
    case EEPRMA2_M95040_0:
      if (EEPRMA2_M95_WriteCmd(EEPROMEX_WRDI, M95040_SPI_ADDR) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        readstatus = EEPRMA2_SPI_ReadReg(rx, M95040_SPI_ADDR);
      }
      break;
  }
  if (TargetName == EEPRMA2_M95M04_0)
  {
    printf("target: %s|rx: %02x \r\n", "M95M04", rx[0]);
  }
  else if (TargetName == EEPRMA2_M95256_0)
  {
    printf("target: %s|rx: %02x \r\n", "M95256", rx[0]);
  }
  else if (TargetName == EEPRMA2_M95040_0)
  {
    printf("target: %s|rx: %02x \r\n", "M95040", rx[0]);
  }

  return readstatus;
}

/**
  * @brief  Block Protect  + write + READ check test example for SPI_EEPROM memory .
  * @param  TargetAddress : SPI data memory address to write.
  * @param  TargetName : SPI_EEPROM memory name to write.
  * @param  tx : Input Buffer to Write.
  * @param  rx : Output Buffer to Read.
  * @param  Size : Size in bytes of the value to be written.
  * @retval EEPROMEX enum status
  */
int32_t TestData_BlockProtect(uint8_t Block_Protect, unsigned int TargetAddress, uint8_t TargetName, uint8_t tx[],
                              uint8_t rx[], uint16_t Size)
{
  int32_t readstatus = BSP_ERROR_NONE;

  EEPRMA2_M95_IsDeviceReady(TargetName);
  /* Protect */
  if (EEPRMA2_SPI_WriteReg(Block_Protect, TargetName) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  /* write */
  EEPRMA2_M95_WriteData(TargetName, tx, TargetAddress, Size);
  HAL_Delay(5);
  /* read */
  readstatus = EEPRMA2_M95_ReadData(TargetName, rx, TargetAddress, Size);

  if (TargetName == EEPRMA2_M95040_0)
  {
    printf("BlockProtect|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d \r\n\n", "M95040", TargetAddress,
           tx, rx, Size);
  }
  else if (TargetName == EEPRMA2_M95M04_0)
  {
    printf("BlockProtect|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d \r\n\n", "M95M04", TargetAddress,
           tx, rx, Size);
  }
  else if (TargetName == EEPRMA2_M95256_0)
  {
    printf("BlockProtect|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d \r\n\n", "M95256", TargetAddress,
           tx, rx, Size);
  }

  return readstatus;
}

/**
  * @brief  Write data + Protect with WC + READ to check no change test example only for I2C_EEPROM memory .
  * @param  TargetAddress : SPI data memory address to write.
  * @param  TargetName : SPI_EEPROM memory name to write.
  * @param  tx : Input Buffer to Write.
  * @param  rx : Output Buffer to Read.
  * @param  Size : Size in bytes of the value to be written.
  * @param  expWriteStatus : Expected Write EEPROM enum Status according to parameter passed.
  * @param  expReadStatus : Expected Read EEPROM enum Status according to parameter passed.
  */
void TestData_WriteProtect(unsigned int TargetAddress, uint8_t TargetName, uint8_t tx[], uint8_t rx[],
                           uint16_t Size, int32_t expWriteStatus, int32_t expReadStatus)
{

  EEPROMEX_CTRL_LOW(WC); /*Only for I2C*/
  int32_t writeStatus = EEPRMA2_M24_WriteData(TargetName, tx, TargetAddress, Size);
  HAL_Delay(5);

  EEPROMEX_CTRL_HIGH(WC); /*Only for I2C*/

  EEPRMA2_M24_WriteData(TargetName, tx10, TargetAddress, SIZE10);
  HAL_Delay(5);
  int32_t readstatus = EEPRMA2_M24_ReadData(TargetName, rx, TargetAddress, Size);
  HAL_Delay(5);

  if (TargetName == EEPRMA2_M24C02_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: passed \r\n",
             "M24C02", TargetAddress, tx, rx, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: failed \r\n",
             "M24C02", TargetAddress, tx, rx, Size);
    }
  }

  if (TargetName == EEPRMA2_M24256_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: passed \r\n",
             "M24256", TargetAddress, tx, rx, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: failed  \r\n",
             "M24256", TargetAddress, tx, rx, Size);
    }
  }

  if (TargetName == EEPRMA2_M24M01_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: passed \r\n",
             "M24M01", TargetAddress, tx, rx, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: failed\r\n",
             "M24M01", TargetAddress, tx, rx, Size);
    }
  }
}

/**
  * @brief  Test Byte Write and Byte Read in the memory at specific address.
  * @param  TargetAddress : I2C/SPI data memory address to write.
  * @param  TargetName : memory name to write.
  * @param  expWriteStatus : Expected Write EEPROM enum Status according to parameter passed.
  * @param  expReadStatus : Expected Read EEPROM enum Status according to parameter passed.
  * @retval None
  */
void M24_TestSingleByte(unsigned int TargetAddress, int32_t TargetName, int32_t expWriteStatus, int32_t expReadStatus)
{
  uint8_t tx[] = "k";
  uint8_t rx[1];
  EEPROMEX_CTRL_LOW(WC);
  int32_t writeStatus = EEPRMA2_M24_WriteByte(TargetName, tx, TargetAddress);
  HAL_Delay(10);
  EEPROMEX_CTRL_HIGH(WC);
  int32_t readstatus = EEPRMA2_M24_ReadByte(TargetName, rx, TargetAddress);
  HAL_Delay(10);

  switch (TargetName)
  {
    case EEPRMA2_M24C02_0:
      if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))  /* to check whether roll over case */
      {
        printf("TestByte|target: %s|address: %u|TX: %s|\t RX: %s|result: passed \r\n", "M24C02", TargetAddress, tx, rx);
      }
      else
      {
        printf("TestByte|target: %s|address: %u|result: failed \r\n", "M24C02", TargetAddress);
      }
      break;
    case EEPRMA2_M24256_0:
      if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))  /* to check whether roll over case */
      {
        printf("TestByte|target: %s|address: %u|TX: %s|\t RX: %s|result: passed \r\n", "M24256", TargetAddress, tx, rx);
      }
      else
      {
        printf("TestByte|target: %s|address: %u|result: failed \r\n", "M24256", TargetAddress);
      }
      break;
    case EEPRMA2_M24M01_0:
      if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))  /* to check whether roll over case */
      {
        printf("TestByte|target: %s|address: %u|TX: %s|\t RX: %s|result: passed \r\n", "M24M01", TargetAddress, tx, rx);
      }
      else
      {
        printf("TestByte|target: %s|address: %u|TX: %s|\t RX: %s|result: failed \r\n", "M24M01", TargetAddress, tx, rx);
      }
      break;
    default:
      break;
  }
}

/**
  * @brief  Test Byte Write and Byte Read in the memory at specific address.
  * @param  TargetAddress : I2C/SPI data memory address to write.
  * @param  TargetName : memory name to write.
  * @param  expWriteStatus : Expected Write EEPROM enum Status according to parameter passed.
  * @param  expReadStatus : Expected Read EEPROM enum Status according to parameter passed.
  * @retval None
  */
void M95_TestSingleByte(unsigned int TargetAddress, uint32_t TargetName, int32_t expWriteStatus,
                        int32_t expReadStatus)
{
  uint8_t tx[] = "b";
  uint8_t rx[1];

  int32_t writeStatus = EEPRMA2_M95_WriteByte(TargetName, tx, TargetAddress);
  HAL_Delay(10);

  int32_t readstatus = EEPRMA2_M95_ReadByte(TargetName, rx, TargetAddress);
  HAL_Delay(10);

  switch (TargetName)
  {
    case EEPRMA2_M95M04_0:
      if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))     /* to check whether roll over case */
      {
        printf("TestByte|target: %s|address: %u|TX: %s|\t RX: %s|result: passed \r\n",
               "M95M04", TargetAddress, tx, rx);
      }
      else
      {
        printf("TestByte|target: %s|address: %u|result: failed \r\n", "M95M04", TargetAddress);
      }
      break;
    case EEPRMA2_M95256_0:
      if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))     /* to check whether roll over case */
      {
        printf("TestByte|target: %s|address: %u|result: passed \r\n", "M95256", TargetAddress);
      }
      else
      {
        printf("TestByte|target: %s|address: %u|result: failed \r\n", "M95256", TargetAddress);
      }
      break;
    case EEPRMA2_M95040_0:
      if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))     /* to check whether roll over case */
      {
        printf("TestByte|target: %s|address: %u|TX: %s|\t RX: %s|result: passed \r\n",
               "M95040", TargetAddress, tx, rx);
      }
      else
      {
        printf("TestByte|target: %s|address: %u|TX: %s|\t RX: %s|result: failed \r\n",
               "M95040", TargetAddress, tx, rx);
      }
      break;

    default:
      break;
  }
}

/**
  * @brief  Test Page Write/Read in the memory at specific address.
  * @param  TargetAddress : I2C/SPI data memory address to write.Its pointing to pageIndex Number.
  * @param  TargetName : memory name to write.
  * @param  tx : Input Buffer to Write.
  * @param  rx : Output Buffer to Read.
  * @param  Size : Size in bytes of the value to be written.
  * @param  expWriteStatus : Expected Write EEPROM enum Status according to parameter passed.
  * @param  expReadStatus : Expected Read EEPROM enum Status according to parameter passed.
  * @retval None
  */
void M24_TestPage(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[],
                  uint16_t Size, int32_t expWriteStatus, int32_t expReadStatus)
{

  EEPROMEX_CTRL_LOW(WC);  /* Only for I2C */
  int32_t writeStatus = EEPRMA2_M24_WritePage(TargetName, tx, TargetAddress, Size);
  HAL_Delay(5);
  EEPROMEX_CTRL_HIGH(WC);  /* Only for I2C */
  int32_t readstatus = EEPRMA2_M24_ReadPage(TargetName, rx, TargetAddress, Size) ;
  HAL_Delay(5);

  if (TargetName == EEPRMA2_M24C02_0)
  {
    /* To check whether Actual status is same as Expected status, so as to verify roll over case */
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: passed \r\n", "M24C02", TargetAddress, Size);
    }
    else
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: failed \r\n", "M24C02", TargetAddress, Size);
    }
  }
  else if (TargetName == EEPRMA2_M24256_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: passed \r\n", "M24256", TargetAddress, Size);
    }
    else
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: failed \r\n", "M24256", TargetAddress, Size);
    }
  }
  else if (TargetName == EEPRMA2_M24M01_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: passed \r\n", "M24M01", TargetAddress, Size);
    }
    else
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: failed \r\n", "M24M01", TargetAddress, Size);
    }
  }
}

/**
  * @brief  Test Page Write/Read in the memory at specific address.
  * @param  TargetAddress : I2C/SPI data memory address to write.Its pointing to pageIndex Number.
  * @param  TargetName : memory name to write.
  * @param  tx : Input Buffer to Write.
  * @param  rx : Output Buffer to Read.
  * @param  Size : Size in bytes of the value to be written.
  * @param  expWriteStatus : Expected Write EEPROM enum Status according to parameter passed.
  * @param  expReadStatus : Expected Read EEPROM enum Status according to parameter passed.
  * @retval None
  */
void M95_TestPage(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t Size,
                  int32_t expWriteStatus, int32_t expReadStatus)
{
  int32_t writeStatus = EEPRMA2_M95_WritePage(TargetName, tx, TargetAddress, Size);
  HAL_Delay(5);
  int32_t readstatus = EEPRMA2_M95_ReadPage(TargetName, rx, TargetAddress, Size) ;
  HAL_Delay(5);

  if (TargetName == EEPRMA2_M95M04_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: passed \r\n", "M95M04", TargetAddress, Size);
    }
    else
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: failed \r\n", "M95M04", TargetAddress, Size);
    }
  }
  else if (TargetName == EEPRMA2_M95040_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: passed \r\n", "M95040", TargetAddress, Size);
    }
    else
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: failed \r\n", "M95040", TargetAddress, Size);
    }
  }
  else if (TargetName == EEPRMA2_M95256_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: passed \r\n", "M95256", TargetAddress, Size);
    }
    else
    {
      printf("TestPage|target: %s|address: %u|data size: %d|result: failed \r\n", "M95256", TargetAddress, Size);
    }
  }
}

/**
  * @brief  Bytes Write/Read in the memory at specific address.
  * @param  TargetAddress : I2C/SPI data memory address to write.
  * @param  TargetName : memory name to write.
  * @param  tx : Input Buffer to Write.
  * @param  rx : Output Buffer to Read.
  * @param  Size : Size in bytes of the value to be written.
  * @param  expWriteStatus : Expected Write EEPROM enum Status according to parameter passed.
  * @param  expReadStatus : Expected Read EEPROM enum Status according to parameter passed.
  * @retval None
  */
void M24_TestData(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t Size,
                  int32_t expWriteStatus, int32_t expReadStatus)
{
  int32_t writeStatus = EEPRMA2_M24_WriteData(TargetName, tx, TargetAddress, Size);
  HAL_Delay(5);
  int32_t readstatus = EEPRMA2_M24_ReadData(TargetName, rx, TargetAddress, Size);
  HAL_Delay(5);

  if (TargetName == EEPRMA2_M24C02_0)
  {
    /* To check whether Actual status is same as Expected status, so as to verify roll over case */
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: passed \r\n",
             "M24C02", TargetAddress, tx, rx, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: failed \r\n", "M24C02", TargetAddress, Size);
    }
  }
  else if (TargetName == EEPRMA2_M24256_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: passed \r\n", "M24256", TargetAddress, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: failed \r\n", "M24256", TargetAddress, Size);
    }
  }
  else if (TargetName == EEPRMA2_M24M01_0)
  {
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: passed \r\n", "M24M01", TargetAddress, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: failed \r\n", "M24M01", TargetAddress, Size);
    }
  }
}

/**
  * @brief  Bytes Write/Read in the memory at specific address.
  * @param  TargetAddress : I2C/SPI data memory address to write.
  * @param  TargetName : memory name to write.
  * @param  tx : Input Buffer to Write.
  * @param  rx : Output Buffer to Read.
  * @param  Size : Size in bytes of the value to be written.
  * @param  expWriteStatus : Expected Write EEPROM enum Status according to parameter passed.
  * @param  expReadStatus : Expected Read EEPROM enum Status according to parameter passed.
  * @retval None
  */
void M95_TestData(unsigned int TargetAddress, uint32_t TargetName, uint8_t tx[], uint8_t rx[], uint16_t Size,
                  int32_t expWriteStatus, int32_t expReadStatus)
{
  int32_t writeStatus = EEPRMA2_M95_WriteData(TargetName, tx, TargetAddress, Size);
  HAL_Delay(5);
  int32_t readstatus = EEPRMA2_M95_ReadData(TargetName, rx, TargetAddress, Size);
  HAL_Delay(5);

  if (TargetName == EEPRMA2_M95256_0)
  {
    /* To check whether Actual status is same as Expected status, so as to verify roll over case */
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: passed \r\n",
             "M95256", TargetAddress, tx, rx, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: failed \r\n", "M95256", TargetAddress, Size);
    }
  }
  if (TargetName == EEPRMA2_M95040_0)
  {
    /* To check whether Actual status is same as Expected status, so as to verify roll over case */
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: passed \r\n",
             "M95040", TargetAddress, tx, rx, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: failed \r\n", "M95040",  TargetAddress, Size);
    }
  }
  if (TargetName == EEPRMA2_M95M04_0)
  {
    /* To check whether Actual status is same as Expected status, so as to verify roll over case */
    if ((writeStatus == expWriteStatus) && (readstatus == expReadStatus))
    {
      printf("TestData|target: %s|address: %u|TX: %s|\t RX: %s|data size: %d|result: passed \r\n",
             "M95M04", TargetAddress, tx, rx, Size);
    }
    else
    {
      printf("TestData|target: %s|address: %u|data size: %d|result: failed \r\n", "M95M04", TargetAddress, Size);
    }
  }
}

/**
  * @brief  Read Identification Page in the memory.
  * @param  TargetName : memory name to read.
  * @param  TargetAddress : SPI data memory address to read.
  * @retval readstatus
  */
int32_t M95_ReadID(uint32_t TargetName, const unsigned int TargetAddress)
{
  uint8_t rx[512] = {0};
  int32_t readstatus = BSP_ERROR_NONE;
  uint32_t index = 0;

  readstatus =  EEPRMA2_M95_ReadID(TargetName, rx, TargetAddress, 15);

  printf(" Read ID Page from address : %6.6ux:\n", TargetAddress);
  for (index = 0; index < 15U; index++)
  {
    printf("  %2.2x", rx[index]);
  }

  HAL_Delay(10);
  return readstatus;
}

/**
  * @brief  Write Identification Page in the memory.
  * @param  TargetName : memory name to write.
  * @param  TargetAddress : SPI data memory address to write.
  * @retval writestatus
  */
int32_t M95_WriteID(uint32_t TargetName, const uint32_t TargetAddress)
{
  uint8_t tx[10] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
  uint16_t size = 10;
  int32_t writestatus = BSP_ERROR_NONE;

  printf("\n Writing Identification Page...\r\n");
  writestatus =  EEPRMA2_M95_WriteID(TargetName, tx, TargetAddress, size);

  HAL_Delay(10);
  return writestatus;
}

/**
  * @brief  Read lock status of Identification Page in the memory.
  * @param  TargetName : memory name to write.
  * @retval readstatus
  */
int32_t M95_LockStatus(uint32_t TargetName)
{
  uint8_t rx[1];
  uint8_t lock_status;
  int32_t readstatus = BSP_ERROR_NONE;

  readstatus = EEPRMA2_M95_LockStatus(TargetName, rx);
  lock_status = rx[0] & BIT0;

  if (lock_status == 0U)
  {
    printf("\n\rLock not active\r\n");
  }
  else
  {
    printf("\n\rLock active\r\n");
  }

  HAL_Delay(10);
  return readstatus;
}

/**
  * @brief  Lock Identification Page in the memory.
  * @param  TargetName : memory name to write.
  * @retval status
  */
int32_t M95_LockID(uint32_t TargetName)
{
  int32_t status = BSP_ERROR_NONE;

  status = EEPRMA2_M95_LockID(TargetName);

  return status;
}

#ifdef __cplusplus
}
#endif

