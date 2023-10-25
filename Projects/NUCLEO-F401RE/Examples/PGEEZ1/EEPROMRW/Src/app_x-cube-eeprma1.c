/**
  ******************************************************************************
  * @file           : app_x-cube-eeprma1.c
  * @brief          : This file provides code for the configuration
  *						of the STMicroelectronics.X-CUBE-EEPRMA1.4.2.0 instances.
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

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include "app_x-cube-eeprma1.h"
#include "pgeez1_m95p32.h"

/** @defgroup EEPROM_NUCLEO
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define TARGET_ADDRESS_00          0x00
#define TARGET_ADDRESS_0x400       0x400
#define TARGET_ADDRESS_0x1800      0x1800

#define SIZE100         100U
#define SIZE200         200U
#define SIZE256         256U
#define SIZE520         520U
#define SIZE600         600U
#define SIZE1024        1024U
#define RXBUFFERSIZE    45U
#define M95P32_PAGE_SIZE 512U
#define M95P32_BLOCK_SIZE 65536U
#define M95P32_BLOCK_COUNT 64U

/* Private variables ---------------------------------------------------------*/
uint8_t ReceiveBuff[SIZE1024] = {0};
uint8_t pData[SIZE600];

#ifdef USE_QUADSPI
uint8_t Data[1040] = {0};
extern QSPI_HandleTypeDef QSPI_INSTANCE;
#endif

uint8_t SampleData[SIZE200] = {0};
uint8_t ReadBuff[SIZE200] = {0};

__IO ITStatus UartReady = RESET;
unsigned int add = 0x00;
uint8_t Buff[SIZE256]={0};
unsigned int CmdParam[SIZE520]={0};

volatile uint8_t IT_receive = 0;
uint8_t User_Choice = 0;

uint8_t aRxBuffer[RXBUFFERSIZE];
/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int32_t Test_ReadStatusReg(uint32_t TargetName);
int32_t WriteEnableReadStatus(uint32_t TargetName);
int32_t WriteDisableReadStatus(uint32_t TargetName);
void IntroScreen(void);
void User_Scenario_IntroScreen(void);
void Specific_Behaviour_IntroScreen(void);
HAL_StatusTypeDef UARTConsolePrint( char *puartmsg );
void App_Error_Handler(void);

/* USER CODE BEGIN PM */
#ifndef DEBUG_APPLI
#define DEBUG_APPLI 0
#endif

#if DEBUG_APPLI
#define PRINTF_APPLI(...) printf(__VA_ARGS__)
#else
#define PRINTF_APPLI(...)
#endif
/* USER CODE END PM */

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

  /* USER CODE BEGIN M95P32_Library_Init_PreTreatment */

  /* USER CODE END M95P32_Library_Init_PreTreatment */

  /* Initialize the peripherals and the M95P32 component */

  MX_EEPROM_RW_Init();
  IntroScreen();

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN M95P32_Library_Init_PostTreatment */

  /* USER CODE END M95P32_Library_Init_PostTreatment */

}

/**
* @brief  EEPROM process
* @param  None
* @retval None
*/
void MX_EEPROM_Process(void)
{
  /* USER CODE BEGIN M95P32_Library_Process */
   MX_EEPROM_RW_Process();

  /* USER CODE END M95P32_Library_Process */
}

/**
  * @brief Process going on in infinite while loop of main function
  * @param None
  * @retval None
  */
void MX_EEPROM_RW_Process()
{
  int32_t status;
  uint8_t status_reg_val;
  uint8_t volatile_reg_val;
  unsigned int index;
  uint32_t index_2;
  uint32_t index_3;

  if(IT_receive == 1U)
  {
    IT_receive = 0;

    if(User_Choice == 1)
    {
      HAL_UART_Receive(&hcom_uart[COM1], (uint8_t *)aRxBuffer+1, 30, 0x3ff);
    }
    else
    {
      HAL_UART_Receive(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 30, 0x3ff);
    }

    memset(&Buff, 0x00, SIZE256);

    index = 0;
    index_2 = 0;
    index_3 = 0;
    while(aRxBuffer[index] != 0U)
    {
      if(aRxBuffer[index]==0x20U)
      {

        CmdParam[index_3] = (unsigned int)strtol((char*)Buff,NULL,16);
        index_3 ++;
        memset(&Buff, 0x00, SIZE256);
        index ++;
        index_2 = 0;
      }
      else if (aRxBuffer[index]==0x0AU)
      {

        CmdParam[index_3] = (unsigned int)strtol((char*)Buff,NULL,16);
        memset(&Buff, 0x00, SIZE256);
        index_3 = 0;
        index_2 = 0;
        index = 0;
        break;
      }
      else
      {
        Buff[index_2] = aRxBuffer[index];
        index_2 ++;
        index ++;
      }
    }

    memset(&Buff, 0x00, SIZE256);
    memset(&aRxBuffer, 0x00, RXBUFFERSIZE);

    switch(CmdParam[0])
    {

    case 'R':
      User_Choice = 0;
      IntroScreen();
      break;

    case CMD_ERASE_SECTOR:
      if(CmdParam[2] != 0U)
      {
        printf("Erase Sector expects 0 parameter. Command not executed !\r\n");
        break;
      }
      printf("Erase Sector Add:  0x%6.6x \r\n",CmdParam[1]);
      PGEEZ1_M95P32_EraseSector(PGEEZ1_M95P32_0, CmdParam[1]);
      break;

    case CMD_ERASE_BLOCK:
      if(CmdParam[2] != 0U)
      {
        printf("Erase Block expects 0 parameter. Command not executed !\r\n");
        break;
      }
      printf("Erase Block Add:  0x%6.6x \r\n",CmdParam[1]);
      PGEEZ1_M95P32_EraseBlock(PGEEZ1_M95P32_0, CmdParam[1]);
      break;

    case CMD_ERASE_CHIP:
      if(CmdParam[1] != 0U)
      {
        printf(" Erase Chip expects 0 parameter. Command not executed !\r\n");
        break;
      }
      printf("Erase Chip \r\n");
      PGEEZ1_M95P32_EraseChip(PGEEZ1_M95P32_0);
      break;

    case CMD_ERASE_PAGE:
      if(CmdParam[2] != 0U)
      {
        printf("Erase Page expects 0 parameter. Command not executed !\r\n");
        break;
      }
      printf("Erase Page Add:  0x%6.6x\r\n",CmdParam[1]);
      PGEEZ1_M95P32_ErasePage(PGEEZ1_M95P32_0, CmdParam[1]);
      break;

    case CMD_PROG_PAGE:
      if(CmdParam[2] <= SIZE600)
      {
        memset(&pData, 0x00, SIZE600);
        add = CmdParam[1];
        for(index = 0; index < CmdParam[2]; index++)
        {
          pData[index] = (uint8_t)CmdParam[3];
        }

        PGEEZ1_M95P32_VolRegRead(PGEEZ1_M95P32_0, &volatile_reg_val);

        if( volatile_reg_val == 0x01U)
        {
          printf("Prog Page Add:  0x%6.6x \r\n ",CmdParam[1]);
          PGEEZ1_M95P32_ProgramPage(PGEEZ1_M95P32_0, pData, add, index);
        }
        else
        {
          printf("Prog Page in Buffer Mode Add:  0x%6.6x \r\n",CmdParam[1]);
          PGEEZ1_M95P32_PageProgBuffer(PGEEZ1_M95P32_0, pData, add, index);
        }
      }
      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_WRITE_PAGE:
      if(CmdParam[2] <= SIZE600)
      {
        memset(&pData, 0x00, SIZE600);
        add = CmdParam[1];
        for(index = 0; index < CmdParam[2]; index++)
        {
          pData[index] = (uint8_t)CmdParam[3];
        }
        printf("Write Page Add:  0x%6.6x \r\n",CmdParam[1]);
        PGEEZ1_M95P32_WritePage(PGEEZ1_M95P32_0, pData, add, index);
      }
      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
      }
      break;

    case CMD_WREN:
      if(CmdParam[1] != 0U)
      {
        printf("WRITE_ENABLE expects 0 parameter. Command not executed !\r\n");
        break;
      }
      printf("WRITE_ENABLE \r\n");
      PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
      break;

    case CMD_WRDI:
      if(CmdParam[1] != 0U)
      {
        printf("WRITE_DISABLE expects 0 parameter. Command not executed !\r\n");
        break;
      }
      printf("WRITE_DISABLE \r\n");
      PGEEZ1_M95P32_WriteDisable(PGEEZ1_M95P32_0);
      break;

    case CMD_READ_STATUS_REG:
      if(CmdParam[1] != 0U)
      {
        printf("CMD_READ_STATUS_REG expects 0 parameter. Command not executed !\r\n");
        break;
      }
      status = PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
      if(status != M95_OK)
      {
        printf("__ERROR in READ_STATUS_REG__\r\n");
        App_Error_Handler();
      }
      else
      {
        printf("STATUS_REG value: 0x%2.2x\r\n", status_reg_val);
      }
      break;

    case CMD_READ_CONF_SAFETY_REG:
      if(CmdParam[2] != 0U)
      {
        printf("CMD_READ_CONF_SAFETY_REG expects 1 parameter. Command not executed !\n");
        break;
      }
      printf("CMD_READ_CONF_SAFETY_REG\r\n");
      if(CmdParam[1] == 1U)
      {
        PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 1);
        printf("Configuration Register value: 0x%2.2x\n",ReceiveBuff[0]);
      }
      else
      {
        PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
        printf("Configuration Register value: 0x%2.2x \r\nSafety Register value: 0x%2.2x \r\n",ReceiveBuff[0],ReceiveBuff[1]);
      }
      break;

    case CMD_CLEAR_SAFETY_REG:
      if(CmdParam[1] != 0U)
      {
        printf("CMD_CLEAR_SAFETY_REG expects 0 parameter. Command not executed !\r\n");
        break;
      }
      printf("CMD_CLEAR_SAFETY_REG\r\n");
      PGEEZ1_M95P32_ClearSafetyFlag(PGEEZ1_M95P32_0);
      break;

    case CMD_READ_VOLATILE_REG:
      if(CmdParam[1] != 0U)
      {
        printf("CMD_READ_VOLATILE_REG expects 0 parameter. Command not executed !\r\n");
        break;
      }
      PGEEZ1_M95P32_VolRegRead(PGEEZ1_M95P32_0, &volatile_reg_val);
      printf("Volatile Register value: 0x%2.2x\r\n", volatile_reg_val);
      break;

    case CMD_WRITE_VOLATILE_REG:
      if(CmdParam[2] != 0U)
      {
        printf("CMD_WRITE_VOLATILE_REG expects 1 parameter only. Command not executed !\r\n");
        break;
      }
      printf("CMD_WRITE_VOLATILE_REG with value : 0x%2.2x\r\n",CmdParam[1]);
      PGEEZ1_M95P32_VolRegWrite(PGEEZ1_M95P32_0, (uint8_t)CmdParam[1]);
      break;

    case CMD_WRITE_STATUS_CONF_REG:
      if(CmdParam[3] != 0U)
      {
        printf("CMD_WRITE_STATUS_CONF_REG expects 1 or 2 parameters. Command not executed !\n");
        break;
      }
      memset(&pData, 0x00, SIZE520);
      pData[0] = (uint8_t)CmdParam[1];
      pData[1] = (uint8_t)CmdParam[2];
      printf("CMD_WRITE_STATUS_CONF_REG\r\nValues to be written:\r\nFor Status Register: 0x%2.2x \r\nFor Configuration Register: 0x%2.2x\r\n",pData[0],pData[1]);
      PGEEZ1_M95P32_StatusConfigRegWrite(PGEEZ1_M95P32_0, pData, 2);
      break;

    case CMD_READ_DATA:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf("CMD_READ_DATA expects 2 parameters. Command not executed !\r\n");
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Read data Add: 0x%6.6x:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  0x%2.2x",ReceiveBuff[index]);
        }
        printf("\n");
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_FAST_READ_SINGLE:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_SINGLE expects 2 parameters. Command not executed !\n");
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastRead(PGEEZ1_M95P32_0, ReceiveBuff, CmdParam[1],CmdParam[2]);
        printf(" Fast Read data Add: 0x%6.6x:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  0x%2.2x",ReceiveBuff[index]);
        }
        printf("\n");
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_FAST_READ_DUAL:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_DUAL expects 2 parameters. Command not executed !\r\n");
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastDRead(PGEEZ1_M95P32_0, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Dual Read data Add: 0x%6.6x:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2];index++)
        {
          printf("  0x%2.2x",ReceiveBuff[index]);
        }
        printf("\n");
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }

      break;

    case CMD_FAST_READ_QUAD:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_Quad expects 2 parameters. Command not executed !\r\n");
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastQRead(PGEEZ1_M95P32_0, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Quad Read data Add: 0x%6.6x:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  0x%2.2x",ReceiveBuff[index]);
        }
        printf("\n");
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_READ_ID_PAGE:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_READ_ID_PAGE expects 2 parameters. Command not executed !\r\n");
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_ReadID(PGEEZ1_M95P32_0, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Read Id Page data Add: 0x%6.6x:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  0x%2.2x",ReceiveBuff[index]);
        }
        printf("\n");
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_FAST_READ_ID_PAGE:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_ID_PAGE expects 2 parameters. Command not executed !\r\n");
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastReadID(PGEEZ1_M95P32_0, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Fast Read Id Page data Add: 0x%6.6x:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf(" 0x%2.2x",ReceiveBuff[index]);
        }
        printf("\n");
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_WRITE_ID_PAGE:
      if(CmdParam[2] <= SIZE1024)
      {
        memset(&pData ,0x00, SIZE520);
        add = CmdParam[1];
        for(index = 0; index < CmdParam[2]; index++)
        {
          pData[index] = (uint8_t)CmdParam[3];
        }
        PGEEZ1_M95P32_WriteID(PGEEZ1_M95P32_0, pData, add, CmdParam[2]);
        printf(" CMD_WRITE_ID_PAGE Add 0x%6.6x data : \n", add);
        for(index_2 = 0; index_2 < index; index_2++)
        {
          printf(" 0x%2.2x",pData[index_2] );
        }
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_DEEP_POWER_DOWN:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_DEEP_POWER_DOWN expects 0 parameter. Command not executed !\r\n");
        break;
      }
      PGEEZ1_M95P32_DeepPowerDown(PGEEZ1_M95P32_0);
      printf("CMD_DEEP_POWER_DOWN\r\n");
      break;

    case CMD_DEEP_POWER_DOWN_RELEASE:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_DEEP_POWER_DOWN_RELEASE expects 0 parameter. Command not executed !\r\n");
        break;
      }
      PGEEZ1_M95P32_DeepPowerDownRel(PGEEZ1_M95P32_0);
      printf("CMD_DEEP_POWER_DOWN_RELEASE\r\n");
      break;

    case CMD_READ_JEDEC:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_READ_JEDEC expects 0 parameter. Command not executed !\r\n");
        break;
      }
      memset(&ReceiveBuff, 0x00, SIZE1024);
      PGEEZ1_M95P32_JEDECRead(PGEEZ1_M95P32_0, ReceiveBuff, 3);
      printf("CMD_READ_JEDEC: ");
      for(index = 0; index < 3U; index++)
      {
        printf(" 0x%2.2x",ReceiveBuff[index]);
      }
      printf("\n");
      break;

    case CMD_READ_SFDP:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_READ_SFDP expects 2 parameter. Command not executed !\r\n");
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_SFDPRead(PGEEZ1_M95P32_0, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf("CMD_READ_SFDP: \n");
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("0x%4.4x : 0x%2.2x\n", index + CmdParam[1], ReceiveBuff[index]);
        }
        printf("\n");
      }

      else
      {
        printf("The requested size is greater than the buffer configured, kindly check MCU application accordingly.\r\n");
      }
      break;

    case CMD_ENABLE_RESET:
      if(CmdParam[1] != 0U)
      {
        printf("CMD_ENABLE_RESET expects 0 parameter. Command not executed !\r\n");
        break;
      }
      PGEEZ1_M95P32_EnableReset(PGEEZ1_M95P32_0);
      printf("CMD_ENABLE_RESET\r\n");
      break;

    case CMD_SOFT_RESET:
      if(CmdParam[1] != 0U)
      {
        printf("CMD_SOFT_RESET expects 0 parameter. Command not executed !\r\n");
        break;
      }
      PGEEZ1_M95P32_SoftReset(PGEEZ1_M95P32_0);
      printf("CMD_SOFT_RESET\r\n");
      break;

    default:
      printf("Nucleo_reader : Enter a ranged value : \r\n");
      break;
    }

    if(CmdParam[0] != 'R')
    {
      printf("Enter Command set: \r\n");
      User_Choice = 1;
    }

    memset(&CmdParam, 0x00, SIZE520 * sizeof(CmdParam[0]));

    UartReady = SET;
    HAL_NVIC_EnableIRQ(USARTX_INTERRUPT);
    if(HAL_UART_Receive_IT(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
      App_Error_Handler();
    }

  }

  /* User Scenario option selected*/
  if (IT_receive == 2)
  {
    IT_receive = 0;

    if(User_Choice != 2)
    {
      HAL_UART_Receive(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1, 0xffffff);
    }

    User_Choice = 2;

    switch (aRxBuffer[0])
    {
    case 0x31:

      printf("\nProgram with Safety Register Check: \r\n");
      M95P32_PageProg_with_SafetyRegisterCheck();
      break;

    case 0x32:

      printf("\nWrite with Safety Register Check: \r\n");
      M95P32_PageWrite_with_SafetyRegisterCheck();
      break;

    case 0x33:

      printf("\nBuffer Mode: \r\n");
      M95P32_PageProgram_with_BufferLoad();
      break;

    default:
      printf("\nReturning to main menu \r\n");
      User_Choice = 0;
      IntroScreen();
      break;
    }

    if(User_Choice != 0)
    {
      User_Scenario_IntroScreen();
    }

    UartReady = SET;
    HAL_NVIC_EnableIRQ(USARTX_INTERRUPT);
    memset(&aRxBuffer, 0x00, RXBUFFERSIZE);
    if(HAL_UART_Receive_IT(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
      App_Error_Handler();
    }
  }

  /* Tips and Tricks option selected */
  if (IT_receive == 3)
  {
    IT_receive = 0;

    if(User_Choice != 3)
    {
      HAL_UART_Receive(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1, 0xffffff);
    }

    User_Choice = 3;

    switch (aRxBuffer[0])
    {
    case 0x31:

      printf("\n16 word align: \r\n");
      M95P32_16Word_align();

      /* erase block 0 */
      PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
      PGEEZ1_M95P32_EraseBlock(PGEEZ1_M95P32_0, TARGET_ADDRESS_00);

      break;

    case 0x32:

      printf("\nWrite in protected area \r\n");
      M95P32_WriteProtectedArea();
      break;

    default:
      printf("\nReturning to main menu \r\n");
      User_Choice = 0;
      IntroScreen();
      break;
    }

    if(User_Choice != 0)
    {
      Specific_Behaviour_IntroScreen();
    }

    UartReady = SET;
    HAL_NVIC_EnableIRQ(USARTX_INTERRUPT);
    memset(&aRxBuffer, 0x00, RXBUFFERSIZE);
    if(HAL_UART_Receive_IT(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
      App_Error_Handler();
    }
  }

  if (IT_receive == 4)
  {
    IT_receive = 0;

    int32_t status = M95_OK;
    /* Test Write Enable and Write Disable*/
    status = M95P32_TestWriteEnableDisable();
    if(status != M95_OK)
    {
      printf("\t**************Test 1 Failed**************\r\n\n");
      App_Error_Handler();
    }
    else
    {
      printf("\t**************Test 1 Passed**************\r\n\n");
    }

    /* Test Page Read and Write Operation*/
    status = M95P32_TestPage();
    if(status != M95_OK)
    {
      printf("\t**************Test 2 Failed**************\r\n\n");
      App_Error_Handler();
    }
    else
    {
      printf("\t**************Test 2 Passed**************\r\n\n");
    }

    /* Test Page Erase Operation*/
    status = M95P32_TestPageErase();
    if(status != M95_OK)
    {
      printf("\t**************Test 3 Failed**************\r\n\n");
      App_Error_Handler();
    }
    else
    {
      printf("\n\n\r Page Erase				: Passed\r\n");
      printf("\t**************Test 3 Passed**************\r\n\n");
    }

    /* Test Sector Erase Operation*/
    status = M95P32_TestSectorErase();
    if(status != M95_OK)
    {
      printf("\t**************Test 4 Failed**************\r\n\n");
      App_Error_Handler();
    }
    else
    {
      printf("\n\n\r Sector Erase				: Passed\r\n");
      printf("\t**************Test 4 Passed**************\r\n\n");
    }

    /* Test Block Erase Operation*/
    status = M95P32_TestBlockErase();
    if(status != M95_OK)
    {
      printf("\t**************Test 5 Failed**************\r\n\n");
      App_Error_Handler();
    }
    else
    {
      printf("\n\n\r Block Erase				: Passed\r\n");
      printf("\t**************Test 5 Passed**************\r\n\n");
    }

    /* Test Buffer Mode Operation*/
    status = M95P32_TestBufferMode();
    if(status != M95_OK)
    {
      printf("\t**************Test 6 Failed**************\r\n\n");
    }
    else
    {
      printf("\n\n\r Buffer Mode				: Passed\r\n");
      printf("\t**************Test 6 Passed**************\r\n\n");
    }

    if(status != M95_OK)
    {
      App_Error_Handler();
    }

    /*Erase memory*/
    PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
    PGEEZ1_M95P32_EraseChip(PGEEZ1_M95P32_0);

    IntroScreen();

    UartReady = SET;
    HAL_NVIC_EnableIRQ(USARTX_INTERRUPT);
    if(HAL_UART_Receive_IT(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
      App_Error_Handler();
    }

  }
  /* USER CODE END WHILE */

}

/**
  * @brief  This function initiates all the test cases and print the test status
  *         whether pass or fail
  * @param  None
  * @retval None
  */
void MX_EEPROM_RW_Init(void)
{
  BSP_COM_Init(COM1);
  while( PGEEZ1_M95P32_Init(PGEEZ1_M95P32_0)!= M95_OK);

  if(HAL_UART_Receive_IT(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler();
  }
  UartReady = SET;

}

/**
  * @brief  Test Case to Verify Write Disable and Write Enable
  * @param  None
  * @retval BSP status
  */
int32_t M95P32_TestWriteEnableDisable(void)
{

  /*
  Test Case to Verify Read Status Register, Write Enable and Write Disable

  __________Steps_____________
  *Read Status Register: If value is 00 then passed

  *Write Enable
  *Read Status Register: If value is 02 then passed

  *Write Disable
  *Read Status Register: If value is 00 then passed
  */

  int32_t status;

  printf(" \r\n ");
  printf("\t\t______Test 1 Starts______\r\n");
  printf("\n\tTest case to verify Write Enable and Write Disable\r\n");

  /*Reading default value of Status register*/
  status = Test_ReadStatusReg(PGEEZ1_M95P32_0);
  if(status != M95_OK)
  {
    return status;
  }
  HAL_Delay(5);

  /*Reading value of status register after Write Enable*/
  status = WriteEnableReadStatus(PGEEZ1_M95P32_0);
  if(status != M95_OK)
  {
    return status;
  }
  HAL_Delay(5);

  /*Reading value of status register after Write Disable*/
  status = WriteDisableReadStatus(PGEEZ1_M95P32_0);
  if(status != M95_OK)
  {
    return status;
  }
  HAL_Delay(5);
  return status;
}

/**
  * @brief  Read status register(EEPROMEX_RDSR) test examples for SPI_EEPROM memory
  * @param  TargetName : SPI_EEPROM memory name to write
  * @retval BSP status
  */
int32_t Test_ReadStatusReg(uint32_t TargetName)
{
  uint8_t val_status_reg;
  int32_t readstatus;

  readstatus = PGEEZ1_M95P32_StatusRegRead(TargetName, &val_status_reg);

  if(readstatus != M95_OK)
  {
    return readstatus;
  }
  else
  {
    PRINTF_APPLI("__READ_STATUS_REG_DEFAULT_\r\n");

    if(val_status_reg == 00U)
    {
      printf("%2.2x     : Passed \r\n", val_status_reg);
    }
    else
    {
      printf("%2.2x     : Failed \r\n", val_status_reg);
    }
  }

  return readstatus;
}

/**
  * @brief  Write Disable(WRDI) test examples for SPI_EEPROM memory
  * @param  TargetName : SPI_EEPROM memory name to write
  * @retval BSP status
  */
int32_t WriteDisableReadStatus(uint32_t TargetName)
{
  uint8_t val_status_reg;
  int32_t readstatus;

  PGEEZ1_M95P32_WriteDisable(TargetName);

  readstatus = PGEEZ1_M95P32_StatusRegRead(TargetName,&val_status_reg);

  if(readstatus != M95_OK)
  {
    return readstatus;
  }
  else
  {
    PRINTF_APPLI("__READ_STATUS_REG_AFTER_WRITE_DISABLE\r\n");

    if(val_status_reg == 00U)
    {
      printf("%2.2x     : Passed \r\n", val_status_reg);
    }
    else
    {
      printf("%2.2x     : Failed \r\n", val_status_reg);
    }

  }

  return readstatus;
}

/**
  * @brief  Write Enable(WREN) test examples for SPI_EEPROM memory
  * @param  TargetName : SPI_EEPROM memory name to write
  * @retval BSP status
  */
int32_t WriteEnableReadStatus(uint32_t TargetName)
{
  uint8_t val_status_reg;
  int32_t readstatus;

  PGEEZ1_M95P32_WriteEnable(TargetName);

  readstatus = PGEEZ1_M95P32_StatusRegRead(TargetName, &val_status_reg);

  if(readstatus != M95_OK)
  {
    return readstatus;
  }
  else
  {
    PRINTF_APPLI("__READ_STATUS_REG_AFTER_WRITE_ENABLE\r\n");

    if(val_status_reg == 02U)
    {
      printf("%2.2x     : Passed \r\n", val_status_reg);
    }
    else
    {
      printf("%2.2x     : Failed \r\n", val_status_reg);
    }
  }

  return readstatus;
}

/**
  * @brief  Tests page write and read functionality for SPI_EEPROM memory
  * @param  None
  * @retval BSP status
  */
int32_t M95P32_TestPage(void)
{
  /*
  Test case to verify Page Write and Read
  __________Steps_____________
  *Write Enable
  *Writing first 100 bytes of a page to 0x0f at address: 0x00 (Page Number-1)
  *Verifying if bytes of page are updated to 0x0f or not
  *Reading 200 Bytes of page at address: 0x00 using Single SPI
  *Reading 200 Bytes of page at address: 0x00 using Fast Read Single SPI
  *Reading 200 Bytes of page at address: 0x00 using Fast Read Dual SPI
  *Reading 200 Bytes of page at address: 0x00 using Fast Read Quad SPI
  */

  unsigned int index;

  memset(&SampleData, 0x0F, SIZE100);

  printf("\t\t______Test 2 Starts______\r\n");
  printf("\tTest case to verify Page Write and Read\r\n");

  /* Write Enable*/
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PRINTF_APPLI(" \n\r Write Page (100 Bytes) at Address 0x00  \n\r");

  /*Writing first 100 bytes of a page to 0x0f at address: 0x00 (Page Number-1)*/
  PGEEZ1_M95P32_WritePage(PGEEZ1_M95P32_0, SampleData, TARGET_ADDRESS_00, SIZE100);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReadBuff, TARGET_ADDRESS_00, SIZE200);

  /*Verifying if bytes of page are updated to 0x0f or not*/
  for(index = 0; index < SIZE200; index++)
  {
    if(ReadBuff[index] == SampleData[index])
      continue;
    else
      break;  /* If first 100 bytes are 0x0F, loop breaks at index 100 */
  }

  if(index == SIZE100)
  {
    printf("\n\n\r Write Page				: Passed\r\n");
  }
  else
  {
    printf("\n\n\r Write Page				: Failed\r\n");
    return M95_ERROR;
  }
  printf("\n");
  memset(&ReadBuff, 0x00, SIZE200);

  /*Reading 200 Bytes of page at address: 0x00 using Single SPI*/
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReadBuff, TARGET_ADDRESS_00, SIZE200);
  for(index = 0; index < SIZE200; index++)
  {
    if(ReadBuff[index] == SampleData[index])
      continue;
    else
      break;  /* If first 100 bytes are 0x0F, loop breaks at index 100 */
  }
  if(index == SIZE100)
  {
    printf("\n\r Read data in Single SPI		: Passed\r\n");
    for(index = 0; index < SIZE200; index++)
    {
      printf("  %2.2x",ReadBuff[index]);
    }
  }
  else
  {
    printf("\n\r Read data in Single SPI		: Failed\r\n");
    return M95_ERROR;
  }
  printf("\n");
  memset(&ReadBuff, 0x00, SIZE200);

  /*Reading 200 Bytes of page at address: 0x00 using Fast Read Single SPI*/
  PGEEZ1_M95P32_FastRead(PGEEZ1_M95P32_0, ReadBuff, TARGET_ADDRESS_00, SIZE200);

  for(index = 0; index < SIZE200; index++)
  {
    if(ReadBuff[index] == SampleData[index])
      continue;
    else
      break;  /* If first 100 bytes are 0x0F, loop breaks at index 100 */
  }

  if(index == SIZE100)
  {
    printf("\n\r Read data Fast in Single SPI		: Passed\r\n");
    for(index = 0; index < SIZE200; index++)
    {
      printf("  %2.2x",ReadBuff[index]);
    }
  }
  else
  {
    printf("\n\r Read data Fast in Single SPI		: Failed\r\n");
    return M95_ERROR;
  }
  #ifdef USE_QUADSPI
  printf("\n");
  memset(&ReadBuff, 0x00, SIZE200);

  /*Reading 200 Bytes of page at address: 0x00 using Fast Read Dual SPI*/
  PGEEZ1_M95P32_FastDRead(PGEEZ1_M95P32_0, ReadBuff, TARGET_ADDRESS_00, SIZE200);
  for(index = 0; index < SIZE200; index++)
  {
    if(ReadBuff[index] == SampleData[index])
      continue;
    else
      break;  /* If first 100 bytes are 0x0F, loop breaks at index 100 */
  }

  if(index == SIZE100)
  {
    printf("\n\r Read data Fast in Dual SPI		: Passed\r\n");
    for(index = 0; index < SIZE200; index++)
    {
      printf("  %2.2x",ReadBuff[index]);
    }
  }
  else
  {
    printf("\n\r Read data Fast in Dual SPI		: Failed\r\n");
    return M95_ERROR;
  }
  printf("\n");
  memset(&ReadBuff, 0x00, SIZE200);

  /*Reading 200 Bytes of page at address: 0x00 using Fast Read Quad SPI*/
  PGEEZ1_M95P32_FastQRead(PGEEZ1_M95P32_0, ReadBuff, TARGET_ADDRESS_00, SIZE200);
  for(index = 0; index < SIZE200; index++)
  {
    if(ReadBuff[index] == SampleData[index])
      continue;
    else
      break;  /* If first 100 bytes are 0x0F, loop breaks at index 100 */
  }

  if(index == SIZE100)
  {
    printf("\n\r Read data Fast in Quad SPI		: Passed\r\n");
    for(index = 0; index < SIZE200; index++)
    {
      printf("  %2.2x",ReadBuff[index]);
    }
  }
  else
  {
    printf("\n\r Read data Fast in Quad SPI		: Failed\r\n");
    return M95_ERROR;
  }
  #endif
  printf("\n");
  memset(&ReadBuff, 0x00, SIZE200);
  return M95_OK;
}

/**
  * @brief  Tests page erase functionality for SPI_EEPROM memory
  * @param  None
  * @retval BSP status
  */
int32_t M95P32_TestPageErase(void)
{

  /*
  Test Case to Verify Page Erase
  __________Steps_____________
  *Write Enable
  *Erasing page (512 Byte) at address 0x00
  *Verifying that after Page erase all bytes are at their default value of 0xff
  */

  uint32_t index;
  uint8_t a_rcvbuff_page_erase[SIZE200] = {0};

  printf("\n\t\t______Test 3 Starts______\r\n\n");
  printf("\t  Test Case to verify Page Erase\r\n");

  /*Write Enable*/
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  PRINTF_APPLI("\n *PAGE_ERASE* at Address 0x00 \r\n");
  PGEEZ1_M95P32_ErasePage(PGEEZ1_M95P32_0, TARGET_ADDRESS_00);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, a_rcvbuff_page_erase, TARGET_ADDRESS_00, SIZE200);

  for(index = 0; index < SIZE200; index++)
  {

    if(a_rcvbuff_page_erase[index] != 0xFFU)
      return M95_ERROR;

  }

  printf(" Data after Page Erase \r\n");
  for(index = 0; index < SIZE200; index++)
  {
    printf("  %2.2x", a_rcvbuff_page_erase[index]);
  }
  printf("\n");
  return M95_OK;
}

/**
  * @brief  Tests sector erase functionality for SPI_EEPROM memory
  * @param  None
  * @retval BSP status
  */
int32_t M95P32_TestSectorErase(void)
{
  /*
  Test Case to Verify Sector Erase
  __________Steps_____________
  *Write Enable
  *Writing first 100 bytes of a page to 0x0f at address: 0x400 (Page Number-3)
  *Write Enable
  *Erasing sector (4Kbyte = 8 Pages) at address 0x00*
  *Verifying that after sector erase all bytes of 8 pages are at their default value of 0xff
  */
  printf("\n\t\t______Test 4 Starts______\r\n");
  printf("\t  Test Case to verify Sector Erase\r\n");

  uint32_t index;
  uint8_t a_rcvbuff_sector_erase[SIZE200] = {0};

  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PRINTF_APPLI("\n *Sector_ERASE* at Address 0x00 \r\n");

  memset(&SampleData, 0x0F, SIZE200);

  PGEEZ1_M95P32_WritePage(PGEEZ1_M95P32_0, SampleData, TARGET_ADDRESS_0x400, SIZE200);

  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PGEEZ1_M95P32_EraseSector(PGEEZ1_M95P32_0, TARGET_ADDRESS_0x400);

  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, a_rcvbuff_sector_erase, TARGET_ADDRESS_0x400, SIZE200);

  for(index = 0; index < SIZE200; index++)
  {

    if(a_rcvbuff_sector_erase[index] != 0xFFU)
      return M95_ERROR;
  }

  printf(" Data after Sector Erase \r\n");
  for(index = 0; index < SIZE200; index++)
  {
    printf("  %2.2x",a_rcvbuff_sector_erase[index]);
  }
  printf("\n");
  return M95_OK;

}

/**
  * @brief  Tests block erase functionality for SPI_EEPROM memory
  * @param  None
  * @retval BSP status
  */
int32_t M95P32_TestBlockErase(void)
{
  /*
  Test Case to Verify Block Erase
  __________Steps_____________
  *Write Enable
  *Writing first 100 bytes of a page to 0x0f at address: 0x1800 (Page Number-10)
  *Write Enable
  *Erasing Block (64Kbyte = 128 Pages) at address: 0x00
  *Verifying that after block erase all bytes of 128 pages are at their default value of 0xff
  */

  printf("\n\t\t______Test 5 Starts______\r\n\n");
  printf("\t  Test Case to verify Block Erase\r\n");

  uint32_t index;
  uint8_t a_rcvbuff_block_erase[SIZE200] = {0};

  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PRINTF_APPLI(" *Block_ERASE* at Address 0x00 \r\n");

  memset(&SampleData, 0x0F, 0x64);

  PGEEZ1_M95P32_WritePage(PGEEZ1_M95P32_0, SampleData, TARGET_ADDRESS_0x1800, SIZE200);

  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PGEEZ1_M95P32_EraseBlock(PGEEZ1_M95P32_0, TARGET_ADDRESS_0x1800);

  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, a_rcvbuff_block_erase, TARGET_ADDRESS_0x1800, SIZE200);

  for(index = 0; index < SIZE200; index++)
  {

    if(a_rcvbuff_block_erase[index] != 0xFFU)
      return M95_ERROR;
  }

  printf(" Data after Block Erase \r\n");
  for(index = 0; index < SIZE200; index++)
  {
    printf("  %2.2x",a_rcvbuff_block_erase[index]);
  }
  return M95_OK;
}

/**
  * @brief  Programs specific Page with checks on safety register
  * @param  None
  * @retval None
  */
void M95P32_PageProg_with_SafetyRegisterCheck()
{
  uint8_t sample_data;
  uint8_t status_reg_val = 0;
  uint32_t index = 0;

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);
  printf("Power UP Done.\r\n");

  /* Polling on Safety register Bit6 Power Up Flag (PUF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  while((ReceiveBuff[1] & 0x40 ) != 0); /* ReceiveBuff[1] corresponds to Safety Register */
  printf("Power UP OK.\r\n");

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Erase Page */
  PGEEZ1_M95P32_ErasePage(PGEEZ1_M95P32_0, TARGET_ADDRESS_00);

  /* Pollling on Status register Bit0 WIP */
  status_reg_val = 0;
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);
  printf("Page Erase Done.\r\n");

  /* Check Safety register Bit5 Erase Flag (ERF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x20) == 0)
  {
    printf("Page Erase Done without issues.\r\n");

  }
  else
  {
    printf("Page Erase NOT DONE.\r\n");
  }

  /* Check page erased successfully by reading data*/
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0, M95P32_PAGE_SIZE);

  for(index = 0; index < M95P32_PAGE_SIZE; index++)
  {
    if(ReceiveBuff[index] != 0xFF)
    {
      break;
    }
  }
  if (index != M95P32_PAGE_SIZE)
  {
    printf("Page NOT Erased Correctly.\r\n");
  }

  else
  {
    printf("Page Erased Correctly.\r\n");
  }

  /* Read Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("NO ECC detected during Erase Check.\r\n");
  }
  else
  {
    printf("ECC detected during Erase Check.\r\n");
  }

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Page Program*/

#ifdef USE_QUADSPI
  memset(&pData, 0x0F, SIZE600);
#else
  memset(&pData, 0x55, SIZE600);
#endif

  add = 0;
  PGEEZ1_M95P32_ProgramPage(PGEEZ1_M95P32_0, pData, add, M95P32_PAGE_SIZE);

  /* Check Safety register Bit4 Program Flag (PRF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x10) == 0)
  {
    printf("Page Program Done without issues.\r\n");

  }
  else
  {
    printf("Page Program NOT DONE.\r\n");
  }

  /* Check page prgrammed successfully by reading data*/
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0, M95P32_PAGE_SIZE);

#ifdef USE_QUADSPI
  sample_data = 0x0F;
#else
  sample_data = 0x55;
#endif

  for(index = 0; index < M95P32_PAGE_SIZE; index++)
  {
    if(ReceiveBuff[index] != sample_data)
    {
      break;
    }
  }
  if (index != M95P32_PAGE_SIZE)
  {
    printf("Page NOT Programmed Correctly.\r\n");
  }

  else
  {
    printf("Page Programmed Correctly.\r\n");
  }

  /* Read Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("NO ECC detected during Page Program Check.\r\n");
  }
  else
  {
    printf("ECC detected during Page Program Check.\r\n");
  }

}

/**
  * @brief  Programs specific Page with checks on safety register
  * @param  None
  * @retval None
  */
void M95P32_PageWrite_with_SafetyRegisterCheck()
{
  uint8_t status_reg_val = 0;
  uint32_t index = 0;

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);
  printf("Power UP Done.\r\n");

  /* Polling on Safety register Bit6 Power Up Flag (PUF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  while((ReceiveBuff[1] & 0x40 ) != 0); /* ReceiveBuff[1] corresponds to Safety Register */
  printf("Power UP OK.\r\n");

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Page Write with data 0x56 */
  memset(&pData, 0x56, SIZE600);

  add = 0;
  PGEEZ1_M95P32_WritePage(PGEEZ1_M95P32_0, pData, add, M95P32_PAGE_SIZE);

  /* Check Safety register Bit5 Erase Flag (ERF) and Bit4 Program Flag (PRF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x30) == 0)
  {
    printf("Page Write Done without issues.\r\n");

  }
  else
  {
    printf("Page Write NOT DONE.\r\n");
  }

  /* Check page write successfully by reading data*/
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0, M95P32_PAGE_SIZE);

  for(index = 0; index < M95P32_PAGE_SIZE; index++)
  {
    if(ReceiveBuff[index] != 0x56)
    {
      break;
    }
  }
  if (index != M95P32_PAGE_SIZE)
  {
    printf("Page NOT Written Correctly.\r\n");
  }

  else
  {
    printf("Page Written Correctly.\r\n");
  }

  /* Read Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("NO ECC detected during Page Write Check.\r\n");
  }
  else
  {
    printf("ECC detected during Page Write Check.\r\n");
  }

  /* Page Write with data 0xA9 */
  memset(&pData, 0xA9, SIZE600);

  add = 0;
  PGEEZ1_M95P32_WritePage(PGEEZ1_M95P32_0, pData, add, M95P32_PAGE_SIZE);

  /* Check Safety register Bit5 Erase Flag (ERF) and Bit4 Program Flag (PRF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x30) == 0)
  {
    printf("Page Write Done without issues.\r\n");

  }
  else
  {
    printf("Page Write NOT DONE.\r\n");
  }

  /* Check page write successfully by reading data*/
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0, M95P32_PAGE_SIZE);

  for(index = 0; index < M95P32_PAGE_SIZE; index++)
  {
    if(ReceiveBuff[index] != 0xA9)
    {
      break;
    }
  }
  if (index != M95P32_PAGE_SIZE)
  {
    printf("Page NOT Written Correctly.\r\n");
  }

  else
  {
    printf("Page Written Correctly.\r\n");
  }

  /* Read Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("NO ECC detected during Page Write Check.\r\n");
  }
  else
  {
    printf("ECC detected during Page Write Check.\r\n");
  }

}

/**
  * @brief  Page Program with Buffer Load
  * @param  None
  * @retval None
  */
void M95P32_PageProgram_with_BufferLoad()
{
  uint8_t status_reg_val = 0;

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);
  printf("Power UP Done.\r\n");

  /* Polling on Safety register Bit6 Power Up Flag (PUF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  while((ReceiveBuff[1] & 0x40 ) != 0); /* ReceiveBuff[1] corresponds to Safety Register */
  printf("Power UP OK.\r\n");

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Sector Erase */
  PGEEZ1_M95P32_EraseSector(PGEEZ1_M95P32_0, TARGET_ADDRESS_00);

  /* Read Safety register to ensure no issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if(ReceiveBuff[1] == 0)
  {
    printf("Safety Register status OK.\r\n");
  }
  else
  {
    printf("Safety Register status NOT OK.\r\n");
  }

  /* Buffer Mode */

  uint32_t start_prog_add = 0;
  uint32_t nb_byte = SIZE600;

  memset(&pData, 0x89, SIZE600);
  if(nb_byte > SIZE600)
  {
    printf("Data buffer size is greater than allocated memory, Aborted!\r\n");
  }

  else
  {
    /* write from start_prog_add 0x00, data 0x89 and size 600 nb_byte */
    PageProgram_with_BufferLoad(PGEEZ1_M95P32_0, pData,  start_prog_add,  nb_byte);
  }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void M95P32_16Word_align()
{
  uint8_t status_reg_val = 0;
  uint32_t index;

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);
  printf("Power UP Done.\r\n");

  /* Polling on Safety register Bit6 Power Up Flag (PUF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0xF0 ) == 0) /* ReceiveBuff[1] corresponds to Safety Register */
  {
    printf("Power UP OK.\r\n");
  }
  else
  {
    printf("Power UP NOT OK.\r\n");
  }

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Page Erase starting from address 0x00 */
  PGEEZ1_M95P32_ErasePage(PGEEZ1_M95P32_0, 0x00);

  /* Check Safety register to ensure no erase issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Page Erased from address 0x00. Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0xA0) == 0)
  {
    printf("PAMAF and ERF bit not set. OK.\r\n");
  }
  else
  {
    printf("Error ! PAMAF or ERF bit is set. NOT OK! \r\n");
  }

  /* Check data after page erase operation */

  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0x00, M95P32_PAGESIZE);

  for(index = 0; (ReceiveBuff[index] == 0xff) && (index < M95P32_PAGESIZE); index++){}

  if(index == M95P32_PAGESIZE)
  {
    printf("Erased page data is in erased state <0xFF>. OK.\r\n");
  }
  else
  {
    printf("Erased page data is NOT in erased state. Abort!\r\n");
    return;
  }

  /* Check Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("No ECC flag set. OK.\r\n\n");
  }
  else
  {
    printf("ECC bits are set. NOT OK!\r\n");
  }

  /* Page Program 1 byte on first page address */

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  memset(&pData, 0x55, SIZE600);
  PGEEZ1_M95P32_ProgramPage(PGEEZ1_M95P32_0, pData, 0, 1);

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);

  /* Check Safety register to ensure no issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Programmed 1 byte from address 0x00 with value 0x55. Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0xF0) == 0)
  {
    printf("No flag set. OK.\r\n");
  }
  else
  {
    printf("Flag set. NOT OK!\r\n");
  }

  /* READ single byte */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0x00, 1);

  /* Check Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Read 1 byte from address 0x00. Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("No ECC flag set. OK.\r\n\n");
  }
  else
  {
    printf("ECC Flag set. NOT OK!\r\n");
  }

  /* Page Program 1 byte on 16th byte of first page address */

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  memset(&pData, 0xA5, SIZE600);
  PGEEZ1_M95P32_ProgramPage(PGEEZ1_M95P32_0, pData, 0x100, 1);

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);

  /* Check Safety register to ensure no issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Programmed 1 byte from address 0x100 with value 0xA5. Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0xF0) == 0x00)
  {
    printf("No Flag set. OK.\r\n");
  }
  else
  {
    printf("Flag set. NOT OK!\r\n");
  }

  /* READ single byte */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0x100, 1);
  printf("Data at address 0x100 is: 0x%X \r\n", ReceiveBuff[0]);
  if(ReceiveBuff[0] == 0xA5)
  {
    printf("Page Program successful\r\n");
  }
  else
  {
    printf("Page Program NOT successful\r\n");
  }

  /* Check Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Read 1 byte from address 0x100. Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("No ECC flag set. OK.\r\n\n");
  }
  else
  {
    printf("ECC Flag set. NOT OK!\r\n");
  }

  /* Page Program 1 byte on 17th byte of first page address */

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  memset(&pData, 0x55, SIZE600);
  PGEEZ1_M95P32_ProgramPage(PGEEZ1_M95P32_0, pData, 0x101, 1);

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);

  /* Check Safety register to ensure no issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Programmed 1 byte from address 0x101 with value 0x55. Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0xF0) == 0)
  {
    printf("No flag set. OK.\r\n");
  }
  else
  {
    if((ReceiveBuff[1] & 0x10) == 0x10)
    {
      printf("PRF Flag set as expected!\r\n");
    }
  }

  /* Clear safety flag */
  PGEEZ1_M95P32_ClearSafetyFlag(PGEEZ1_M95P32_0);

  /* READ single byte */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0x101, 1);
  printf("Data at address 0x101 is: 0x%X \r\n", ReceiveBuff[0]);
  if(ReceiveBuff[0] == 0x55)
  {
    printf("Page Program successful\r\n");
  }
  else
  {
    printf("Page Program NOT successful\r\n");
  }

  /* Check Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Safety Register Value is: 0x%x \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("No ECC flag set. OK.\r\n");
  }
  else
  {
    printf("ECC Flag set. NOT OK!\r\n");
  }

}

/**
  * @brief  Attempts write to protected memory area
  * @param  None
  * @retval None
  */
void M95P32_WriteProtectedArea()
{
  uint8_t status_reg_val = 0;

  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);
  printf("Power UP Done.\r\n");

  /* Polling on Safety register Bit6 Power Up Flag (PUF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  while((ReceiveBuff[1] & 0x40 ) != 0); /* ReceiveBuff[1] corresponds to Safety Register */
  printf("Power UP OK.\r\n");

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Write Status register BPx bits to 1 i.e 0x1C in Status register */
  memset(&pData, 0x00, SIZE520);
  pData[0] = 0x1C;
  PGEEZ1_M95P32_StatusConfigRegWrite(PGEEZ1_M95P32_0, pData, 1);

  /* Read status register  */
  PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  printf("Status Register value is: 0x%X\r\n", status_reg_val);
  if(status_reg_val == 0x1C)
  {
    printf("Chip fully protected.\r\n");
  }
  else
  {
    printf("Chip not protected.\r\n");
  }

  /* Read a random data */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0x600, 1);
  printf("Data stored at address 0x600 is 0x%X \r\n", ReceiveBuff[0]);

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Write data 0x87 at 0x600 address*/
  memset(&pData, 0x87, SIZE600);

  add = 0x600;
  PGEEZ1_M95P32_WritePage(PGEEZ1_M95P32_0, pData, add, 1);

  /* Write Disable called explicitly. WEL bit in status register will not reset automatically as previous write was not performed. Chip protected.*/
  PGEEZ1_M95P32_WriteDisable(PGEEZ1_M95P32_0);

  /* Check Safety register Bit7 Protected array modified attempt flag (PAMAF) */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  printf("Attempt to write 0x87 at address 0x600. Safety Register Value is: 0x%X \r\n", ReceiveBuff[1]);
  if((ReceiveBuff[1] & 0x80) == 0x80)
  {
    printf("PAMAF bit set, Modify operation to a protected memory area has been attempted.\r\n");
  }
  else
  {
    printf("Error ! PAMAF bit should be set! \r\n");
  }

  /* Clear Safety Flags*/
  PGEEZ1_M95P32_ClearSafetyFlag(PGEEZ1_M95P32_0);

  /* Read data at address 0x600 */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0x600, 1);
  printf("Data stored at address 0x600 is 0x%X\r\n", ReceiveBuff[0]);

  /* Read Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(PGEEZ1_M95P32_0, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("NO ECC correction detected. OK. \r\n");
  }
  else
  {
    printf("ECC correction detected !\r\n");
  }

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);

  /* Clear BPx bits of Status Register*/
  memset(&pData, 0x00, SIZE520);
  PGEEZ1_M95P32_StatusConfigRegWrite(PGEEZ1_M95P32_0, pData, 1);
  /* Read status register  */
  PGEEZ1_M95P32_StatusRegRead(PGEEZ1_M95P32_0, &status_reg_val);
  printf("Status Register value : 0x%X\r\n", status_reg_val);

}

/**
  * @brief  Tests buffer mode functionality for SPI EEPROM memory
  * @param  Instance : Instance of the SPI EEPROM memory
  * @param  pData : pointer to data buffer
  * @param  start_prog_add : Starting address to write in memory
  * @param  nb_byte : Number of Bytes of data to be written
  * @retval None
  */
void PageProgram_with_BufferLoad(uint32_t Instance, uint8_t *pData, uint32_t start_prog_add, uint32_t nb_byte)
{
  uint32_t result;
  uint32_t start_erase_add;
  uint32_t end_erase_add;
  uint32_t end_prog_add;
  uint32_t top_add_secure;
  uint32_t bot_add_secure;
  uint8_t status_reg_val;
  uint8_t volatile_reg_val;
  uint16_t bytesToWrite;
  uint16_t bytesToRead;
  uint32_t remainingSize;
  uint16_t offset;
  unsigned int targetAddress;
  uint8_t *pbuffdata;
  uint16_t index;
  uint8_t BP_bits;
  uint8_t TB_bit;

  /* Erase Check */

  /* start_erase_add calculation*/
  result = start_prog_add % 16;
  if(result == 0) /* check address is word aligned */
  {
    start_erase_add = start_prog_add;
  }

  else
  {
    start_erase_add = start_prog_add - result;
  }

  /* end_erase_add calculation*/
  end_prog_add = start_prog_add + nb_byte - 1;
  result = end_prog_add % 15;

  if(result == 0) /* check address is word aligned */
  {
    end_erase_add = end_prog_add;
  }

  else
  {
    end_erase_add = end_prog_add +(16 - result);
  }

  /* Read status register to check if area is protected */
  PGEEZ1_M95P32_StatusRegRead(Instance, &status_reg_val);
  BP_bits = (status_reg_val & 0x1C) >> 2;
  TB_bit = (status_reg_val & 0x40) >> 6;

  if(BP_bits == 0)
  {
    /* no protection, continue S*/
  }
  else if(BP_bits == 7)
  {
    /* memory fully protected, abort */
    return;
  }
  else if ((BP_bits > 0) && (BP_bits < 7) && (TB_bit == 0))
  {
    /* memory protected from top, calculate bot_add_secure */
    bot_add_secure = (M95P32_BLOCK_SIZE * (M95P32_BLOCK_COUNT - (2^(BP_bits - 1))));
    if(end_erase_add >= bot_add_secure)
    {
      /* algorithm aborted because area to program is protected */
      return;
    }

  }
  else if ((BP_bits > 0) && (BP_bits < 7) && (TB_bit == 1))
  {
    /* memory protected from bottom, calculate top_add_secure */
    top_add_secure = (M95P32_BLOCK_SIZE * (2^(BP_bits - 1))) - 1;
    if(start_erase_add <= top_add_secure)
    {
      /* algorithm aborted because area to program is protected */
      return;
    }

  }
  else
  {
    /* nothing to do*/
  }

  /* Check if area is already erased */

  remainingSize = nb_byte;
  offset = start_prog_add % M95P32_PAGESIZE;
  targetAddress = start_prog_add;

  /* Iterate over the pages and READ the data */
  while (remainingSize > 0)
  {
    bytesToRead = (remainingSize < M95P32_PAGE_SIZE - offset) ? remainingSize : (M95P32_PAGE_SIZE - offset);

    /* Perform the READ operation */

    memset(&ReceiveBuff, 0x00, SIZE1024);
    PGEEZ1_M95P32_ReadPage(Instance, ReceiveBuff, targetAddress, bytesToRead);

    /* Check programmed data */
    for(index = 0; (ReceiveBuff[index] == 0xff) && (index < bytesToRead); index++)
    {
      PRINTF_APPLI(" Address: 0x%x  Data: 0x%x. \r\n", targetAddress, ReceiveBuff[index]);
      targetAddress++;
    }
    if(index == bytesToRead)
    {
      printf("Data is in erased state. OK.\r\n");
    }
    else
    {
      printf("Data is NOT in erased state. Abort!\r\n");
      return;
    }

    /* Update the pointers and sizes for the next page */

    remainingSize -= bytesToRead;
    offset = targetAddress % M95P32_PAGE_SIZE;

  }

  /* Read Safety register to ensure no ECC issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(Instance, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("NO ECC correction detected. OK.\r\n");
  }
  else
  {
    printf("ECC correction detected !\r\n");
  }

  /* Check start and end address page alignment */
  if(start_prog_add % M95P32_PAGE_SIZE != 0)
  {
    printf("First Page program is managed outside of an array.\r\n");
  }

  if(end_prog_add % 511 != 0)
  {
    printf("Last Page program is managed outside of an array.\r\n");
  }

  /* Setup Buffer load */

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(Instance);

  /* Write volatile register with value 0x02 to set BUFFEN bit to 1 */
  PGEEZ1_M95P32_VolRegWrite(Instance, 0x02);

  /* Program in Buffer Mode */

  remainingSize = nb_byte;
  offset = start_prog_add % M95P32_PAGESIZE;
  targetAddress = start_prog_add;
  pbuffdata = pData;

  /* Write Enable only once*/
  PGEEZ1_M95P32_WriteEnable(Instance);

  /* Iterate over the pages and write the data */
  while (remainingSize > 0)
  {
    bytesToWrite = (remainingSize < M95P32_PAGE_SIZE - offset) ? remainingSize : (M95P32_PAGE_SIZE - offset);

    /* Perform the write operation */

    PGEEZ1_M95P32_ProgramPage(Instance, pbuffdata, targetAddress, bytesToWrite);

    /* Read volatile register to check buffer loading status BUFLD bit */
    do
    {
      PGEEZ1_M95P32_VolRegRead(Instance, &volatile_reg_val);
    }
    /* Poll until BUFLD bit is 0 */
    while((volatile_reg_val & 0x01) != 0);

    /* Update the pointers and sizes for the next page */
    pbuffdata += bytesToWrite;
    targetAddress += bytesToWrite;
    remainingSize -= bytesToWrite;
    offset = targetAddress % M95P32_PAGE_SIZE;

  }

  /* Final polling */
  /* Pollling on Status register Bit0 WIP */
  do
  {
    PGEEZ1_M95P32_StatusRegRead(Instance, &status_reg_val);
  }
  while((status_reg_val & 0x01) != 0);

  /* Read Safety register to ensure no issue */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(Instance, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x70) == 0)
  {
    printf("Safety Register status OK.\r\n");
  }
  else
  {
    printf("Safety Register status NOT OK.\r\n");
    return; /* abort */
  }

  /* Write Enable */
  PGEEZ1_M95P32_WriteEnable(Instance);

  /* Clear BUFFEN bit to 0 in volatile register */
  PGEEZ1_M95P32_VolRegWrite(Instance, 0x01);

  /* Read volatile register to check BUFFEN bit is 0 */
  PGEEZ1_M95P32_VolRegRead(Instance, &volatile_reg_val);
  if((volatile_reg_val & 0x02) == 0)
  {
    printf("BUFFEN bit is 0. OK.\r\n");
  }
  else
  {
    printf("BUFFEN bit is 1. NOT OK !\r\n");
  }

  /* Check programmed data with a READ from start_prog_add to end_prog_add */

  remainingSize = nb_byte;
  offset = start_prog_add % M95P32_PAGESIZE;
  targetAddress = start_prog_add;
  pbuffdata = pData;

  /* Iterate over the pages and READ the data */
  while (remainingSize > 0)
  {
    bytesToRead = (remainingSize < M95P32_PAGE_SIZE - offset) ? remainingSize : (M95P32_PAGE_SIZE - offset);

    /* Perform the READ operation */

    memset(&ReceiveBuff, 0x00, SIZE1024);
    PGEEZ1_M95P32_ReadPage(Instance, ReceiveBuff, targetAddress, bytesToRead);

    /* Check programmed data */
    for(index = 0; (pbuffdata[index] == ReceiveBuff[index]) && (index < bytesToRead); index++)
    {
      PRINTF_APPLI(" Address: 0x%x  Data: 0x%x.\r\n", targetAddress, ReceiveBuff[index]);
      targetAddress++;
    }
    if(index == bytesToRead)
    {
      printf("Data programmed correctly.\r\n");
    }
    else
    {
      printf("Data programmed is NOT correct!\r\n");
    }

    /* Update the pointers and sizes for the next page */
    pbuffdata += bytesToRead;
    remainingSize -= bytesToRead;
    offset = targetAddress % M95P32_PAGE_SIZE;

  }

  /* READ safety register to check ECC bits */
  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ConfSafetyRegRead(Instance, ReceiveBuff, 2);
  if((ReceiveBuff[1] & 0x0F) == 0)
  {
    printf("NO ECC correction detected. OK.\r\n");
  }
  else
  {
    printf("ECC correction detected !\r\n");
  }

}

/**
  * @brief  Tests buffer mode functionality for SPI_EEPROM memory
  * @param  None
  * @retval BSP status
  */
int32_t M95P32_TestBufferMode(void)
{
  /*
  Test Case to Verify Buffer Mode
  __________Steps_____________
  *Write Enable
  *Write volatile Register to Enable Buffer Mode (0x81 02)
  *Write Enable
  *Programming 1024 bytes of a page to 0x09 starting from address: 0x00
  *Write Volatile Register to exit Buffer Mode
  *Verifying that after Page Program in buffer mode the 1024 bytes from address 0x00 have value 0x09
  */

  printf("\n\t\t______Test 6 Starts______\r\n\n");
  printf("\t  Test Case to verify Page Program in Buffer Mode\r\n");

  uint32_t index;

  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PRINTF_APPLI(" *Write Volatile Register to enter Buffer Mode* \r\n");
  PGEEZ1_M95P32_VolRegWrite(PGEEZ1_M95P32_0, SET_BUFEN_BIT);

  memset(&ReceiveBuff, 0x09, SIZE1024);
  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PGEEZ1_M95P32_PageProgBuffer(PGEEZ1_M95P32_0, ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_VolRegWrite(PGEEZ1_M95P32_0, SET_BUFLD_BIT);

  memset(&ReceiveBuff, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(PGEEZ1_M95P32_0, ReceiveBuff, 0x00, SIZE1024);

  for(index = 0x00; index < SIZE1024; index++)
  {

    if(ReceiveBuff[index] != 0x09U)
      return M95_ERROR;
  }
  printf(" \nData after Programming 1024 bytes starting from address 0x00 in buffer mode \r\n");
  for(index = 0x100U; index < 0x300U; index++)
  {
    printf("  %2.2x",ReceiveBuff[index]);
  }

  PGEEZ1_M95P32_WriteEnable(PGEEZ1_M95P32_0);
  PGEEZ1_M95P32_EraseSector(PGEEZ1_M95P32_0, TARGET_ADDRESS_00);
  return M95_OK;
}

/**
  * @brief Function to show introduction screen
  * @param None
  * @retval None
  */
void IntroScreen(void)
{
    UARTConsolePrint("\r\n"
                       "**********************************\r\n"
                       "Main Menu \r\n"
                       "**********************************\r\n"
                       "Command Set            : 1\r\n"
                       "User Scenario          : 2\r\n"
                       "Specific Behaviour     : 3\r\n"
                       "Run Test Cases         : 4\r\n\n"
                       "**********************************\r\n\n"
                       "Enter Command Number   :\r\n");

}

/**
  * @brief Function to show User Scenario introduction screen
  * @param None
  * @retval None
  */
void User_Scenario_IntroScreen(void)
{
    UARTConsolePrint("\r\n"
                     "***************************************************\r\n"
                     "User Scenario \r\n"
                     "***************************************************\r\n"
                     "Program with safety register check          : 1\r\n"
                     "Write with safety register check            : 2\r\n"
                     "Buffer Mode                                 : 3\r\n"

                     "Enter any other option to return to Main Menu!\r\n"
                     "***************************************************\r\n\n"

                     "Enter Command Number : \r\n" );
}

/**
  * @brief Function to show User Scenario introduction screen
  * @param None
  * @retval None
  */
void Specific_Behaviour_IntroScreen(void)
{
   UARTConsolePrint("\r\n"
                     "***************************************************\r\n"
                     "Specific Behaviour \r\n"
                     "***************************************************\r\n"
                     "16 word align             : 1\r\n"
                     "Write in protected area   : 2\r\n"

                     "Enter any other option to return to Main Menu!\r\n"
                     "***************************************************\r\n\n"

                     "Enter Command Number : \r\n" );
}

/**
  * @brief Function for UART console print
  * @param puartmsg: pointer to message
  * @retval None
  */
HAL_StatusTypeDef UARTConsolePrint( char *puartmsg )
{
  return HAL_UART_Transmit( &hcom_uart[COM1], (uint8_t *)puartmsg, (uint16_t)strlen( puartmsg ), 500);
}

/**
  * @brief Callback function for UART receive complete
  * @param UartHandle: handle for UART
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)

{
  if(User_Choice == 1)
  {
    /* Option 1 already selected */
    IT_receive = 1;

  }

  else if(User_Choice == 2)
  {
    /* Option 2 already selected */
    IT_receive = 2;

  }
  else if(User_Choice == 3)
  {
    /* Option 4 already selected */
    IT_receive = 3;

  }
  else if(User_Choice == 4)
  {
    /* Option 4 already selected */
    IT_receive = 4;

  }

  else
  {
    /* Set transmission flag: transfer complete*/
    char MajoBuffer[40];

    if(aRxBuffer[0] == 0x2AU) /* 0x2A hex for * character */
    {
      UartReady = RESET;
      aRxBuffer[0] = 0;
      sprintf(MajoBuffer, "STM32L0_nucleo_reader\r\n");

      UARTConsolePrint(MajoBuffer);
      IT_receive = 0;

      if(HAL_UART_Receive_IT(UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
      {
        App_Error_Handler();
      }
      IntroScreen();
    }
    else if(aRxBuffer[0] == 0x31U) /* 0x31 hex for 1 character */
    {
      /* Command Set option 1 selected from Main Menu*/
      IT_receive = 1;
      printf("\nEnter Command set: \r\n");

    }
    else if(aRxBuffer[0] == 0x32U) /* 0x32 hex for 2 character */
    {
      /* User Scenario option 2 selected from Main Menu*/
      IT_receive = 2;
      User_Scenario_IntroScreen();

    }
    else if(aRxBuffer[0] == 0x33U) /* 0x33 hex for 3 character */
    {
      /* Specific Behaviour option 3 selected from Main Menu*/
      IT_receive = 3;
      Specific_Behaviour_IntroScreen();

    }
    else if(aRxBuffer[0] == 0x34U ) /* 0x34 hex for 4 character */
    {
      /* Run Test Cases option 4 selected from Main Menu*/
      IT_receive = 4;

    }
    else
    {
      /* No option selected from Main Menu*/
      UARTConsolePrint("\r\nEnter correct option!\r\n" );
      UartReady = RESET;
      aRxBuffer[0] = 0;

      if(HAL_UART_Receive_IT(UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
      {
        App_Error_Handler();
      }

      IntroScreen();
    }

  }

  if (IT_receive != 0)
  {
    /* Disable UART Interrupt to handle option selected*/

    HAL_NVIC_DisableIRQ(USARTX_INTERRUPT);
  }
}

/**
  * @brief Callback function for UART transfer complete
  * @param UartHandle: handle for UART
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete*/
  UartReady = SET;
}

/**
  * @brief  This function is executed in case of application error occurrence.
  * @retval None
  */
void App_Error_Handler(void)
{
  while (1)
  {
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
	HAL_Delay(200);
  }
}

#ifdef __cplusplus
}
#endif

