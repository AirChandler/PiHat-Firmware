# PiHat Firmware for JACDAC Hardware Testing 
STM32F4 PiHat Firmware for Jacdac module hardware testing https://github.com/microsoft/jacdac-hw-testing

 Dev (recommended): /PiHat-Firmware/Build
 
 	- Build using: make
	- Flash using: openocd -f flash.cfg
 MakeCode firmware (unstable): https://github.com/AirChandler/jd-shield-firmware
 
 ## Service Commands
The PiHat currently understands 5 service_commands over the Jacdac

    - 0x00 - Broadcast- broadcasts the upcoming Jacdac packet size
    - 0x01 - set GPIO Pin- this sets a corresponding GPIO pin to a passed state (HIGH OR LOW) on the PiHat
    - 0x02 - get GPIO Pin- this sets the pull of a GPIO pin to a passed pull setting, then returns the current pin state
    - 0x03 - get Buttons- this returns the value of the current Button being pressed on the PiHat
    - 0x04 - get Analog- this reads the analog reading from the passed GPIO pin value (UNUSED)
