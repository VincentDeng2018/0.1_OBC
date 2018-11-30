/**
  @page USB_HOST_DEVICE  USB Manual Dual role device example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Manual Dual role device example
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

This example provides a description of how to use the USB OTG Device/Host peripheral 
on the STM32F207xx, STM32F407xx, STM32F429xx and STM32F107xx devices.
In device mode The STM32 is enumerated as an USB Mass storage Device, that uses the
embedded microSD as storage media.
In Host mode, the STM32 behave as a mass storage Host that can enumerate, show content
and display the supported BMP image in the attached USB flash disk. 
The user has to select the USB mode (Host or Device) using the embedded joystick
following the menu displayed in the Bottom of the LCD display.

 @note Only the BMP files with the following format are supported : 
         - width  : 320
         - Height : 240
         - BPP    :  16
         - Compression : RGB bitmap with RGB masks  
       To use the Image browser, copy the BMP files from "\Utilities\Binary\Media"
       to the root of the USB flash disk to be used.   

 @note The USB Device library uses the multi-packet feature, this functionality increase 
       the transfer performance when using big amount of data. In the Mass storage demo, 
       an intermediary buffer is used to receive/send data from/to USB  and used also to 
       write/read from the physical media (microSD). The bigger is the buffer, the faster
       is the IO operations in USB and also in media; in order to use a balanced buffer size
       with the project footprint; the buffer size was defined in the usbd_conf.h file to 4K,
       but it can be modified by the user from 512 Bytes to 64KBytes.  

This example works
  - in high speed (HS) when the STM322xG-EVAL, STM324xG-EVAL or STM324x9I-EVAL boards and the 
    USB OTG HS peripheral are used
  - in full speed (FS) when the STM322xG-EVAL, the STM324xG-EVAL or the STM324x9I-EVAL board and the
    USB OTG FS peripheral are used, or when using the STM3210C-EVAL board.
  - in HS-IN-FS mode when STM324x9I-EVAL board and USB OTG HS-IN-FS peripheral are used

@par Hardware and Software environment 

   - This example has been tested with STM3210C-EVAL RevC (STM32F107xx devices), 
     STM322xG-EVAL RevC (STM32F207xx), STM324xG-EVAL RevC (STM32F407xx) and STM324x9I-EVAL RevB (STM32F429xx)     

  - STM3210C-EVAL Set-up 
    - use CN2 connector to connect the board to a PC host or USB device
		@limitation :
		 _ maximim size of SDcard to be used with STM3210C-EVAL is 2Go.
		 
  - STM322xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host or USB device when using USB OTG FS peripheral.
    - Use CN9 connector to connect the board to a PC host or USB device when using USB OTG HS peripheral.
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS 
		 
  - STM324xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
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

    
@note Known Limitations	  
    - Display BMP image is not managed within STM324x9I-EVAL eval board.
	
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
