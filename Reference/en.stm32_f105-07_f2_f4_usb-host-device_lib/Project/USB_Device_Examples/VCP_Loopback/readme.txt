/**
  @page USBD_VCP_Loopback USB Device VCP example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Device VCP (Virtual Com Port) example
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License 
  *   
  ******************************************************************************
   @endverbatim

   
@par Example Description 

This Demo presents the implementation of a Virtual Com Port (VCP) capability in 
the STM32F207xx, STM32F407xx, STM32F429xx, STM32F446xx, STM32F469xx and STM32F107xx devices. 

The aim of this example is to read data from and write data to USB devices using the CDC protocol. 
It makes the USB device look like a serial port (NO serial cable connectors:You can see the data
transferred to and from via USB instead of USB-to-USART bridge connection). 
This example loops back the contents of a text file over usb port. To run the example, Type a message using the Pc's keyboard.
Any data that shows in HyperTerminal is received from the device. Here are the modifiction done versus the VCP example:

- In class layer usb_cdc_core.c : remove the asynchronous transfer which is called on SOF callback and remove management of class control 
  requests which are not needed for the simple send/receive.
- In application layer file usbd_cdc_vcp.c : implemente the above API functions:
		- VCP_SendData: Sends data 
		- VCP_CheckDataSent: Returns 1 when data sent
		- VCP_ReceiveData: Prepares reception of data 
		- VCP_CheckDataReceived: Returns received byte count
		
This example works
  - in high speed (HS) when the STM322xG-EVAL, the STM324xG-EVAL board, the STM324x9I-EVAL board, 
     STM32469I-EVAL or the STM32446-EVAL board and the USB OTG HS peripheral are used
  - in full speed (FS) when the STM322xG-EVAL, the STM324xG-EVAL board, the STM324x9I-EVAL board
    STM32469I-EVAL or the STM32446-EVAL board and the USB OTG FS peripheral are used, or when using
    the STM3210C-EVAL board.
    
@par Hardware and Software environment 

   - This example has been tested with STM3210C-EVAL RevC (STM32F107xx devices), 
     STM322xG-EVAL RevC (STM32F207xx), STM324xG-EVAL RevC (STM32F407xx), STM324x9I-EVAL RevB (STM32F429xx),
	 STM32446E-EVAL RevB (STM32F446xx) and STM32469I-EVAL  RevC (STM32F469xx)
   - This example works in HS-IN-FS mode when STM324x9I-EVAL, STM32469I-EVAL boards and USB OTG HS-IN-FS 
	 peripheral are used
  
  - STM3210C-EVAL Setup 
    - Use CN2 connector to connect the board to a PC host   
	- Jumper JP16 should be connected in position 2-3.
	
  - STM322xG-EVAL Setup
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS 
		 
  - STM324xG-EVAL Setup
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS 
		 
  - STM32446E-EVAL Setup
    - Use CN9 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN8 connector to connect the board to a PC host when using USB OTG HS peripheral
		@note Make sure that :
		 - jumper JP4 is on FS position (2-3)
		 - jumper JP7 is on FS position (2-3)


  - STM324x9I-EVAL Set-up
    - Use CN14 connector to connect the board to host when using USB OTG FS peripheral
    - Use CN9  connector to connect the board to host when using USB OTG HS peripheral
    - Use CN15  connector to connect the board to host when using USB OTG HS-IN-FS peripheral
      Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM324x9I-EVAL Evaluation Board 
              User Manual.
        @note Make sure that :
         - Please ensure that jumper JP16 is not fitted when using USB OTG FS
        
  - STM32469I-EVAL RevC Set-up
    - Connect the STM32469I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN8 : connector to connect the board to host when using USB OTG HS peripheral 
      - CN13: connector to connect the board to host when using USB OTG FS peripheral
      - CN14: connector to connect the board to host when using USB OTG HS-IN-FS peripheral
        Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM32469I-EVAL Evaluation Board 
              User Manual.
        @note Make sure that :
         - jumper JP9 must be removed when using USB OTG FS
         
@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain
  - In the workspace toolbar select the project config:
    - STM322xG-EVAL_USBD-HS: to configure the project for STM32F207xx devices and use USB OTG HS peripheral
    - STM322xG-EVAL_USBD-FS: to configure the project for STM32F207xx devices and use USB OTG FS peripheral

    - STM324xG-EVAL_USBD-HS: to configure the project for STM32F407xx devices and use USB OTG HS peripheral
    - STM324xG-EVAL_USBD-FS: to configure the project for STM32F407xx devices and use USB OTG FS peripheral

    - STM32446E-EVAL_USBD-HS: to configure the project for STM32F446xx devices and use USB OTG HS peripheral
    - STM32446E-EVAL_USBD-FS: to configure the project for STM32F446xx devices and use USB OTG FS peripheral

    - STM324x9i-EVAL_USBD-HS:to configure the project for STM32F429xx devices and use USB OTG HS peripheral
    - STM324x9i-EVAL_USBD-FS:to configure the project for STM32F429xx devices and use USB OTG FS peripheral
    - STM324x9i-EVAL_USBD-HS-IN-FS: to configure the project for STM32F429xx devices and use USB OTG HS-IN-FS peripheral

    - STM32469I-EVAL_USBD-HS:to configure the project for STM32F469xx devices and use USB OTG HS peripheral
    - STM32469I-EVAL_USBD-FS:to configure the project for STM32F469xx devices and use USB OTG FS peripheral
    - STM32469I-EVAL_USBD-HS-IN-FS: to configure the project for STM32F469xx devices and use USB OTG HS-IN-FS peripheral

    - STM3210C-EVAL_USBD-FS: to configure the project for STM32F107xx devices  
  
  - Rebuild all files and load your image into target memory
  - Run the application
  
  - the hyperterminal configuration 
    - Baudrate : 115200
    - Data bits: 8
    - Parity : none
    - Stop bit : 1
    - Flow Control : None
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
