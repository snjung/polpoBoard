/*
  PolpoShifter.cpp - Library
  Code for driving multiple common Anode 7 Segment Displays using cascaded shift-registers like the TI TPIC6B595
  
  The TI TPIC6B595 communicates via SPI protocol. Several ICs can be daisy-chained, to have this cascaded setup work, 
  SEROUT of each IC has to be connected to SERIN of the next IC in line.
  SRCLRPin, SRCKPin, RCKPin will be identical signals for all ICs (must all be connected to same net / potential coming from corresponding PINs of the Arduino)
  Make sure the G Pin is grounded
  Make sure SRCLR is tied to 5V
*/

#include "Arduino.h"
#include "PolpoShifter.h"
#include "SPI.h"

// A Shifter-Object represents a cascaded setup of TPIC6B595 ICs (one or more)
// The confirmTransmissionPin is used for signaling, that transferring the serial data to the internal shift-registers of the ICs is completed
// and it's status can be transfered to the output registers (thus the output channels of the ICs)
// Regarding SPI-standards, this is identical to the Slave-Select (SS) Pin. In terms of shift registers, this is identical to the RCK Pin
Shifter::Shifter(int NumOfDigits, int confirmTransmissionPin)
{
  _NumOfDigits = NumOfDigits;
  pinMode(confirmTransmissionPin, OUTPUT);
  _confirmTransmissionPin = confirmTransmissionPin;
  SPI.begin();
}

// Clears the full display by disabling all LEDs
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

/*
Will set Configure the Shift-Registers to show a specific sequence of numbers
Valid input-characters are numbers from 0-9 and '-' (minus sign)
The minus sign will be interpreted as a deactivated (fully dark) 7-segment display element
*/
int Shifter::display(char* numberToDisplay)
{
  /*
  For each 7-segment element, an 8-bit value must be transmitted to represent the status of each individual line-element of the segment
  Depending on the specific wiring of the 7-segment line-elements to the output pins of the shift-register the 8-bit values for the accepted input chars might differ
  This is the configuration based on the standard used in this project (see documentation for details)
  */
  int numberArray[] = {63,6,107,79,86,93,125,7,127, 95};
  int shiftWordArray[]={0,0,0,0,0,0,0,0}; // This will hold the data to be send to the shift-register, initated as zero-array

  int x;
  int res;
  int shiftword;
  for (x = 0 ; x<_NumOfDigits ; x++)
  {
	//check if it is a dash, which means digit off
	if (numberToDisplay[x] == '-'){ 
		//turnoff the digit
  		shiftWordArray[x] = 0; 
	} else {
		//turn the character in to an int
  		res = numberToDisplay[x] - '0'; // This substracts ascii-value of char zero from the asci value to be displayed. For characters '0' to '9' this will result in an integer equal to the number itself
		//look up the bit pattern to display
  		shiftWordArray[x] = numberArray[res];
	}
  }

  //Initiate the transmission, several parameters have to be set
  //speedMaximum: The maximum speed of communication
  //dataOrder: MSBFIRST or LSBFIRST
  //dataMode: SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3 Defines polarity and phases of the signals
  // take the confirmation Pin to LOW
  //  send in the data via via SPI:
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

