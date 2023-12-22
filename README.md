# polpoBoard

## Introduction
This repository compiles code and documentation for an electronic scoreboard. It has been successfully operated for several years and was continuously updated and improved.
The idea and inspiration comes from the awesome folks from Westbury on Severn Cricket Club
You can find their blog with tons of useful information on the project at https://buildyourownscoreboard.wordpress.com/

The main idea is to have a Raspberry Pi acting as a web-server and providing control of the scoreboard via a simple webpage to connected devices. The Raspberry Pi will transmit the data to be shown on the scoreboard to an arduino board via serial interface. The Arduino will then control the hardware. Currently this includes
- Custom Build 7-segment elements to show goals and time left of the current game
- A 12V car horn used for signaling end of game / half-time
- Display-Panels build of 8x8 dot-matrix LED elements to show the names of the teams
