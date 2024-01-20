# polpoBoard

## Introduction
This repository compiles code and documentation for an electronic scoreboard. It has been successfully operated for several years and was continuously updated and improved.
The idea and inspiration comes from the awesome folks from Westbury on Severn Cricket Club
You can find their blog with tons of useful information on the project at https://buildyourownscoreboard.wordpress.com/

The main idea is to have a Raspberry Pi acting as a web-server and providing control of the scoreboard via a simple webpage to connected devices. The Raspberry Pi will transmit the data to be shown on the scoreboard to an arduino board via serial interface. The Arduino will then control the hardware. Currently this includes
- Custom Build 7-segment elements to show goals and time left of the current game
- A 12V car horn used for signaling end of game / half-time
- Display-Panels build of 8x8 dot-matrix LED elements to show the names of the teams

### Hardware

#### 7-Segment Elements

The 7-Segment elements are custom bild and consist of 12V-LED stripes or bars. 

To control the high number of LED elements by a single microcontroller (4 x 2digits x 7 LED elements), the schematics uses shift registers for multiplexing.

A convenient choice is the TPIC6B595N. The advantage is, that its open-drain DMOS transistors are intended for high current operation and can directly drive the LED-elements.  With 8 outputs, one TPIC6B595N can provide current to all elements of one 7-segment display. The TPIC6B595N has a continuous sink-current capability of 150mA on each output channel.

If the LED-stripes to be used would sink more than 150mA () the TPIC6B595N would have to be reduced. I could not find suitable devices, that combine the shifter funtionality with the high current output capability. It would probably be advisable to use a dedicated (low current) shifter like a 74HC595 in combination with a multi-channel transistor IC like the ULN2803A. The ULN2803A is capable of sinking 500mA continuously.   

### Software

#### Arduino Softwa

#### Serial Communication

Raspberry Pi and Arduino have to exchange information on the scoreboard status via serial port. The Arduino Firmware uses  CmdMessenger by Thijs Elenbaas to implement the serial communication. Project repository can be found at https://github.com/thijse/Arduino-CmdMessenger. CmdMessengers message format:

``Cmd Id, param 1, [...] , param N;``

Default field seperator is a comma, default command sepeartor is semi-colon.
For our application, the seperators are changed to:
- field seperator: ,
- command seperator: #

There is a Python client to communicate with an Arduino implementing this protocol: https://github.com/harmsm/PyCmdMessenger

#### Control of 7-segment elements

For controlling the 7-segment displays via the TPIC6B595N, the communication and control of the shift register has to be done via SPI. The specific control scheme to control the drain-outputs of the shift registers is implemented in a simple library called polposhifter. This library uses the standard SPI-library that is pre-installed with arduino IDE.

#### Raspberry Pi

For setting-up the Raspberry Pi one can closely followed along the instructions from https://buildyourownscoreboard.wordpress.com/setting-up-the-raspberry-jessie/
The instructions are based on Raspeerry OS 8 (Jessie) and php versiion 5. I used more up-to-date software versions, which works with minimal modification to the. The main steps are:
- Download and install the Raspberry imager from  https://www.raspberrypi.com/software/
- Use this tool to prepare the micro-SD card for the Raspberry (I use a Rev 3 B+ ) with Raspberry Pi OS in lite version (without desktop environement). The Version proposed and used by the installer was bullseye (version 11)
- All further steps can be followed from the linked instructions, with some minor adjustements:
  - When doing the configuration modifications (sudo raspi-config), the mentioned options to be changed are not part of advanced options but instad can be found unter inderface options. When deactivation the serial console I1 lets you activate SSH remote command line access. I6 lets you disable shell and kernel messages on the serial connection. It also asks you, if serial port hardware should be deactivated (choose no) 
  - Installing more recent php-version is straight forward. Just use sudo apt-get install php (instead of php5) and libapache2-mod-phpxxx with the specific php-version that apt-get got tfromit's repositories.
  
  The main operation principle is as follows:
  - The Raspberry hosts an apache webserver, delivering an interactive webpage to connected clients (PC, tablet or smartphone). This has the required interface elements to control the scorepoard and GUI elements to show its current status.
  - The Apache Server runs with enhanced rights (AllowOverride set to All, apache user www-data has user rights to dialout / access serial interface)
  - The web-interface uses javascript routines to implement the scoreboards logic. In essence, all logic is implemented on this level, the arduino is just getting information via serial port on the content, that should be shown on the physical scoreboard. The logic specific javascript-code is part of scoreboard.js.   