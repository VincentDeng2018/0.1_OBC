/**
  @page USBD_DualCore  USB Device Dual Core example

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2017
  * @brief   Description of the USB Device Dual Core example
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

The USB Device Dual Core example uses the STM32F207xx, STM32F429xx and STM32F407xx multi core support
feature and integrates the Mass Storage and HID examples in the same project.
The Mass storage device should be connected to the USB HS port while the HID device
should be connected to the FS port.

  @note The USB Device library uses the multi-packet feature, this functionality increase
        the transfer performance when using big amount of data. In   the Mass storage demo,
        an intermediary buffer is used to receive/send data from/to USB  and used also to
        write/read from the physical media (microSD). The bigger is the buffer, the faster
        is the IO operations in USB and also in media; in order to use a balanced buffer size
        with the project footprint; the buffer size was defined in the usbd_conf.h file 
		"MSC_MEDIA_PACKET" to 4K, but it can be modified by the user from 512 Bytes to 64KBytes.


@par Hardware and Software environment

   - This example has been tested STM322xG-EVAL RevC (STM32F207xx), STM324xG-EVAL RevC (STM32F407xx) and STM324x9I-EVAL RevB (STM32F429xx)

  - STM322xG-EVAL Set-up
    - Use both CN8 and CN9 connectors to connect the board to a PC host
    - Make sure that a microSD card is plugged into CN6 connector
          - STM322xG-EVAL RevC: make sure that Jumpers JP22 and JP16 are in position 1<->2
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS

  - STM324xG-EVAL Set-up
    Use both CN14 and CN9 connectors to connect the board to a PC host
    - Make sure that a microSD card is plugged into CN6 connector, and Jumpers
      JP22 & JP16 are in position 1<->2
		@note Make sure that :
		 - Please ensure that jumper JP31 is fitted when using USB OTG HS

  - STM324x9I-EVAL Set-up
    - Use CN14 connector to connect the board to host when using USB OTG FS peripheral
    - Use CN9  connector to connect the board to host when using USB OTG HS peripheral
      Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM324x9I-EVAL Evaluation Board
              User Manual.
        @note Make sure that :
         - Please ensure that jumper JP16 is not fitted when using USB OTG FS

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain
	- In the workspace toolbar select the project config:
	- STM322xG-EVAL_USBD-HS_FS: to configure the project for STM32F207xx devices and use USB OTG HS and OTG FS peripherals
	- STM324xG-EVAL_USBD-HS_FS: to configure the project for STM32F407xx devices and use USB OTG HS and OTG FS peripherals
	- STM324x9i-EVAL_USBD-HS_FS :to configure the project for STM32F429xx devices and use USB OTG HS and OTG FS peripherals

  - Rebuild all files and load your image into target memory
  - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
