/*
  PolpoDisplay.h - Library
  */

//Prevents someone from including the library twice

#ifndef PolpoDisplay_h
#define PolpoDisplay_h

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Font_Data.h>

class Display
{
  public:
    Display(int width, int CS_PIN);
    //void clear();
    void display(char* numberToDisplay);
    
  private:
	//would it make sense to declare numberToDisplay as native c char?
    //String _numberToDisplay;
    int _width;
    int _CS_PIN;
  
};

#endif

