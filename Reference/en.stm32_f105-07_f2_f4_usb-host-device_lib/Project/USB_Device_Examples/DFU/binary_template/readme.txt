/**
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the binary directory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */


Description
===================
This directory contains a set of sources files that build the application to be
loaded into Flash memory using device firmware upgrade (DFU).

To build such application, some special configuration has to be preformed: 
1. Set the program load address at 0x0800C000 for STM32F207xx, STM32F407xx and STM32F429xx 
and 0x08008000 for the STM32F107xx, using your toolchain linker file
2. Relocate the vector table at address 0x0800C000 for STM32F207xx, STM32F407xx, STM32F429xx 
and 0x08008000 for the STM32F107xx, using the "NVIC_SetVectorTable" function.
 
The SysTick example provided within the STM32F207xx, STM32F407xx, STM32F429xx and STM32F107xx Standard 
Peripheral examples is used as illustration. 
   
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to 72 MHz for STM32F207xx, STM32F407xx and STM32F429xx devices and to 24MHz for 
STM32F107xx devices, the SysTick is clocked by the AHB clock (HCLK)

A "Delay" function is implemented based on the SysTick end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


Directory contents
==================

  + binary_template\EWARM: This folder contains a preconfigured project file 
                           that produces a binary image of SysTick example to be  
                           loaded with DFU.

  + binary_template\MDK-ARM: This folder contains a preconfigured project file 
                           that produces a binary image of SysTick example to be 
                           loaded with DFU.
						   
 + \SW4STM32 : This folder contains a preconfigured project file 
                that produces a binary image of SysTick example to be 
                loaded with DFU.                                      
                  
  + binary_template\inc: contains the binary_template firmware header files
    - stm32fxxx_it.h      Header for stm32xxx_it.c
    - stm32f10x_conf.h    Library Configuration file for STM32F10xx devices
    - stm32f4xx_conf.h    Library Configuration file for STM32F4xxx devices
    - stm32f2xx_conf.h    Library Configuration file for STM32F2xxx devices
    - lcd_log_conf.h	  LCD Configuration file
	- app.h               Header for app.c

  + binary_template\src: contains the binary_template firmware source files 
    - app.c              Main program
    - stm32fxxx_it.c     Interrupt handlers for STM32F10xxx,STM32L15xxx, STM32F30xxx and STM32F37xxx devices
	- system_stm32f2xx.c System Configuration file for STM32F2xxx devices
	- system_stm32f4xx.c System Configuration file for STM32F4xxx devices
	- system_stm32f10x.c System Configuration file for STM32F10xx devices

	Hardware environment
====================
This example runs on STMicroelectronics STM322xG-EVAL, STM3210C-EVAL, STM324xG-EVAL
and STM324x9i-EVAL evaluation boards and can be easily tailored to any other hardware.
To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

 + STM322xG-EVAL 
    - Use LD1, LD2, LD3 and LD4 leds
 
 + STM3210C-EVAL
    - Use LD1, LD2, LD3 and LD4 leds

 + STM324xG-EVAL
    - Use LD1, LD2, LD3 and LD4 leds
 
 + STM324x9i-EVAL
	- Use LD1, LD2, LD3 and LD4 leds
 
   
How to use it
=============
In order to load the SysTick example with the DFU, you must do the following:
 
  + EWARM
    - Open the binary_template.eww workspace
    - In the workspace toolbar select the project config:
        - STM322xG-EVAL
        - STM3210C-EVAL
        - STM324xG-EVAL
        - STM324x9i-EVAL
		
   - Rebuild all files: Project->Rebuild all
   - A binary file will be generated under "STM3210C-EVAL\Exe" or "STM322xG-EVAL\Exe" or "STM324xG-EVAL\Exe"
	 or "STM324x9i-EVAL\Exe" folder depending on configuration you selected.
	 
	
 + The generated .hex file should be converted to the DFU format using the “DFU File
   Manager Tool” included in the “DfuSe” PC software install. For more details on
   how to convert a .hex file to DFU format please refer to the UM0412 user manual
   “Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension”
   available from the STMicroelectronics microcontroller website www.st.com.

************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE******
