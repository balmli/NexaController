#ifndef __RGBLED_HH__
#define __RGBLED_HH__

#include "config.h"

#include "Cosa/Pins.hh"

#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE [0,0,255]
#define YELLOW [255,255,0]
#define WHITE [255,255,255]

class RgbLed {
protected:
  PWMPin _redPin;
  PWMPin _greenPin;
  PWMPin _bluePin;
public:
    RgbLed(Board::PWMPin redPin, Board::PWMPin greenPin, Board::PWMPin bluePin) :
    _redPin(redPin), 
    _greenPin(greenPin), 
    _bluePin(bluePin)
    {
    }
    
    void off() {
      _redPin.write(255);
      _greenPin.write(255);
      _bluePin.write(255);
    }
    void white() {
      _redPin.write(0);
      _greenPin.write(0);
      _bluePin.write(0);
    }
    void red() {
      _redPin.write(0);
      _greenPin.write(255);
      _bluePin.write(255);
    }
    void green() {
      _redPin.write(255);
      _greenPin.write(0);
      _bluePin.write(255);
    }
    void blue() {
      _redPin.write(255);
      _greenPin.write(255);
      _bluePin.write(0);
    }
    void yellow() {
      _redPin.write(0);
      _greenPin.write(0);
      _bluePin.write(255);
    }
    
    void setColor(uint8_t c[3]) {
      _redPin.write(255 - c[0]);
      _greenPin.write(255 - c[1]);
      _bluePin.write(255 - c[2]);
    }

};

#endif

