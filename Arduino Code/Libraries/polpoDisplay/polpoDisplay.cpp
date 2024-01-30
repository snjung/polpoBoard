#include "Arduino.h"
#include "PolpoDisplay.h"
#include "SPI.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_ZONES 2
#define ZONE_SIZE 13
#define MAX_DEVICES (MAX_ZONES * ZONE_SIZE)
#define SCROLL_SPEED  30
#define ZONE_UPPER  1
#define ZONE_LOWER  0

#define CLK_PIN   13
#define DATA_PIN  11

polpoDisplay::polpoDisplay(int CS_PIN)
{
  _CS_PIN = CS_PIN;

  _P = new MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

  _P ->begin(MAX_ZONES); //Begin with 2 zones
  _P ->setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  _P ->setFont(ZONE_LOWER, BigFontLower);  

  _P ->setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES-1);
  _P ->setFont(ZONE_UPPER, BigFontUpper);

  _P ->setCharSpacing(_P ->getCharSpacing() * 2); // double height --> double spacing
}

void polpoDisplay::display(char* textToDisplay)
{
  // Run the animation and then check if BOTH zones have
  // completed. The animations are not the same length due
  // to upper/lower effects being displayed differently.
  _P ->displayAnimate();
  if (_P ->getZoneStatus(ZONE_LOWER) && _P ->getZoneStatus(ZONE_UPPER))
    {
    _P ->setFont(ZONE_LOWER, BigFontLower);
    _P ->setFont(ZONE_UPPER, BigFontUpper);
    _P ->displayZoneText(ZONE_LOWER, textToDisplay, PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    _P ->displayZoneText(ZONE_UPPER, textToDisplay, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    _P ->displayClear();
    _P ->synchZoneStart();
    }
    // synchronise the start
  
}