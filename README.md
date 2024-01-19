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

The 7-Segment elements are custom build and consist of 12V-LED stripes or bars. 

### Control Scheme

To control the high number of LED elements by a single microcontroller (4 x 2digits x 7 LED elements), the schematics uses shift registers for multiplexing.
Some great explanation of the functionality for a shift register can be found at https://www.mikrocontroller.net/articles/AVR-Tutorial:_Schieberegister
Each shift register provides 8 output channels and can be used to control one individual digit of the scoreboard. All shift registers can be cascaded (serial_out connected to serial_in of the next IC) and controlled via one single data pin.
The electrical signals we need are:
- **Serial Data**: Sequence of bit-values indicating if a specific output channel should be enabled/disabled. Due to the shifting, the first bit send to the shift registers will end up to be the last channel of the last register in the chain.
- **Clock signal**, that is in sync with the data signal. On each positive flank, the current value on the serial-data line will be stored in the 1st bit of an internal register of the IC (shift register) and already stored values will be shifted to the next address. Data, that was stored at the last position in the shift register will be send to serial_out / the next shift register IC). Depending on the specific IC, the PIN is usually called SCK or SRCK.
- **Release signal**. When all data has been transmitted thus the internal shift registers of the ICs have been properly set, a rising flank of this signal will cause the current values of the shift register to be transfered to the output register. Only then will the outputs of the ICs actually change state. The corresponding Pin is usually called RCK.
There are some additional signals, that are not essential but could be used for further control of the ICs:
- **Output activation signal** (Usually diaignated G). Can be used to enable/disable the outputs indipendent of the state of the output register. For the relevant ICs, 0V indicates outputs enabled. Therefore, if this function is not required, the corresponding pin can be wired to GND
- **Clear signal**: Can be used to erase all data from the shift register (Setting all bits to 0). If this function is not required, the corresponding pin can be wired to +5V. Default designator for the pin is SCL

For controling the shift registers, we can use the SPI-bus of the Arduino, since it already comes with a resource-efficient implementation for sending serial data with a synchronous clock signal. 
SPI is a three wire communication bus that is intended to connect a number of slave devices to one master device in parallel. The electrical signals are:
- SLK (or SCK / SCLK): Clock signal for synchronising Master and Slave - In our case this corresponds to SCK/SRCK of the shift registers
- MOSI (Master output Slave input) (also called PICO): Used to send data from the Master to the slave - In our case this corresponds to the Serial Data send to the input of the first shift register
- MISO (Master input Slave Output) (also called POCI): Used to send data back to the master - In our case not required

Since all slave devices are connected to the SPI bus in parallel, there must be a mechanism to identify the device that is selected for communication. For this purpose, the SPI-bus uses an additional signal / wire for each slave device, that enables communcication. This pin usually is designated as SS (slave select). The default state of the SS-lines is +5V. During communication with one individual slave, its SS-line will be pulled to GND (for the duration of the communication).

The Shift registers have no slave select pin. However, it is still possible to use these devices in parallel with outher SPI devices by using the RCK pin (Release signal). Although the ICs will process all data (also data meant for other SPI-devices on the bus) and change the internal shift registers accordingly, there won't be any unintended change to the storage register and output channels without setting the RCK-signal. Since the Transfer from shift- to output registers is triggered by a positive flank on the RCK-line, the SS-select signal can be used and directly being hooked to the RCK pin. One should be aware, that when using multiple devices on the SPI-bus, the state of the shift registers might have some arbitrary state after some time. Before enabling the release signal, one must always (re-)set the shift-registers to its correct/intended state.

### Hardware

A convenient choice is the TPIC6B595N. The advantage is, that its open-drain DMOS transistors are intended for high current operation and can directly drive the LED-elements.  With 8 outputs, one TPIC6B595N can provide current to all elements of one 7-segment display. The TPIC6B595N has a continuous sink-current capability of 150mA on each output channel.

If the LED-stripes to be used would sink more than 150mA () the TPIC6B595N would have to be reduced. I could not find suitable devices, that combine the shifter funtionality with the high current output capability. It would probably be advisable to use a dedicated (low current) shifter like a 74HC595 in combination with a multi-channel transistor IC like the ULN2803A. The ULN2803A is capable of sinking 500mA continuously.   

