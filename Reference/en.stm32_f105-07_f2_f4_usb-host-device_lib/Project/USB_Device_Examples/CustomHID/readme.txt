/**
  @page USBD_HID  USB Device HID example

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Custom HID application example
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

This application is a part of the USB Device Library package. It describes how to
use USB device application based on the Custom HID Class.

To test the application, user must proceed as follows:
 - Start the "USB HID Demonstrator" PC applet, available for download from www.st.com,
   and connect STM32 USB to PC
 - The device should be detected and shown in the USB HID target box
 - Press Graphic View button
 - Select "use SET_FEATURE" or "use SET_REPORT" in order to use SET_REPORT request to send HID Report
   for LED control
 - Use the potentiometer of the EVAL board to transfer the result of the converted voltage
  (via the ADC) the to the PC host (these values are sent to the PC using the endpoint1 IN)
 - Make sure that following report ID are configured: LED1 ID (0x1), LED2 ID(0x2), LED3 ID(0x3),
   LED4 ID(0x4), BUTTON1_ID(0x5) and Potentiometer_ID(0x7).
 - Select LEDs to switch on/off on the EVAL board: a SET_REPORT request will be sent

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
