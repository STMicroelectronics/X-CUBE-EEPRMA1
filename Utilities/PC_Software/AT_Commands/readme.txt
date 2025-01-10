/**
  @page Docklight application for X-NUCLEO-PGEEZ1 Expansion Board
  
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  SRA Application Team
  * @brief   This application contains Docklight application which demonstartes
			 different instructions on M95P32 SPI EEPROM
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
  @endverbatim

  
@par Tool Description
	
	- Docklight is a testing, analysis and simulation tool for serial communication protocols
	- It is supported on Windows platform
	  URL: https://docklight.de/
	- With this application the user can send predefined sequences to execute various instructions
	  supported by M95P32 SPI EEPROM on X-NUCLEO-PGEEZ1 only

@par Keywords

EEPROM, M95P32, SPI, External Storage Device, Docklight
  
@par Hardware and Software environment

  - Docklight runs on Windows platform and can be installed from above URL
  - Docklight requires X-NUCLEO-PGEEZ1 exapansion board connected with supported STM32 NUCLEO boards
	which, in turn, is plugged to the PC via a USB cable
  - This application has been tested with X-NUCLEO-PGEEZ1 on STMicroelectronics:
    - NUCLEO-F401RE RevC board  
    - NUCLEO-L053R8 RevC board
    - NUCLEO-G474RE RevC board
    - NUCLEO-H743ZI RevE board

@par How to use it? 

In order to make the program work, you must do the following:
 
 - Connect X-NUCLEO-PGEEZ1 exapansion board with supported STM32 NUCLEO boards to the PC via a USB cable
 - Windows: The tool can be launched by double-clicking the X-NUCLEO-PGEEZ1_Commands.ptp project file
 - From the drop down menu in the application i.e. Tools-->Project Settings... , select the required COM port
   and start the communication from the play button (F5 shorcut)
 - The application already has a limited set of predefined list of sequences supported by M95P32 SPI EEPROM which can be sent
   by clicking the '--->' button beside each sequence
 - For each sequence, a predefined response shall be received based on the application
 - The user can add their own sequence based on the application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
