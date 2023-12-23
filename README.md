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

