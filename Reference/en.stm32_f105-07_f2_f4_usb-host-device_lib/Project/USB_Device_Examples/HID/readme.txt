/**
  @page USBD_HID  USB Device HID example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Device HID (Joystick) example
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

This example provides a description of how to use the USB OTG Device peripheral 
on the STM32F207xx, STM32F407xx, STM32F429xx and STM32F105/7 devices.
The STM32 device is enumerated as an USB Device Joystick Mouse, that uses the
native PC Host HID driver.

The Joystick mounted on the STM322xG-EVAL, STM324xG-EVAL, STM324x9I-EVAL  and STM3210C-EVAL boards
is used to emulate the Mouse directions.

This example supports remote wakeup (which is the ability of a USB device to bring
a suspended bus back to the active condition), use the key push-button and then the joystick
as the remote wakeup source.    
By default, in Windows powered PC the Power Management feature of USB mouse devices
is turned off. This setting is different from classic PS/2 computer functionality. 
Therefore, to enable the Wake from standby option, you must manually turn on the
Power Management feature for the USB mouse.
To manually enable the Wake from standby option for the USB mouse, 
  - start "Device Manager",
  - select "Mice and other pointing devices",
  - select the "HID-compliant mouse" device (make sure that PID &VID are equal to 0x5710 & 0x0483 respectively) 
  - right click and select "Properties", 
  - select "Power Management" tab,
  - finally click to select "Allow this device to wake the system from standby" check box.

This example works
  - in high speed (HS) when the STM322xG-EVAL, the STM324xG-EVAL or the STM324x9I-EVAL  
    board and the USB OTG HS peripheral are used.
  - in full speed (FS) when the STM322xG-EVAL, the STM324xG-EVAL or the STM324x9I-EVAL board and the
    USB OTG FS peripheral are used, or when using the STM3210C-EVAL board.
  - in HS-IN-FS when the STM324x9I-EVAL board and the USB OTG HS-IN-FS peripheral are used.


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
