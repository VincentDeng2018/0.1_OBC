/**
  @page USBH_HID  USB Host HID example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Host HID example
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

This example provides a description of how to use the USB OTG host peripheral 
on the STM32F207xx, STM32F407xx, STM32F429xx and STM32F107xx devices.

When an USB Device is attached to the Host port, the device is enumerated and checked
whether it can support HID device or not, if the attached device is HID one, when the user
press the key button, the mouse or the keyboard application is launched.

Moving the mouse will move the pointer in the display rectangle and if a button is pressed,
the corresponding rectangle will be highlighted in green
If a keyboard has been attached, the display show the following messages and the taped
characters are displayed in green on the display.

This example works:
  - in high speed (HS) when the STM322xG-EVAL, the STM324xG-EVAL or the STM324x9I-EVAL  
    board and the USB OTG HS peripheral are used.
  - in full speed (FS) when the STM322xG-EVAL, the STM324xG-EVAL or the STM324x9I-EVAL board and the
    USB OTG FS peripheral are used, or when using the STM3210C-EVAL board.

@note In the USB Host HID class, two layouts are defined in the usbh_hid_keybd.h file
      and could be used (Azerty and Querty)
        //#define QWERTY_KEYBOARD
        #define AZERTY_KEYBOARD
      The User can eventually add his own layout by editing the HID_KEYBRD_Key array
      in the usbh_hid_keybd.c file
    
  
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
    - STM322xG-EVAL_USBH-HS: to configure the project for STM32F207xx devices and use USB OTG HS peripheral
    - STM322xG-EVAL_USBH-FS: to configure the project for STM32F207xx devices and use USB OTG FS peripheral

    - STM324xG-EVAL_USBH-HS: to configure the project for STM32F407xx devices and use USB OTG HS peripheral
    - STM324xG-EVAL_USBH-FS: to configure the project for STM32F407xx devices and use USB OTG FS peripheral

    - STM324x9i-EVAL_USBH-HS:to configure the project for STM32F429xx devices and use USB OTG HS peripheral
    - STM324x9i-EVAL_USBH-FS:to configure the project for STM32F429xx devices and use USB OTG FS peripheral
    - STM324x9i-EVAL_USBH-HS-IN-FS: to configure the project for STM32F429xx devices and use USB OTG HS-IN-FS peripheral

    - STM3210C-EVAL_USBH-FS: to configure the project for STM32F107xx devices  
  
  - Rebuild all files and load your image into target memory
  - Run the application
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
