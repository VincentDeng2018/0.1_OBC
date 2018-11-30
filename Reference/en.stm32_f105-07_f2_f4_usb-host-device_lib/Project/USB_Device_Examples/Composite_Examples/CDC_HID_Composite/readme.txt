/**
  @page USBD_Composite  USB Device Composite example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Device Composite (HID+CDC) example
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

The examples provide a composite device.
A composite device is one that has multiple interfaces controlled independently of each other.
Using composite device, multiple functions are combined into a single device.
In this Example the independent interfaces are : HID and VCP (CDC).

The implementation of the composite CDC-HID device is done by creating a wrapping
class (see folder hid_cdc_wrapper ) that makes calls to the separate CDC and HID class layer
handlers (files usb_cdc_core.c and usb_hid_core.c files).

To test the demo, you can run HID mouse example and in same time you can
do a USB to USART communication by opening a serial terminal window (like hyperterminal)
and selecting the STM32 virtual comport (you need to follow same steps as  the CDC virtual comport example).
   
To run this example, you will need to modify the following lines in the .inf file of the cdc driver:
stmcdc.inf (you will find it in "C:\Program Files\STMicroelectronics\Software\Virtual COM Port Driver).
 
The stmcdc.inf exists to assist in the loading of the proper USB serial drivers in the Windows OSs for 
the composite device to operate correctly as a virtual COM interface.

The following Lines associates the CDC device with the OS’s usbser.sys driver file 
and causes the OS to load this driver during the enumeration process.

The "MI" stands for Multiple Interface.

***********************************************
;VID/PID Settings
;------------------------------------------------------------------------------
[DeviceList.NT]
%DESCRIPTION%=DriverInstall,USB\VID_0483&PID_3256&MI_01

[DeviceList.NTamd64]
%DESCRIPTION%=DriverInstall,USB\VID_0483&PID_3256&MI_01

******************************************************


@par Hardware and Software environment 

- This example has been tested with STM3210C-EVAL RevC (STM32F107xx devices), STM322xG-EVAL RevC (STM32F207xx), 
	STM324xG-EVAL RevC (STM32F407xx) and STM324x9I-EVAL RevB (STM32F429xx) 
   
  - STM3210C-EVAL Set-up 
    - Use CN2 connector to connect the board to host
	- Use CN6 connector to connect the board to PC over RS232 (USART) serial cable.
		@note Make sure that :
		 - Please ensure that Jumper JP16 is connected in position 2-3
		 
  - STM322xG-EVAL Set-up
    - Use CN8 connector to connect the board to host when using USB OTG FS peripheral 
	- Use CN9 connector to connect the board to host when using USB OTG HS peripheral 
	- Use CN16 connector to connect the board to PC over RS232 (USART) serial cable. 
        @note Make sure that :                                                                                     
		 - Please ensure that Jumper JP22 is connected in position 1-2
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS 
		 
  - STM324xG-EVAL Set-up
    - Use CN8 connector to connect the board to host when using USB OTG FS peripheral 
	- Use CN9 connector to connect the board to host when using USB OTG HS peripheral 
	- Use CN16 connector to connect the board to PC over RS232 (USART) serial cable. 
		@note Make sure that :
         - Please ensure that Jumper JP22 is connected in position 1-2
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS 
		 
  - STM324x9I-EVAL Set-up
    - Use CN14 connector to connect the board to host when using USB OTG FS peripheral
    - Use CN9  connector to connect the board to host when using USB OTG HS peripheral
    - Use CN15  connector to connect the board to host when using USB OTG HS-IN-FS peripheral
	- Use CN8 connector to connect the board to PC over RS232 (USART) serial cable
      Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM324x9I-EVAL Evaluation Board 
              User Manual.
        @note Make sure that :
         - Please ensure that jumper JP16 is not fitted when using USB OTG FS
		 - Please ensure that Jumper JP7 is connected in position 1-2
		 
@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain
  - In the workspace toolbar select the project config:
    - STM322xG-EVAL_USBD-HS: to configure the project for STM32F207xx devices and use USB OTG HS peripheral
    - STM322xG-EVAL_USBD-FS: to configure the project for STM32F207xx devices and use USB OTG FS peripheral

    - STM324xG-EVAL_USBD-HS: to configure the project for STM32F407xx devices and use USB OTG HS peripheral
    - STM324xG-EVAL_USBD-FS: to configure the project for STM32F407xx devices and use USB OTG FS peripheral

    - STM324x9i-EVAL_USBD-HS:to configure the project for STM32F429xx devices and use USB OTG HS peripheral
    - STM324x9i-EVAL_USBD-FS:to configure the project for STM32F429xx devices and use USB OTG FS peripheral
    - STM324x9i-EVAL_USBD-HS-IN-FS: to configure the project for STM32F429xx devices and use USB OTG HS-IN-FS peripheral

    - STM3210C-EVAL_USBD-FS: to configure the project for STM32F107xx devices  
  
  - Rebuild all files and load your image into target memory
  - Run the application
  
 - the hyperterminal configuration : 
	- Baudrate : 115200
	- Data bits: 8
	- Parity : none
	- Stop bit : 1
	- Flow Control : None
	  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
