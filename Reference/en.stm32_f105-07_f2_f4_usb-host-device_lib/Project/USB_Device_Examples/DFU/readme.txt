/**
  @page USBD_DFU  USB Device DFU example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Device DFU (Device Firmware Upgrade) example
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

This Demo presents the implementation of a device firmware upgrade (DFU) capability
in the STM32F207xx, STM32F407xx, STM32F429xx and STM32F107xx devices. 
It follows the DFU class specification defined by the USB Implementers Forum for 
reprogramming an application through USB-FS-Device. 
The DFU principle is particularly well suited to USB-FS-Device applications that 
need to be reprogrammed in the field.

To have an example of how to generate a DFU image, please refer to the project 
provided in folder "binary_template". This project allows to generate a binary
image (*.bin file) that can be used to generate a *.dfu image using the tool
provided by ST: "DFUse\DFU File Manager" available for download from www.st.com

Once the *.dfu image is generated, it can be downloaded into internal flash memory
using the ST tool "DFUse\DFUse Demonstration" or the tool "DFUse\STDFU Tester" both
available for download from www.st.com


The supported memories for this example are:
 - Internal Flash memory for STM32F2xx, STM32F4xx and STM32F105/7 devices
 - OTP memory for STM32F2xx and STM32F4xx devices.
 
To add a new memory interface you can use the template memory interface provided 
in folder \Libraries\STM32_USB_Device_Library\Class\dfu.

@note After each device reset, hold down the Key push-button on the STM322xG-EVAL,  
      STM324xG-EVAL, STM324x9I-EVAL or STM3210C-EVAL evaluation boards

This example works
  - in high speed (HS) when the STM322xG-EVAL, the STM324xG-EVAL or the STM324x9I-EVAL board and the
	USB OTG HS peripheral are used
  - in full speed (FS) when the STM322xG-EVAL, the STM324xG-EVAL or the STM324x9I-EVAL board and the
    USB OTG FS peripheral are used, or when using the STM3210C-EVAL board.
  - This example works in HS-IN-FS mode when STM324x9I-EVAL board and USB OTG HS-IN-FS peripheral are used 

@par Hardware and Software environment 

   - This example has been tested with STM3210C-EVAL RevC (STM32F107xx devices), 
     STM322xG-EVAL RevC (STM32F207xx), STM324xG-EVAL RevC (STM32F407xx) and STM324x9I-EVAL RevB (STM32F429xx) 
   
  - STM3210C-EVAL Set-up 
    - Use CN2 connector to connect the board to host

  - STM322xG-EVAL Set-up
    - Use CN8 connector to connect the board to host when using USB OTG FS peripheral 
	- Use CN9 connector to connect the board to host when using USB OTG HS peripheral 
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS 
		 
  - STM324xG-EVAL Set-up
    - Use CN8 connector to connect the board to host when using USB OTG FS peripheral 
	- Use CN9 connector to connect the board to host when using USB OTG HS peripheral 
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS 
		 
  - STM324x9I-EVAL Set-up
    - Use CN14 connector to connect the board to host when using USB OTG FS peripheral
    - Use CN9  connector to connect the board to host when using USB OTG HS peripheral
    - Use CN15  connector to connect the board to host when using USB OTG HS-IN-FS peripheral
      Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM324x9I-EVAL Evaluation Board 
              User Manual.
        @note Make sure that :
         - Please ensure that jumper JP16 is not fitted when using USB OTG FS

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
	
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
