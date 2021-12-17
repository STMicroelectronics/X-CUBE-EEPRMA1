/**
  @page EEPROMRW application for X-NUCLEO-EEPRMA1 Expansion Board based on M24xx I2C and M95xx SPI EEPROM and STM32 Nucleo Boards
  
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  SRA Application Team
  * @brief   This application contains an example which shows
  *          how to use different feature of M24xx I2C and M95xx SPI EEPROM 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  @endverbatim

  
@par Example Description 

This example application shows different features of M24xx and M95xx based EEPROM.
------------------------------------
WARNING: When starting the project from Example Selector in STM32CubeMX and regenerating it
from ioc file, you may face a build issue. To solve it, if you started the project for the
Nucleo-L476RG board, remove from the IDE project the file stm32l4xx_nucleo.c in the Application/User
virtual folder and delete, from Src and Inc folders, the files: stm32l4xx_nucleo.c, stm32l4xx_nucleo.h
and stm32l4xx_nucleo_errno.h.
The same operations apply if you starts the project for another STM32 Nucleo board (e.g. for
Nucleo-L053R8, the files to be removed are stm32l0xx_nucleo.c, stm32l0xx_nucleo.c, stm32l0xx_nucleo.h
and stm32l0xx_nucleo_errno.h).
------------------------------------

After system initialization and clock configuration, meaningful messages will be printed on terminal emulator (exp Tera Term).
Results will be displayed for each operation of Write/Read Byte, Bunch of Data and Pages.


The X-NUCLEO-EEPRMA1 expansion board is designed for M24xx series I²C and M95xx series SPI EEPROM for data reading and writing

The expansion board acts as an external storage device that can be used to store
data such as manufacturing traceability, calibration, user settings, error flags, data
logs and monitoring data to make applications more flexible and accurate



@par Keywords

EEPROM, M24, M95, I2C, SPI, External Storage Device
  
@par Hardware and Software environment

  - This example runs on STM32 Nucleo boards with X-NUCLEO-EEPRMA1 STM32 expansion board
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board  
    - NUCLEO-L053R8 RevC board
    - NUCLEO-G474RE RevC board

ADDITIONAL_BOARD : X-NUCLEO-EEPRMA1 https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/stm32-nucleo-expansion-boards/x-nucleo-eeprma1.html
ADDITIONAL_COMP : M24M01-DF https://www.st.com/content/st_com/en/products/memories/serial-eeprom/standard-serial-eeprom/standard-i2c-eeprom/m24m01-df.html
ADDITIONAL_COMP : M24256-DF https://www.st.com/content/st_com/en/products/memories/serial-eeprom/standard-serial-eeprom/standard-i2c-eeprom/m24256-df.html
ADDITIONAL_COMP : M24C02-F https://www.st.com/content/st_com/en/products/memories/serial-eeprom/standard-serial-eeprom/standard-i2c-eeprom/m24c02-f.html
ADDITIONAL_COMP : M95M01-DF https://www.st.com/content/st_com/en/products/memories/serial-eeprom/standard-serial-eeprom/standard-spi-eeprom/m95m01-df.html
ADDITIONAL_COMP : M95256-DF https://www.st.com/content/st_com/en/products/memories/serial-eeprom/standard-serial-eeprom/standard-spi-eeprom/m95256-df.html
ADDITIONAL_COMP : M95040-R https://www.st.com/content/st_com/en/products/memories/serial-eeprom/standard-serial-eeprom/standard-spi-eeprom/m95040-r.html

@par How to use it? 

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open STM32CubeIDE (this firmware has been successfully tested with Version 1.7.0).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.32.0) or the IAR toolchain (this firmware has 
   been successfully tested with Embedded Workbench V8.50.9).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
