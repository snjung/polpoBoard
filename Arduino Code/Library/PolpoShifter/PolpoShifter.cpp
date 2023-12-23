/*
  PolpoShifter.cpp - Library
  Code for driving multiple common Anode 7 Segment Displays using the TI TPIC6B595 8-Bit Shift Register
  This has been written for use with the Arduino UNO. 
  
  Connect between shift registers:-
  SRCLRPin, SRCKPin, RCKPin and connect the SEROUT to SERIN of each cascaded shift register
  Make sure the G Pin is grounded
  Make sure SRCLR is tied to 5V

*/

#include "Arduino.h"
#include "PolpoShifter.h"
#include "SPI.h"

Shifter::Shifter(int NumOfDigits, int confirmTransmissionPin)
{
  _NumOfDigits = NumOfDigits;
  pinMode(confirmTransmissionPin, OUTPUT);
  _confirmTransmissionPin = confirmTransmissionPin;
  SPI.begin();  
}

//Clears the full display by disabling all LEDs
void Shifter::clear()
{
  int x; 
  SPI.beginTransaction(SPISettings(20000, MSBFIRST, SPI_MODE0));
  digitalWrite(_confirmTransmissionPin, LOW);

  // shift out 0's to clear display
  for (x = _NumOfDigits; x >= 0; x--)
    {
      SPI.transfer(0);
    }
  digitalWrite(_confirmTransmissionPin, HIGH);
  SPI.endTransaction();  
}

int Shifter::display(char* numberToDisplay)
{
  //These sets the byte-values for the digits from 0 to 9
  //int numberArray[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
  //int numberArray[] = {207,192,107,233,228,173,175,200,239,237};
  int numberArray[] = {63,6,107,79,86,93,125,7,127, 95};
  int shiftWordArray[]={0,0,0,0,0,0,0,0};

  //int numberArray[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
  int x;
  int res;
  int shiftword;
  //Initiate the transmission, several parameters have to be set
  //speedMaximum: The maximum speed of communication
  //dataOrder: MSBFIRST or LSBFIRST
  //dataMode: SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3 Defines polarity and phases of the signals
  // take the confirmation Pin to LOW
  //  send in the data via via SPI:
  for (x = 0 ;x<_NumOfDigits ; x++)
  {
	//check if it is a dash, which means digit off
	if (numberToDisplay[x] == '-'){ 
		//turnoff the digit
  		shiftWordArray[x] = 0; 
	} else {
		//turn the character in to an int
  		res = numberToDisplay[x] - '0';
		//look up the bit pattern to display
  		shiftWordArray[x] = numberArray[res];
	}
  }
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
  for (x = 0 ;x<_NumOfDigits ; x++)
	{
	SPI.transfer(shiftWordArray[x]);
	}
  // take the SS pin high to de-select the chip:

  digitalWrite(_confirmTransmissionPin, LOW);
  digitalWrite(_confirmTransmissionPin, HIGH);
  SPI.endTransaction();  
}

