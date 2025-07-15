## <b>EEPROMRW application for X-NUCLEO-EEICA1 Expansion Board based on M24256E-F and M24M01E-F serial I2C bus EEPROM and STM32 Nucleo Boards Example Description</b>

The X-NUCLEO-EEICA1 expansion board is designed for M24256E-F and M24M01E-F serial I2C bus EEPROM for data reading and writing

The expansion board acts as an external storage device that can be used to store
data such as manufacturing traceability, calibration, user settings, error flags, data
logs and monitoring data to make applications more flexible and accurate.

Known limitations:

- When starting the project from Example Selector in STM32CubeMX and regenerating it 
 from ioc file, you may face a build issue. To solve it, if you started the project for the 
 Nucleo-F401RE board, remove from the IDE project the file stm32f4xx_nucleo.c in the Application/User 
 virtual folder and delete, from Src and Inc folders, the files: stm32f4xx_nucleo.c, stm32f4xx_nucleo.h 
 and stm32f4xx_nucleo_errno.h. 
 The same operations apply if you start the project for another STM32 Nucleo board (e.g. for 
 Nucleo-G474RE, the files to be removed are stm32g4xx_nucleo.c, stm32g4xx_nucleo.h 
 and stm32g4xx_nucleo_errno.h).


### <b>Keywords</b>

EEPROM, M24256E, M24M01E, I2C, External Storage Device, Configurable Device Address


### <b>__Directory contents__</b>

 - app_x-cube-eeprma1.c          EEPROMRW application initialization and application code.
 
 - main.c                        Main program body.
 
 - stm32**xx_hal_msp.c           This file provides code for the MSP Initialization and de-Initialization.
 
 - stm32**xx_it.c                Source code for interrupt Service Routines.
 
 - stm32**xx_nucleo.c            Source file for the BSP Common driver.
 
 - stm32**xx_nucleo_bus.c        Source file for the BSP BUS IO driver.
 
 - system_stm32**xx.c            CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.

 
### <b>Hardware and Software environment</b>

  - This example runs on STM32 Nucleo boards with X-NUCLEO-EEPRMA2 STM32 expansion board
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board  
    - NUCLEO-L053R8 RevC board
    - NUCLEO-G474RE RevC board
	 - NUCLEO-H753ZI RevE board

- ADDITIONAL_BOARD : X-NUCLEO-EEICA1 https://www.st.com/en/ecosystems/x-nucleo-eeica1.html
- ADDITIONAL_COMP : M24256E-F https://www.st.com/en/memories/m24256e-f.html
- ADDITIONAL_COMP : M24M01E-F https://www.st.com/en/memories/m24m01e-f.html


### <b>How to use it?</b>

In order to make the program work, you must do the following:
- WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
   
- Open STM32CubeIDE (this firmware has been successfully tested with Version 1.18.1).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.38.0.0) or the IAR toolchain (this firmware has 
   been successfully tested with Embedded Workbench V9.60.3).
   
- Rebuild all files and load your image into target memory.

- Run the example.

- Alternatively, you can download the pre-built binaries in "Binary" folder included in the distributed package.

### <b>Author</b>

SRA Application Team

### <b>License</b>

Copyright (c) 2025 STMicroelectronics. All rights reserved.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component. If no LICENSE file comes with this software, it is provided AS-IS.