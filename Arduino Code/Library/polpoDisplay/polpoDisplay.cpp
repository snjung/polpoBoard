#include "Arduino.h"
#include "PolpoDisplay.h"
#include "SPI.h"

Display::Display(int width, int CS_PIN)
{
  _width = width;
  pinMode(CS_PIN, OUTPUT);
  _CS_PIN = CS_PIN;
  SPI.begin();

  #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
  #define ZONE_SIZE _width
  #define MAX_DEVICES (2 * _width)
  #define SCROLL_SPEED  30
  #define ZONE_UPPER  1
  #define ZONE_LOWER  0

  #define CLK_PIN   13
  #define DATA_PIN  11
  #define CS_PIN    10

  MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
  bool invertUpperZone = false; // Hardware adaptation parameters for scrolling

  P.begin(MAX_ZONES);
  P.setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  P.setFont(ZONE_LOWER, BigFontLower);  

  P.setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES-1);
  P.setFont(ZONE_UPPER, BigFontUpper);

  P.setCharSpacing(P.getCharSpacing() * 2); // double height --> double spacing
}

void Display::display(char* textToDisplay){
}

void Display::display(char* textToDisplay)
{
  // Run the animation and then check if BOTH zones have
  // completed. The animations are not the same length due
  // to upper/lower effects being displayed differently.
  P.displayAnimate();
  if (P.getZoneStatus(ZONE_LOWER) && P.getZoneStatus(ZONE_UPPER))
  {
      P.setFont(ZONE_LOWER, BigFontLower);
      P.setFont(ZONE_UPPER, BigFontUpper);
      P.displayZoneText(ZONE_LOWER, textToDisplay, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayZoneText(ZONE_UPPER, textToDisplay, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
      break;
    // synchronise the start
    P.displayClear();
    P.synchZoneStart();
  }
}