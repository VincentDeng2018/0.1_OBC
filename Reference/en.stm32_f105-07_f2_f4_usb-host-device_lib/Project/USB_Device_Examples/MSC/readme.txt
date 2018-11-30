/**
  @page USBD_MSC  USB Device Mass Storage example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Device Mass Storage example
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

The Mass Storage example gives a typical example of how to use the STM32F207xx, STM32F407xx,
STM32F429xx and STM32F107xx USB OTG Device peripheral to communicate with a PC Host using the bulk
transfer while the microSD card is used as storage media.
This example supports the BOT (bulk only transfer) protocol and all needed SCSI
(small computer system interface) commands, and is compatible with both Windows
XP (SP1/SP2) and Windows 2000 (SP4).
After running the example, the user just has to plug the USB cable into a PC host
and the device is automatically detected. A new removable drive appears in the
system window and write/read/format operations can be performed as with any other
removable drive.

  @note The USB Device library uses the multi-packet feature, this functionality increase 
        the transfer performance when using big amount of data. In   the Mass storage demo, 
        an intermediary buffer is used to receive/send data from/to USB  and used also to 
        write/read from the physical media (microSD). The bigger is the buffer, the faster
        is the IO operations in USB and also in media; in order to use a balanced buffer size
        with the project footprint; the buffer size was defined in the usbd_conf.h file to 4K,
        but it can be modified by the user from 512 Bytes to 64KBytes.
        
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
    - use CN2 connector to connect the board to a PC host
    - Make sure that a microSD card is plugged into CN16 connector
		@limitation :
		 _ maximim size of SDcard to be used with STM3210C-EVAL is 2Go.
		 
  - STM322xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
    - Make sure that a microSD card is plugged into CN6 connector
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS           
		 - Make sure that Jumpers JP22 and JP16 are in position 1<->2

  - STM324xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
    - Make sure that a microSD card is plugged into CN6 connector
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS           
		 - Make sure that Jumpers JP22 and JP16 are in position 1<->2
		  
  - STM324x9I-EVAL Set-up
    - Use CN14 connector to connect the board to host when using USB OTG FS peripheral
    - Use CN9  connector to connect the board to host when using USB OTG HS peripheral
    - Use CN15  connector to connect the board to host when using USB OTG HS-IN-FS peripheral
      Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM324x9I-EVAL Evaluation Board 
              User Manual.
        @note Make sure that :
          - Please ensure that jumper JP16 is not fitted when using USB OTG FS
          - Make sure that a microSD card is plugged into CN17 connector

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
