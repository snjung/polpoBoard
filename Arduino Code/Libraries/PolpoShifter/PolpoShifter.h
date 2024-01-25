/*
  PolpoShifter.h - Library
  Code for driving multiple common Anode 7 Segment Displays using the TI TPIC6B595 8-Bit Shift Register
  This has been written for use with the Arduino UNO

  Connect between shift registers:-
  SRCLRPin, SRCKPin, RCKPin and connect the SEROUT to SERIN of each cascaded shift register
  Make sure the G Pin is grounded
  Make sure SRCLR is tied to 5V

*/

//Prevents someone from including the library twice

#ifndef PolpoShifter_h
#define PolpoShifter_h

#include <Arduino.h>
#include <SPI.h>

class Shifter
{
  public:
    Shifter(int NumOfDigits, int confirmTransmissionPin);
    void clear();
    int display(char* numberToDisplay);
    
  private:
	//would it make sense to declare numberToDisplay as native c char?
    String _numberToDisplay;
    int _NumOfDigits;
    int _confirmTransmissionPin;
  
};

#endif

