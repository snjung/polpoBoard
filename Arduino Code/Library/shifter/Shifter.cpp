/*
  Shifter.cpp - Library
  Code for driving multiple common Anode 7 Segment Displays using the TI TPIC6B595 8-Bit Shift Register
  This has been written for use with the Arduino UNO. Roy Fisher
  25.02.2011
  Released into the public domain.

  Connect between shift registers:-
  SRCLRPin, SRCKPin, RCKPin and connect the SEROUT to SERIN of each cascaded shift register
  Make sure the G Pin is grounded
  Make sure SRCLR is tied to 5V
*/

#include "Arduino.h"
#include "Shifter.h"

Shifter::Shifter(int NumOfDigits, int SRCKPin, int SERINPin, int RCKPin)
{
  _NumOfDigits = NumOfDigits;
  pinMode(SRCKPin, OUTPUT);
  _SRCKPin = SRCKPin;
  pinMode(SERINPin, OUTPUT);
  _SERINPin = SERINPin;
  pinMode(RCKPin, OUTPUT);
  _RCKPin = RCKPin;    
}

void Shifter::clear()
{
  int x; 
  digitalWrite(_RCKPin, LOW);  
  // shift out 0's to clear display
  for (x = _NumOfDigits; x >= 0; x--)
    {
      shiftOut(_SERINPin, _SRCKPin, LSBFIRST, 0); 
    }
  digitalWrite(_RCKPin, HIGH);
}

int Shifter::display(int intNumber)
{ // Displays number on display
  int SegmentArray[] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 230};
  String strDisplay;
  int x;
  int res;
  int shiftword;
  if (intNumber < 10)
  {
    strDisplay =  "0" + String(intNumber, DEC);           // using an int and a base
  }
  else 
  {
    strDisplay = String(intNumber, DEC);
  }
  
  digitalWrite(_RCKPin, LOW);
  for (x = _NumOfDigits ; x >= 0; x--)
  {
  res = strDisplay[x] - '0';
  shiftword = SegmentArray[res];
  // shift out the bits
  shiftOut(_SERINPin, _SRCKPin, LSBFIRST, shiftword);
  }
  digitalWrite(_RCKPin, HIGH);
}

