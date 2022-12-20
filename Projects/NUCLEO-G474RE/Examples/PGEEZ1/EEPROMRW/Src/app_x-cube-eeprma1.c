/**
  ******************************************************************************
  * @file           : app_x-cube-eeprma1.c
  * @brief          : This file provides code for the configuration
  *						of the STMicroelectronics.X-CUBE-EEPRMA1.4.1.0 instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#define SIZE520         520U
#define SIZE600         600U
#define SIZE1024        1024U
#define RXBUFFERSIZE    45U

/* Private variables ---------------------------------------------------------*/
uint8_t ReceiveBuff[SIZE1024] = {0};
uint8_t pData[SIZE600];

#ifdef USE_QUADSPI
uint8_t ReceiveBuff1[SIZE1024] = {0};
uint8_t Data[1040] = {0};
extern QSPI_HandleTypeDef QSPI_INSTANCE;
#endif

uint8_t SampleData[SIZE200] = {0};
uint8_t ReadBuff[SIZE200] = {0};

__IO ITStatus UartReady = RESET;
unsigned int add = 0x00;
uint8_t Buff[SIZE520]={0};
unsigned int CmdParam[SIZE520]={0};

volatile uint8_t IT_receive = 0;

uint8_t aRxBuffer[RXBUFFERSIZE];
/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int32_t Test_ReadStatusReg(uint32_t TargetName);
int32_t WriteEnableReadStatus(uint32_t TargetName);
int32_t WriteDisableReadStatus(uint32_t TargetName);
void IntroScreen(void);
HAL_StatusTypeDef UARTConsolePrint( char *puartmsg );
HAL_StatusTypeDef HAL_UART_Receive_Until_CR(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
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

void MX_EEPROM_Process(void)
{
  /* USER CODE BEGIN M95P32_Library_Process */
   MX_EEPROM_RW_Process();

  /* USER CODE END M95P32_Library_Process */
}

HAL_StatusTypeDef HAL_UART_Receive_Until_CR(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint16_t* tmp;
  uint16_t uhMask;
  uint32_t tickstart = 0;

  /* Check that a Rx process is not already ongoing */
  if(huart->RxState == HAL_UART_STATE_READY)
  {
    if((pData == NULL ) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* In case of 9bits/No Parity transfer, pData buffer provided as input paramter
    should be aligned on a u16 frontier, as data to be received from RDR will be
    handled through a u16 cast. */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
    {
      if((((uint32_t)pData)&1U) != 0U)
      {
        return  HAL_ERROR;
      }
    }

    /* Process Locked */
    __HAL_LOCK(huart);

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->RxState = HAL_UART_STATE_BUSY_RX;

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    huart->RxXferSize = Size;
    huart->RxXferCount = Size;

    /* Computation of UART mask to apply to RDR register */
    UART_MASK_COMPUTATION(huart);
    uhMask = huart->Mask;

    /* as long as data have to be received */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE)){
      /* CR + LF ==> 0x0D +0x0A */
      while((huart->RxXferCount > 0U)&&(((uint16_t)(huart->Instance->RDR & uhMask)) !=0x000AU))
      {
        huart->RxXferCount--;
        if(UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
        tmp = (uint16_t*) pData ;
        *tmp = (uint16_t)(huart->Instance->RDR & uhMask);
        pData +=2U;
      }
    }
    else{
      /* CR + LF ==> 0x0D +0x0A */
      while((huart->RxXferCount > 0U)&&(((uint8_t)(huart->Instance->RDR & (uint8_t)uhMask))!=0x0AU))
      {
        huart->RxXferCount--;
        if(UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
        *pData++ = (uint8_t)(huart->Instance->RDR & (uint8_t)uhMask);
      }
    }

    /* At end of Rx process, restore huart->RxState to Ready */
    huart->RxState = HAL_UART_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
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
	HAL_UART_Receive_Until_CR(&hcom_uart[COM1], (uint8_t *)aRxBuffer+1, 30, 0xffffff);

    memset(&Buff, 0x00, SIZE520);
    memset(&CmdParam, 0x00, SIZE520 * sizeof(CmdParam[0]));

    index = 0;
    index_2 = 0;
    index_3 = 0;
    while(aRxBuffer[index] != 0U)
    {
      if(aRxBuffer[index]==0x20U)
      {

        CmdParam[index_3] = (unsigned int)strtol((char*)Buff,NULL,16);
        index_3 ++;
        memset(&Buff, 0x00, 256U);
        index ++;
        index_2 = 0;
      }
      else if (aRxBuffer[index]==0x0AU)
      {

        CmdParam[index_3] = (unsigned int)strtol((char*)Buff,NULL,16);
        memset(&Buff, 0x00, 256U);
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

    memset(&Buff, 0x00, 256U);
    memset(&aRxBuffer, 0x00, RXBUFFERSIZE);

    switch(CmdParam[0])
    {

    case CMD_ERASE_SECTOR:
      if(CmdParam[2] != 0U)
      {
        printf(" Erase Sector n'a qu'un parameter\n");
        IntroScreen();
        break;
      }
      printf(" Erase Sector Add:  %6.6ux",CmdParam[1]);
      PGEEZ1_M95P32_EraseSector(CmdParam[1]);
      IntroScreen();
      break;

    case CMD_ERASE_BLOCK:
      if(CmdParam[2] != 0U)
      {
        printf(" Erase Block n'a qu'un parameter\n");
        IntroScreen();
        break;
      }
      printf(" Erase Block Add:  %6.6ux",CmdParam[1]);
      PGEEZ1_M95P32_EraseBlock(CmdParam[1]);
      IntroScreen();
      break;

    case CMD_ERASE_CHIP:
      if(CmdParam[1] != 0U)
      {
        printf(" Erase Chip n'a pas de parameter\n");
        IntroScreen();
        break;
      }
      printf(" Erase Chip \n");
      PGEEZ1_M95P32_EraseChip();
      IntroScreen();
      break;

    case CMD_ERASE_PAGE:
      if(CmdParam[2] != 0U)
      {
        printf(" Erase Page n'a n'a qu'un parameter\n");
        IntroScreen();
        break;
      }
      printf(" Erase Page Add:  %6.6ux\n",CmdParam[1]);
      PGEEZ1_M95P32_ErasePage(CmdParam[1]);
      IntroScreen();
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

        PGEEZ1_M95P32_VolRegRead(&volatile_reg_val);

        if( volatile_reg_val == 0x01U)
        {
          printf(" Prog Page Add:  %6.6ux\n",CmdParam[1]);
          PGEEZ1_M95P32_ProgramPage(NO_INSTANCE, pData, add, index);
          IntroScreen();
        }
        else
        {
          printf(" Prog Page in Buffer Mode Add:  %6.6ux\n",CmdParam[1]);
          PGEEZ1_M95P32_PageProgBuffer(NO_INSTANCE, pData, add, index);
          IntroScreen();
        }
      }
      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
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
        printf(" Write Page Add:  %6.6ux\n",CmdParam[1]);
        PGEEZ1_M95P32_WritePage(NO_INSTANCE, pData, add, index);
        IntroScreen();
      }
      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }
      break;

    case CMD_WREN:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_WREN 0 Parameter\n");
        IntroScreen();
        break;
      }
      printf("__WRITE_ENABLE__\r\n");
      PGEEZ1_M95P32_WriteEnable();
      IntroScreen();
      break;

    case CMD_WRDI:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_WRDI 0 Parameter\n");
        IntroScreen();
        break;
      }
      printf("__WRITE_DISABLE__\r\n");
      PGEEZ1_M95P32_WriteDisable();
      IntroScreen();
      break;

    case CMD_READ_STATUS_REG:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_READ_STATUS_REG 0 Parameter\n");
        IntroScreen();
        break;
      }
      printf("__READ_STATUS_REG__\r\n");
      status = PGEEZ1_M95P32_StatusRegRead(&status_reg_val);
      if(status != M95_OK)
      {
        printf("__ERROR in READ_STATUS_REG__\r\n");
        App_Error_Handler();
      }
      else
      {
        printf("%2.2x\n", status_reg_val);
        IntroScreen();
      }
      break;

    case CMD_READ_CONF_SAFETY_REG:
      if(CmdParam[2] != 0U)
      {
        printf(" CMD_READ_CONF_SAFETY_REG 1 Parameter\n");
        IntroScreen();
        break;
      }
      printf("CMD_READ_CONF_SAFETY_REG\r\n");
      if(CmdParam[1] == 1U)
      {
        PGEEZ1_M95P32_ConfSafetyRegRead(NO_INSTANCE, ReceiveBuff, 1);
        printf(" value: %2.2x\n",ReceiveBuff[0]);
      }
      else
      {
        PGEEZ1_M95P32_ConfSafetyRegRead(NO_INSTANCE, ReceiveBuff, 2);
        printf(" value: %2.2x | %2.2x\n",ReceiveBuff[0],ReceiveBuff[1]);
      }
      IntroScreen();
      break;

    case CMD_CLEAR_SAFETY_REG:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_CLEAR_SAFETY_REG 0 Parameter\n");
        IntroScreen();
        break;
      }
      printf("CMD_CLEAR_SAFETY_REG\r\n");
      PGEEZ1_M95P32_ClearSafetyFlag();
      IntroScreen();
      break;

    case CMD_READ_VOLATILE_REG:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_READ_VOLATILE_REG 0 Parameter\n");
        IntroScreen();
        break;
      }
      PGEEZ1_M95P32_VolRegRead(&volatile_reg_val);
      printf("CMD_READ_VOLATILE_REG : %2.2x\r\n", volatile_reg_val);
      IntroScreen();
      break;

    case CMD_WRITE_VOLATILE_REG:
      if(CmdParam[2] != 0U)
      {
        printf(" CMD_WRITE_VOLATILE_REG 1 Parameter\n");
        IntroScreen();
        break;
      }
      printf("CMD_WRITE_VOLATILE_REG : %2.2ux\r\n",CmdParam[1]);
      PGEEZ1_M95P32_VolRegWrite((uint8_t)CmdParam[1]);
      IntroScreen();
      break;

    case CMD_WRITE_STATUS_CONF_REG:
      if(CmdParam[3] != 0U)
      {
        printf(" CMD_WRITE_SATUTS_CONF_REG 1 ou 2 parameters\n");
        IntroScreen();
        break;
      }
      memset(&pData, 0x00, SIZE520);
      pData[0] = (uint8_t)CmdParam[1];
      pData[1] = (uint8_t)CmdParam[2];
      printf("CMD_WRITE_SATUTS_CONF_REG : %2.2x | %2.2x\r\n",pData[1],pData[2]);
      PGEEZ1_M95P32_StatusConfigRegWrite(NO_INSTANCE, pData, 2);
      IntroScreen();
      break;

    case CMD_READ_DATA:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_READ_DATA 2 Parameters\n");
          IntroScreen();
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_ReadPage(NO_INSTANCE, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Read data Add: %6.6ux:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  %2.2x",ReceiveBuff[index]);
        }
        printf("\n");
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }
      break;

    case CMD_FAST_READ_SINGLE:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_SINGLE 2 Parameters\n");
          IntroScreen();
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastRead(NO_INSTANCE, ReceiveBuff, CmdParam[1],CmdParam[2]);
        printf(" Fast Read data Add: %6.6ux:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  %2.2x",ReceiveBuff[index]);
        }
        printf("\n");
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }
      break;

    case CMD_FAST_READ_DUAL:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_DUAL 2 Parameters\n");
          IntroScreen();
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastDRead(NO_INSTANCE, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Dual Read data Add: %6.6ux:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2];index++)
        {
          printf("  %2.2x",ReceiveBuff[index]);
        }
        printf("\n");
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }

      break;

    case CMD_FAST_READ_QUAD:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_Quad 2 Parameters\n");
          IntroScreen();
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastQRead(NO_INSTANCE, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Quad Read data Add: %6.6ux:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  %2.2x",ReceiveBuff[index]);
        }
        printf("\n");
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }
      break;

    case CMD_READ_ID_PAGE:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_READ_ID_PAGE 2 Parameters\n");
          IntroScreen();
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_ReadID(NO_INSTANCE, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Read Id Page data Add: %6.6ux:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  %2.2x",ReceiveBuff[index]);
        }
        printf("\n");
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }
      break;

    case CMD_FAST_READ_ID_PAGE:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_FAST_READ_ID_PAGE 2 Parameters\n");
          IntroScreen();
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_FastReadID(NO_INSTANCE, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf(" Fast Read Id Page data Add: %6.6ux:\n",CmdParam[1]);
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("  %2.2x",ReceiveBuff[index]);
        }
        printf("\n");
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
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
        PGEEZ1_M95P32_WriteID(NO_INSTANCE, pData, add, CmdParam[2]);
        printf(" CMD_WRITE_ID_PAGE Add %6.6ux data : \n", add);
        for(index_2 = 0; index_2 < index; index_2++)
        {
          printf(" %2.2x",pData[index_2] );
        }
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }
      break;

    case CMD_DEEP_POWER_DOWN:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_DEEP_POWER_DOWN 0 Parameter\n");
        IntroScreen();
        break;
      }
      PGEEZ1_M95P32_DeepPowerDown();
      printf("CMD_DEEP_POWER_DOWN\n");
      IntroScreen();
      break;

    case CMD_DEEP_POWER_DOWN_RELEASE:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_DEEP_POWER_DOWN_RELEASE 0 Parameter\n");
        IntroScreen();
        break;
      }
      PGEEZ1_M95P32_DeepPowerDownRel();
      printf("CMD_DEEP_POWER_DOWN_RELEASE\n");
      IntroScreen();
      break;

    case CMD_READ_JEDEC:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_READ_JEDEC 0 Parameter\n");
        IntroScreen();
        break;
      }
      memset(&ReceiveBuff, 0x00, SIZE1024);
      PGEEZ1_M95P32_JEDECRead(NO_INSTANCE, ReceiveBuff, 3);
      printf("CMD_READ_JEDEC: ");
      for(index = 0; index < 3U; index++)
      {
        printf(" %2.2x",ReceiveBuff[index]);
      }
      printf("\n");
      IntroScreen();
      break;

    case CMD_READ_SFDP:
      if(CmdParam[2] <= SIZE1024)
      {
        if(CmdParam[3] != 0U)
        {
          printf(" CMD_READ_SFDP 2 Parameters\n");
          IntroScreen();
          break;
        }
        memset(&ReceiveBuff, 0x00, SIZE1024);
        PGEEZ1_M95P32_SFDPRead(NO_INSTANCE, ReceiveBuff, CmdParam[1], CmdParam[2]);
        printf("CMD_READ_SFDP: \n");
        for(index = 0; index < CmdParam[2]; index++)
        {
          printf("%4.4ux : %2.2x\n", index + CmdParam[1], ReceiveBuff[index]);
        }
        printf("\n");
        IntroScreen();
      }

      else
      {
        printf(" The requested size is greater than the buffer configured, kindly check MCU application accordingly.\n");
        IntroScreen();
      }
      break;

    case CMD_ENABLE_RESET:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_ENABLE_RESET 0 Parameter\n");
        IntroScreen();
        break;
      }
      PGEEZ1_M95P32_EnableReset();
      printf("CMD_ENABLE_RESET\n");
      IntroScreen();
      break;

    case CMD_SOFT_RESET:
      if(CmdParam[1] != 0U)
      {
        printf(" CMD_SOFT_RESET 0 Parameter\n");
        IntroScreen();
        break;
      }
      PGEEZ1_M95P32_SoftReset();
      printf("CMD_SOFT_RESET\n");
      IntroScreen();
      break;

    default:
      printf("Nucleo_reader : Enter a ranged value : \r\n");
      IntroScreen();
      break;
    }

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
  int32_t status = M95_OK;
  BSP_COM_Init(COM1);
  while( PGEEZ1_M95P32_Init(PGEEZ1_M95P32_0)!= M95_OK);

  if(HAL_UART_Receive_IT(&hcom_uart[COM1], (uint8_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler();
  }
  UartReady = SET;

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

  #ifdef USE_QUADSPI
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
  #endif

  if(status != M95_OK)
  {
    App_Error_Handler();
  }

  /*Erase memory*/
  PGEEZ1_M95P32_WriteEnable();
  PGEEZ1_M95P32_EraseChip();
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
  status = Test_ReadStatusReg(NO_INSTANCE);
  if(status != M95_OK)
  {
    return status;
  }
  HAL_Delay(5);

  /*Reading value of status register after Write Enable*/
  status = WriteEnableReadStatus(NO_INSTANCE);
  if(status != M95_OK)
  {
    return status;
  }
  HAL_Delay(5);

  /*Reading value of status register after Write Disable*/
  status = WriteDisableReadStatus(NO_INSTANCE);
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

  readstatus = PGEEZ1_M95P32_StatusRegRead(&val_status_reg);

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

  /* Prevent unused argument(s) compilation warning */
  UNUSED(TargetName);

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

  PGEEZ1_M95P32_WriteDisable();

  readstatus = PGEEZ1_M95P32_StatusRegRead(&val_status_reg);

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
  /* Prevent unused argument(s) compilation warning */
  UNUSED(TargetName);

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

  PGEEZ1_M95P32_WriteEnable();

  readstatus = PGEEZ1_M95P32_StatusRegRead(&val_status_reg);

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

  /* Prevent unused argument(s) compilation warning */
  UNUSED(TargetName);

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
  PGEEZ1_M95P32_WriteEnable();
  PRINTF_APPLI(" \n\r Write Page (100 Bytes) at Address 0x00  \n\r");

  /*Writing first 100 bytes of a page to 0x0f at address: 0x00 (Page Number-1)*/
  PGEEZ1_M95P32_WritePage(NO_INSTANCE, SampleData, TARGET_ADDRESS_00, SIZE100);
  PGEEZ1_M95P32_ReadPage(NO_INSTANCE, ReadBuff, TARGET_ADDRESS_00, SIZE200);

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
  PGEEZ1_M95P32_ReadPage(NO_INSTANCE, ReadBuff, TARGET_ADDRESS_00, SIZE200);
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
  PGEEZ1_M95P32_FastRead(NO_INSTANCE, ReadBuff, TARGET_ADDRESS_00, SIZE200);

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
  PGEEZ1_M95P32_FastDRead(NO_INSTANCE, ReadBuff, TARGET_ADDRESS_00, SIZE200);
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
  PGEEZ1_M95P32_FastQRead(NO_INSTANCE, ReadBuff, TARGET_ADDRESS_00, SIZE200);
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
  PGEEZ1_M95P32_WriteEnable();

  PRINTF_APPLI("\n *PAGE_ERASE* at Address 0x00 \r\n");
  PGEEZ1_M95P32_ErasePage(TARGET_ADDRESS_00);
  PGEEZ1_M95P32_ReadPage(NO_INSTANCE, a_rcvbuff_page_erase, TARGET_ADDRESS_00, SIZE200);

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

  PGEEZ1_M95P32_WriteEnable();
  PRINTF_APPLI("\n *Sector_ERASE* at Address 0x00 \r\n");

  memset(&SampleData, 0x0F, SIZE200);

  PGEEZ1_M95P32_WritePage(NO_INSTANCE, SampleData, TARGET_ADDRESS_0x400, SIZE200);

  PGEEZ1_M95P32_WriteEnable();
  PGEEZ1_M95P32_EraseSector(TARGET_ADDRESS_0x400);

  PGEEZ1_M95P32_ReadPage(NO_INSTANCE, a_rcvbuff_sector_erase, TARGET_ADDRESS_0x400, SIZE200);

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

  PGEEZ1_M95P32_WriteEnable();
  PRINTF_APPLI(" *Block_ERASE* at Address 0x00 \r\n");

  memset(&SampleData, 0x0F, 0x64);

  PGEEZ1_M95P32_WritePage(NO_INSTANCE, SampleData, TARGET_ADDRESS_0x1800, SIZE200);

  PGEEZ1_M95P32_WriteEnable();
  PGEEZ1_M95P32_EraseBlock(TARGET_ADDRESS_0x1800);

  PGEEZ1_M95P32_ReadPage(NO_INSTANCE, a_rcvbuff_block_erase, TARGET_ADDRESS_0x1800, SIZE200);

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

#ifdef USE_QUADSPI
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

  PGEEZ1_M95P32_WriteEnable();
  PRINTF_APPLI(" *Write Volatile Register to enter Buffer Mode* \r\n");
  PGEEZ1_M95P32_VolRegWrite(SET_BUFEN_BIT);

  memset(&Data, 0x09, SIZE1024);
  PGEEZ1_M95P32_WriteEnable();
  PGEEZ1_M95P32_PageProgBuffer(NO_INSTANCE, Data, 0x00, SIZE1024);
  PGEEZ1_M95P32_VolRegWrite(SET_BUFLD_BIT);

  memset(&ReceiveBuff1, 0x00, SIZE1024);
  PGEEZ1_M95P32_ReadPage(NO_INSTANCE, ReceiveBuff1, 0x00, SIZE1024);

  for(index = 0x00; index < SIZE1024; index++)
  {

    if(ReceiveBuff1[index] != 0x09U)
      return M95_ERROR;
  }
  printf(" \nData after Programming 1024 bytes starting from address 0x00 in buffer mode \r\n");
  for(index = 0x100U; index < 0x300U; index++)
  {
    printf("  %2.2x",ReceiveBuff1[index]);
  }

  PGEEZ1_M95P32_WriteEnable();
  PGEEZ1_M95P32_EraseSector(TARGET_ADDRESS_00);
  return M95_OK;
}
#endif

/**
  * @brief Function to show introduction screen
  * @param None
  * @retval None
  */
void IntroScreen(void)
{
  UARTConsolePrint("\r\n Enter cmd :\r\n" );
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
  /* Set transmission flag: trasfer complete*/
  char MajoBuffer[40];
  IT_receive = 1;
  if(aRxBuffer[0] == 0x2AU) /* 0x2A hex for * character */
  {
    UartReady = RESET;
    aRxBuffer[0] = 0;
    sprintf(MajoBuffer, "STM32G4_nucleo_reader\r\n");

    UARTConsolePrint(MajoBuffer);
    IT_receive = 0;

    if(HAL_UART_Receive_IT(UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
      App_Error_Handler();
    }
  }
  else
  {
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
  /* Set transmission flag: trasfer complete*/
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

