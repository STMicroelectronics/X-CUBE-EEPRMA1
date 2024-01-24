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

#include "app_eepromrw.h"
#include "main.h"

/** @defgroup EEPROM_NUCLEO
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SIZE1024      1024U
/* Private variables ---------------------------------------------------------*/
uint8_t Recv_Buff[1024] = {0};
uint8_t Transmit_Buff[1024] = {0};
/* Global variables ----------------------------------------------------------*/
extern uint8_t M24M01E_CDA_DevSelCode;
extern uint8_t M24M01E_DTI_DevSelCode;
extern uint8_t M24M01E_IDPage_DevSelCode;
extern uint8_t M24M01E_SWP_DevSelCode;
extern uint8_t M24M01E_Memory_DevSelCode;
extern uint8_t M24256E_CDA_DevSelCode;

/* Private function prototypes -----------------------------------------------*/

#if DEBUG_APPLI
#define PRINTF_APPLI(...) printf(__VA_ARGS__)
#else
#define PRINTF_APPLI(...)
#endif /* DEBUG_APPLI */
/* USER CODE END PM */

/**
  * @brief  Initialize the EEPROM example
  * @param  None
  * @retval None
  */
void MX_EEPROM_Init(void)
{

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* Initialize the peripherals and the M24256E and M24M01E components */

  MX_EEPROM_RW_Init();

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */
}

/**
  * @brief  EEPROM process
  * @param  None
  * @retval None
  */
void MX_EEPROM_Process(void)
{
  /* USER CODE BEGIN M24_Library_Process */

  MX_EEPROM_RW_Process();

  /* USER CODE END M24_Library_Process */
}

/**
  * @brief  Initialize the EEPROM Read Write Example
  * @param  None
  * @retval None
  */
void MX_EEPROM_RW_Init(void)
{

  /* Init UART for display message on console */
  BSP_COM_Init(COM1);
  while (EEICA1_M24256E_Init(EEICA1_M24256E) != BSP_ERROR_NONE);
  while (EEICA1_M24M01E_Init(EEICA1_M24M01E) != BSP_ERROR_NONE);

  /* At the very first initialization, M24256E-F and M24M01E-F EEPROM will have */
  /*same Device Address as Configuration Device Address (CDA) Register will be 0x00 by default.  */
  /*This will cause conflicts on I2C bus. To avoid this conflict, CDA register of M24M01E-F will be updated. */

  /* 0xB3 as Device Select Code will select M24M01E-F only */
  /* C1 (Bit 2) will be set to 1 in CDA. M24M01E-F */
  EEICA1_Setup_CDA_Register();

  /* Test Cases */

  /* Read CDA Register */
  EEICA1_Read_CDA_Register();

  /* Write CDA Register */
  EEICA1_Write_CDA_Register();

  /* Lock CDA Register*/
  /* Uncomment this function to lock CDA register permanently. Irreversible action !! */
  /* EEICA1_Lock_CDA_Register(); */

  /* Read DTI Register for M24M01 only*/
  EEICA1_Read_DTI_Register();

  /* Read/Write ID page */
  EEICA1_Test_M24256E_ID_Page();
  EEICA1_Test_M24M01E_ID_Page();

  /* Lock ID page*/
  /* Uncomment this function to lock ID page permanently. Irreversible action !! */
  /* EEICA1_Test_M24256E_LockID_Page(); */
  /* EEICA1_Test_M24M01E_LockID_Page(); */

  /* Read SWP Register for M24M01 only */
  EEICA1_Read_SWP_Register();
  /* Write SWP Register for M24M01 only*/
  EEICA1_Write_SWP_Register();

  /* Memory Read/Write M24256E */
  EEICA1_Test_M24256E_Memory();

  /* Memory Read/Write M24M01E */
  EEICA1_Test_M24M01E_Memory();

  /* TURN NUCLEO LED ON only if all test cases pass,  else toggle LED and break*/
  printf("\nAll test cases passed successfully ! \r\n");
  HAL_GPIO_WritePin(EEICA1_NUCLEO_LED_PORT, EEICA1_NUCLEO_LED_PIN, GPIO_PIN_SET);

}

/**
  * @brief  Setup CDA register of M24M01E to avoid bus conflict
  * @param  None
  * @retval None
  */
void EEICA1_Setup_CDA_Register(void)
{

  uint8_t readCDA = 0xFF;
  uint8_t writeCDA = 0xFF;

  /* With Bit 1 set, only M24M01E will respond initially */
  M24M01E_CDA_DevSelCode = 0xB2;
  M24M01E_DTI_DevSelCode = 0xB2;
  M24M01E_IDPage_DevSelCode = 0xB2;
  M24M01E_SWP_DevSelCode = 0xB2;
  M24M01E_Memory_DevSelCode = 0xA2;

  printf("***************************************************************\r\n");
  printf("                SETUP CDA REGISTER M24M01E\r\n");
  printf("***************************************************************\r\n");

  if (EEICA1_M24M01E_ReadCDARegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
  {

    /* Attempt read with C1 bit set*/
    M24M01E_CDA_DevSelCode = 0xB4; /* assuming only C1 bit is set */
    M24M01E_DTI_DevSelCode = 0xB4;
    M24M01E_IDPage_DevSelCode = 0xB4;
    M24M01E_SWP_DevSelCode = 0xB4;
    M24M01E_Memory_DevSelCode = 0xA4;

    if (EEICA1_M24M01E_ReadCDARegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
    {
      printf("Selected Device address is incorrect. Abort!\r\n");
      App_Error_Handler();
    }
    else
    {
      printf("M24M01 CDA: 0x%2.2X\r\n", readCDA);
    }
  }

  else
  {
    if (readCDA == 0)
    {
      PRINTF_APPLI("C1 and C2 bits of CDA in M24M01 are 0. Setting C1 bit of CDA register.\r\n");
      printf("__Writing CDA Val__M24M01 <C1 Bit Set>\r\n");
      writeCDA = 0x04;
      if (EEICA1_M24M01E_WriteCDARegister(EEICA1_M24M01E, &writeCDA) != BSP_ERROR_NONE)
      {
        printf("M24M01 CDA update failed!\r\n");
        App_Error_Handler();

      }
      else
      {
        printf("Updated M24M01 CDA: 0x%2.2X\r\n", writeCDA);

      }
    }
  }

  /* Uncomment this code section to set CDA of M24M01E back to default value of 0x00*/
  /* Note: application tests must run with different CDA values of M24M01E and M24256E */

  /*
  if (EEICA1_M24M01E_ReadCDARegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
  {
    printf("Selected Device address is incorrect. Abort!\r\n");
  }
  else
  {
    printf("M24M01 CDA: 0x%2.2X\r\n", readCDA);
    writeCDA = 0x00;
    if (EEICA1_M24M01E_WriteCDARegister(EEICA1_M24M01E, &writeCDA) != BSP_ERROR_NONE)
    {
      printf("Write to M24M01 CDA failed, Device Addr is 0x%2.2X\r\n", M24M01E_CDA_DevSelCode);
    }
    else
    {
      printf("Updated M24M01 CDA: 0x%2.2X\r\n", writeCDA);
    }
  }
  */

}

/**
  * @brief  Read CDA register
  * @param  None
  * @retval None
  */
void EEICA1_Read_CDA_Register(void)
{

  uint8_t readCDA = 0xFF;
  printf("\n\n***************************************************************\r\n");
  printf("                     READ CDA REGISTER\r\n");
  printf("***************************************************************\r\n");

  if (EEICA1_M24256E_ReadCDARegister(EEICA1_M24256E, &readCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("M24256E CDA Register: 0x%2.2X\r\n", readCDA);
  }

  if (EEICA1_M24M01E_ReadCDARegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("M24M01E CDA Register: 0x%2.2X\r\n", readCDA);
  }

}

/**
  * @brief  Write CDA register
  * @param  None
  * @retval None
  */
void EEICA1_Write_CDA_Register(void)
{
  uint8_t readCDA = 0xFF;
  uint8_t writeCDA = 0xFF;

  printf("\n\n***************************************************************\r\n");
  printf("                     WRITE CDA REGISTER\r\n");
  printf("***************************************************************\r\n");

  /* Read CDA Register of M24256E */
  if (EEICA1_M24256E_ReadCDARegister(EEICA1_M24256E, &readCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("M24256E CDA Register: 0x%2.2X\r\n", readCDA);
  }

  /* Write CDA Register of M24256E, Set C2 (Bit 3) */
  printf("Setting C2 (Bit 3) of CDA register in M24256E . . .\r\n");
  writeCDA = 0x08;

  if (EEICA1_M24256E_WriteCDARegister(EEICA1_M24256E, &writeCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    HAL_Delay(5); /* Tw max is 5ms */

    if (EEICA1_M24256E_ReadCDARegister(EEICA1_M24256E, &readCDA) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      printf("Updated M24256E CDA Register: 0x%2.2X\r\n", readCDA);
    }

  }

  /* Write CDA Register of M24256E, Clear C2 (Bit 3) */
  printf("Clearing C2 (Bit 3) of CDA register in M24256E . . .\r\n");
  writeCDA = 0x00;

  if (EEICA1_M24256E_WriteCDARegister(EEICA1_M24256E, &writeCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    HAL_Delay(5); /* Tw max is 5ms */
    if (EEICA1_M24256E_ReadCDARegister(EEICA1_M24256E, &readCDA) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      printf("Updated M24256E CDA Register: 0x%2.2X\r\n", readCDA);
    }

  }
}

/**
  * @brief  Lock CDA register
  * @param  None
  * @retval None
  */
void EEICA1_Lock_CDA_Register(void)
{
  uint8_t readCDA = 0xFF;
  uint8_t writeCDA = 0xFF;

  printf("\n\n***************************************************************\r\n");
  printf("           LOCK CDA REGISTER (NOTE: IRREVERSIBLE)\r\n");
  printf("***************************************************************\r\n");

  /* Read CDA Register of M24256E */
  if (EEICA1_M24256E_ReadCDARegister(EEICA1_M24256E, &readCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("M24256E CDA Register: 0x%2.2X\r\n", readCDA);
  }

  /* Write CDA Register of M24256E, Set C2 (Bit 3) */
  printf("Setting C2 (Bit 3) and DAL (Bit 0) of CDA register in M24256E . . .\r\n");
  writeCDA = 0x09;

  if (EEICA1_M24256E_WriteCDARegister(EEICA1_M24256E, &writeCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    HAL_Delay(5); /* Tw max is 5ms */

    if (EEICA1_M24256E_ReadCDARegister(EEICA1_M24256E, &readCDA) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      printf("Updated M24256E CDA Register: 0x%2.2X\r\n", readCDA);
    }

  }

  /* Write CDA Register of M24256E, Clear C2 (Bit 3) */
  printf("Attempting clearing C2 (Bit 3) of CDA register in M24256E . . .\r\n");
  writeCDA = 0x00;

  if (EEICA1_M24256E_WriteCDARegister(EEICA1_M24256E, &writeCDA) != BSP_ERROR_NONE)
  {
    printf("Clearing C2 (Bit 3) of CDA register in M24256E failed as expected . . .\r\n");
    HAL_Delay(5); /* Tw max is 5ms */
    if (EEICA1_M24256E_ReadCDARegister(EEICA1_M24256E, &readCDA) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      printf("M24256E CDA Register: 0x%2.2X\r\n", readCDA);
    }

  }
  else
  {
    printf("Clearing C2 bit of CDA register in M24256E done successfully with DAL bit set. Undefined behaviour!\r\n");
    App_Error_Handler();
  }
}

/**
  * @brief  Read DTI register M24M01E
  * @param  None
  * @retval None
  */
void EEICA1_Read_DTI_Register(void)
{

  uint8_t readCDA = 0xFF;
  printf("\n\n***************************************************************\r\n");
  printf("            READ DTI REGISTER only for M24M01E\r\n");
  printf("***************************************************************\r\n");

  if (EEICA1_M24M01E_ReadDTIRegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("M24M01E DTI Register: 0x%2.2X\r\n", readCDA);
  }

}

/**
  * @brief  Test Identification page of M24256E
  * @param  None
  * @retval None
  */
void EEICA1_Test_M24256E_ID_Page(void)
{
  uint32_t addr = 0x00;
  uint16_t idx = 0;
  uint8_t rx_buff[64] = {0};
  uint8_t tx_buff[64] = {0};

  printf("\n\n***************************************************************\r\n");
  printf("                     TEST M24256E ID PAGE\r\n");
  printf("***************************************************************\r\n");

  /* M24256E */
  printf("Read ID Page M24256E:\r\n");

  /* Read ID Page */
  if (EEICA1_M24256E_ReadIDPage(EEICA1_M24256E, rx_buff, addr, M24256E_PAGESIZE) != BSP_ERROR_NONE)
  {
    App_Error_Handler();

  }
  else
  {
    for (idx = 0; idx < M24256E_PAGESIZE; idx++)
    {
      printf("0x%2.2X ", rx_buff[idx]);
    }

  }

  /* Write ID Page */
  memset(tx_buff, 0x55, M24256E_PAGESIZE);
  memset(rx_buff, 0x00, M24256E_PAGESIZE);
  printf("\n\nWriting data 0x55 to complete ID page of M2456E \r\n ");
  if (EEICA1_M24256E_WriteIDPage(EEICA1_M24256E, tx_buff, addr, M24256E_PAGESIZE) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    HAL_Delay(5);
    if (EEICA1_M24256E_ReadIDPage(EEICA1_M24256E, rx_buff, addr, M24256E_PAGESIZE) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      for (idx = 0; idx < M24256E_PAGESIZE; idx++)
      {
        if (rx_buff[idx] == 0x55)
        {
          printf("0x%2.2X ", rx_buff[idx]);
        }
        else
        {
          break;
        }
      }

      if (idx == M24256E_PAGESIZE)
      {
        printf("\nAll data to M24256E ID Page written successfully!\r\n");
      }
      else
      {
        printf("Error in ID Page write M24M256E.\r\n");
        App_Error_Handler();
      }

    }
  }
  /* Clear ID Page to 0xFF */
  memset(tx_buff, 0xFF, M24256E_PAGESIZE);
  if (EEICA1_M24256E_WriteIDPage(EEICA1_M24256E, tx_buff, addr, M24256E_PAGESIZE) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("Cleared ID page of M24256E to 0xFF\r\n ");
  }
}

/**
  * @brief  Test Identification page of M24M01E
  * @param  None
  * @retval None
  */
void EEICA1_Test_M24M01E_ID_Page(void)
{
  uint32_t addr = 0x00;
  uint16_t count = 0;

  printf("\n\n***************************************************************\r\n");
  printf("                     TEST M24M01E ID PAGE\r\n");
  printf("***************************************************************\r\n");

  /* M24M01E */
  printf("Read ID Page M24M01E:\r\n");

  /* Read ID Page */
  if (EEICA1_M24M01E_ReadIDPage(EEICA1_M24M01E, Recv_Buff, addr, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
  {
    App_Error_Handler();

  }
  else
  {
    for (uint16_t idx = 0; idx < M24M01E_PAGESIZE; idx++)
    {
      printf("0x%2.2X ", Recv_Buff[idx]);
    }

  }

  /* Write ID Page */
  memset(Transmit_Buff, 0xAA, M24M01E_PAGESIZE);
  memset(Recv_Buff, 0x00, M24M01E_PAGESIZE);
  printf("\n\nWriting data 0xAA to complete ID page of M24M01E \r\n ");
  if (EEICA1_M24M01E_WriteIDPage(EEICA1_M24M01E, Transmit_Buff, addr, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    HAL_Delay(5);
    if (EEICA1_M24M01E_ReadIDPage(EEICA1_M24M01E, Recv_Buff, addr, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      for (count = 0; count < M24M01E_PAGESIZE; count++)
      {
        if (Recv_Buff[count] == 0xAA)
        {
          printf("0x%2.2X ", Recv_Buff[count]);
        }
        else
        {
          break;
        }
      }

      if (count == M24M01E_PAGESIZE)
      {
        printf("\nAll data to M24M01E ID Page written successfully!\r\n");
      }
      else
      {
        printf("Error in ID Page write M24M01E.\r\n");
        App_Error_Handler();
      }
    }

  }

  /* Clear ID Page to 0xFF */
  memset(Transmit_Buff, 0xFF, M24M01E_PAGESIZE);
  if (EEICA1_M24M01E_WriteIDPage(EEICA1_M24M01E, Transmit_Buff, addr, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("Cleared ID page of M24M01E to 0xFF\r\n ");
  }
}

/**
  * @brief  Lock Identification page of M24256E
  * @param  None
  * @retval None
  */
void EEICA1_Test_M24256E_LockID_Page(void)
{
  if (EEICA1_M24256E_LockIDPage(EEICA1_M24256E) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("Permanently Locked ID page of M24256E. \r\n ");
  }
}

/**
  * @brief  Lock Identification page of M24M01E
  * @param  None
  * @retval None
  */
void EEICA1_Test_M24M01E_LockID_Page(void)
{
  if (EEICA1_M24M01E_LockIDPage(EEICA1_M24M01E) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("Permanently Locked ID page of M24M01E. \r\n ");
  }
}

/**
  * @brief  Read SWP register of M24M01E
  * @param  None
  * @retval None
  */
void EEICA1_Read_SWP_Register(void)
{
  uint8_t readCDA = 0xFF;
  printf("\n\n***************************************************************\r\n");
  printf("            TEST SWP REGISTER only for M24M01E\r\n");
  printf("***************************************************************\r\n");

  if (EEICA1_M24M01E_ReadSWPRegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    printf("M24M01E SWP Register: 0x%2.2X\r\n", readCDA);
  }
}

/**
  * @brief  Write SWP register and test write to memory of M24M01E
  * @param  None
  * @retval None
  */
void EEICA1_Write_SWP_Register(void)
{
  uint8_t writeCDA = 0xFF;
  uint8_t readCDA = 0xFF;
  unsigned int addr  = 0x00;
  uint16_t nbytes = 10;
  uint8_t testdata = 0xBB;
  uint16_t idx;
  writeCDA = 0x0E; /* whole memory is write protected */

  if (EEICA1_M24M01E_WriteSWPRegister(EEICA1_M24M01E, &writeCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }

  else
  {
    HAL_Delay(5);
    if (EEICA1_M24M01E_ReadSWPRegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      printf("Modified M24M01E SWP Register to: 0x%2.2X. Complete memory locked.\r\n", readCDA);
      printf("Attempt to write to memory from Address 0x%2.2X with %d bytes of data. . .\r\n", addr, nbytes);
      memset(Transmit_Buff, testdata, sizeof(Transmit_Buff));
      if (EEICA1_M24M01E_WriteData(EEICA1_M24M01E, Transmit_Buff, addr, nbytes) != BSP_ERROR_NONE)
      {
        printf("Write failed as expected\r\n");
      }
      else
      {
        App_Error_Handler();
      }
      PRINTF_APPLI("Read memory from Address 0x%2.2X with %d bytes of data to check data is not written ...\r\n", addr,
                   nbytes);

      /* Read Data Written*/
      memset(Recv_Buff, 0x00, sizeof(Recv_Buff));
      if (EEICA1_M24M01E_ReadData(EEICA1_M24M01E, Recv_Buff, addr, nbytes) != BSP_ERROR_NONE)
      {
        App_Error_Handler();

      }
      else
      {
        for (idx = 0; idx < nbytes; idx++)
        {
          if (Recv_Buff[idx] == testdata)
          {
            break;
          }
          else
          {
            PRINTF_APPLI("0x%2.2X \r", Recv_Buff[idx]);
          }
        }
        if (idx == nbytes)
        {
          printf("Checked, data not written to memory as expected\r\n");

        }
      }

    }
  }

  writeCDA = 0x00; /* whole memory can be written */
  if (EEICA1_M24M01E_WriteSWPRegister(EEICA1_M24M01E, &writeCDA) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }

  else
  {
    HAL_Delay(5);
    if (EEICA1_M24M01E_ReadSWPRegister(EEICA1_M24M01E, &readCDA) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      printf("Updated M24M01E SWP Register: 0x%2.2X\r\n", readCDA);
    }
  }

}

/**
  * @brief  Test memory Read/Write operations on M24256E
  * @param  None
  * @retval None
  */
void EEICA1_Test_M24256E_Memory(void)
{
  unsigned int addr = 0x00;
  uint16_t idx = 0;
  uint16_t page_count_M24256E = M24256E_MEMORYSIZE / M24256E_PAGESIZE;
  uint16_t n_page;
  uint8_t test_data = 0x85;

  printf("\n\n***************************************************************\r\n");
  printf("                     TEST M24256E MEMORY\r\n");
  printf("***************************************************************\r\n");

  /* M24256E */
  /* 1. Read n_page (20) in memory */
  /* 2. Write 1024 bytes to memory starting from address addr(0x65) */
  /* 3. Read 1024 bytes from memory starting from address addr(0x65) */
  /* 4. Check memory contents are same as test_data(0x85) */
  /* 5. Clear memory contents to 0xFF */

  n_page = 20;
  memset(Recv_Buff, 0x00, sizeof(Recv_Buff));
  PRINTF_APPLI("Read M24256E %d Memory Pages:\r\n", n_page);

  /* 1. Read n_page (20) in memory*/
  if (n_page <= page_count_M24256E)
  {
    for (uint16_t page_idx = 0; page_idx < n_page; page_idx++)
    {
      if (EEICA1_M24256E_ReadData(EEICA1_M24256E, Recv_Buff, addr, M24256E_PAGESIZE) != BSP_ERROR_NONE)
      {
        App_Error_Handler();

      }
      else
      {
        PRINTF_APPLI("\n\nPage %d\n Address 0x%2.2X to 0x%2.2X: \r\n", page_idx + 1, addr, addr + M24256E_PAGESIZE - 1);
        for (idx = 0; idx < M24256E_PAGESIZE; idx++)
        {
          PRINTF_APPLI("0x%2.2X ", Recv_Buff[idx]);
        }
        addr += M24256E_PAGESIZE;
        memset(Recv_Buff, 0x00, sizeof(Recv_Buff));

      }
    }
  }
  else
  {
    printf("Number of pages exceeds total page in memory of M24256E. Abort. \r\n");
    App_Error_Handler();
  }

  /* 2. Write 1024 bytes to memory starting from address addr(0x65)*/

  memset(Transmit_Buff, test_data, sizeof(Transmit_Buff));
  addr = 0x65;
  printf("\nWrite 1024 bytes to memory from Address 0x%2.2X \r\n", addr);
  if (EEICA1_M24256E_WriteData(EEICA1_M24256E, Transmit_Buff, addr, SIZE1024) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    HAL_Delay(5);

    /* 3. Read 1024 bytes from memory starting from address addr(0x65)*/

    memset(Recv_Buff, 0x00, sizeof(Recv_Buff));
    if (EEICA1_M24256E_ReadData(EEICA1_M24256E, Recv_Buff, addr, SIZE1024) != BSP_ERROR_NONE)
    {
      App_Error_Handler();

    }
    else
    {
      /* 4. Check memory contents are same as test_data(0x85) */

      for (idx = 0; idx < SIZE1024; idx++)
      {
        if (Recv_Buff[idx] == test_data)
        {
          PRINTF_APPLI("0x%2.2X ", Recv_Buff[idx]);
        }
        else
        {
          break;
        }
      }
      if (idx == SIZE1024)
      {
        printf("\nWrite Success\r\n");
      }
      else
      {
        printf("\nWrite Failed\r\n");
        App_Error_Handler();
      }

      /* Read Page */
      n_page = 20;
      memset(Recv_Buff, 0x00, sizeof(Recv_Buff));
      printf("Read M24256E %d Memory Pages:\r\n", n_page);

      addr = 0;
      if (n_page <= page_count_M24256E)
      {
        for (uint16_t page_idx = 0; page_idx < n_page; page_idx++)
        {
          if (EEICA1_M24256E_ReadData(EEICA1_M24256E, Recv_Buff, addr, M24256E_PAGESIZE) != BSP_ERROR_NONE)
          {
            App_Error_Handler();

          }
          else
          {
            printf("\n\nPage %d\n Address 0x%2.2X to 0x%2.2X: \r\n", page_idx + 1, addr, addr + M24256E_PAGESIZE - 1);
            for (idx = 0; idx < M24256E_PAGESIZE; idx++)
            {
              printf("0x%2.2X ", Recv_Buff[idx]);
            }
            addr += M24256E_PAGESIZE;
            memset(Recv_Buff, 0x00, sizeof(Recv_Buff));

          }
        }
      }
      else
      {
        printf("Number of pages exceeds total page in memory of M24256E. Abort. \r\n");
        App_Error_Handler();
      }
    }

  }

  /* 5. Clear memory contents to 0xFF */
  addr = 0;
  printf("\nReset memory to 0xFF from Address:0x%2.2X \r\n", addr);
  memset(Transmit_Buff, 0xFF, sizeof(Transmit_Buff));

  /* replace n_page with page_count_M24256E to erase complete memory */
  for (uint16_t page_idx = 0; page_idx < n_page; page_idx++)
  {
    if (EEICA1_M24256E_WriteData(EEICA1_M24256E, Transmit_Buff, addr, M24256E_PAGESIZE) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      addr += M24256E_PAGESIZE;
    }

  }
  printf("\nMemory contents of M24256E cleared to 0xFF \r\n");
}

/**
  * @brief  Test memory Read/Write operations on M24M01E
  * @param  None
  * @retval None
  */
void EEICA1_Test_M24M01E_Memory(void)
{
  unsigned int addr = 0x00;
  uint16_t idx = 0;
  uint16_t page_count_M24M01E = M24M01E_MEMORYSIZE / M24M01E_PAGESIZE;
  uint16_t n_page;
  uint8_t test_data = 0xCC;

  printf("\n\n***************************************************************\r\n");
  printf("                     TEST M24M01E MEMORY\r\n");
  printf("***************************************************************\r\n");

  /* M24M01E */
  /* 1. Read n_page (6) in memory */
  /* 2. Write 1024 bytes to memory starting from address addr(0x12C) */
  /* 3. Read 1024 bytes from memory starting from address addr(0x12C) */
  /* 4. Check memory contents are same as test_data(0xCC) */
  /* 5. Clear memory contents to 0xFF */

  n_page = 6;
  memset(Recv_Buff, 0x00, sizeof(Recv_Buff));
  PRINTF_APPLI("Read M24M01E %d Memory Pages:\r\n", n_page);

  /* 1. Read n_page (6) in memory */
  if (n_page <= page_count_M24M01E)
  {
    for (uint16_t page_idx = 0; page_idx < n_page; page_idx++)
    {
      if (EEICA1_M24M01E_ReadData(EEICA1_M24M01E, Recv_Buff, addr, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
      {
        App_Error_Handler();

      }
      else
      {
        PRINTF_APPLI("\n\nPage %d\n Address 0x%2.2X to 0x%2.2X: \r\n", page_idx + 1, addr, addr + M24M01E_PAGESIZE - 1);
        for (idx = 0; idx < M24M01E_PAGESIZE; idx++)
        {
          PRINTF_APPLI("0x%2.2X ", Recv_Buff[idx]);
        }
        addr += M24M01E_PAGESIZE;
        memset(Recv_Buff, 0x00, sizeof(Recv_Buff));

      }
    }
  }
  else
  {
    printf("Number of pages exceeds total page in memory of M24M01E. Abort. \r\n");
    App_Error_Handler();
  }

  /* 2. Write 1024 bytes to memory starting from address addr(0x12C) */

  memset(Transmit_Buff, test_data, sizeof(Transmit_Buff));
  addr = 0x12C;
  printf("\nWrite 1024 bytes to memory from Address 0x%2.2X \r\n", addr);
  if (EEICA1_M24M01E_WriteData(EEICA1_M24M01E, Transmit_Buff, addr, SIZE1024) != BSP_ERROR_NONE)
  {
    App_Error_Handler();
  }
  else
  {
    HAL_Delay(5);
    /* 3. Read 1024 bytes from memory starting from address addr(0x12C) */
    memset(Recv_Buff, 0x00, sizeof(Recv_Buff));
    if (EEICA1_M24M01E_ReadData(EEICA1_M24M01E, Recv_Buff, addr, SIZE1024) != BSP_ERROR_NONE)
    {
      App_Error_Handler();

    }
    else
    {
      /* 4. Check memory contents are same as test_data(0xCC) */
      for (idx = 0; idx < SIZE1024; idx++)
      {
        if (Recv_Buff[idx] == test_data)
        {
          /* PRINTF_APPLI("0x%2.2X ", Recv_Buff[idx]); */
        }
        else
        {
          break;
        }
      }
      if (idx == SIZE1024)
      {
        printf("\nWrite Success\r\n");
      }
      else
      {
        printf("\nWrite Failed\r\n");
        App_Error_Handler();
      }

      /* Read Page */
      n_page = 6;
      memset(Recv_Buff, 0x00, sizeof(Recv_Buff));
      printf("Read M24M01E %d Memory Pages:\r\n", n_page);

      addr = 0;
      if (n_page <= page_count_M24M01E)
      {
        for (uint16_t page_idx = 0; page_idx < n_page; page_idx++)
        {
          if (EEICA1_M24M01E_ReadData(EEICA1_M24M01E, Recv_Buff, addr, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
          {
            App_Error_Handler();

          }
          else
          {
            printf("\n\nPage %d\n Address 0x%2.2X to 0x%2.2X: \r\n", page_idx + 1, addr, addr + M24M01E_PAGESIZE - 1);
            for (idx = 0; idx < M24M01E_PAGESIZE; idx++)
            {
              printf("0x%2.2X ", Recv_Buff[idx]);
            }
            addr += M24M01E_PAGESIZE;
            memset(Recv_Buff, 0x00, sizeof(Recv_Buff));

          }
        }
      }
      else
      {
        printf("Number of pages exceeds total page in memory of M24M01E. Abort. \r\n");
        App_Error_Handler();
      }
    }

  }
  /* 5. Clear memory contents to 0xFF */
  addr = 0;
  printf("\nReset memory to 0xFF from Address:0x%2.2X \r\n", addr);
  memset(Transmit_Buff, 0xFF, sizeof(Transmit_Buff));

  /* replace n_page with page_count_M24256E to erase complete memory */
  for (uint16_t page_idx = 0; page_idx < n_page; page_idx++)
  {
    if (EEICA1_M24M01E_WriteData(EEICA1_M24M01E, Transmit_Buff, addr, M24M01E_PAGESIZE) != BSP_ERROR_NONE)
    {
      App_Error_Handler();
    }
    else
    {
      addr += M24M01E_PAGESIZE;
    }

  }
  printf("\nMemory contents of M24M01E cleared to 0xFF \r\n");

}

/**
  * @brief  Process for EEPROMRW operations
  * @param  None
  * @retval None
  */
void MX_EEPROM_RW_Process(void)
{
  /* Do nothing */
}

/**
  * @brief  This function is executed in case of application error occurrence
  * @param  None
  * @retval None
  */
void App_Error_Handler(void)
{
  while (1)
  {
    HAL_GPIO_TogglePin(EEICA1_NUCLEO_LED_PORT, EEICA1_NUCLEO_LED_PIN);
    HAL_Delay(200);
  }
}

#ifdef __cplusplus
}
#endif

