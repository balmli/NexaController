#ifndef __NEXA_CONTROLLER_H__
#define __NEXA_CONTROLLER_H__

#include "rtcclock.h"
#include <Cosa/Linkage.hh>
#include <Cosa/Driver/NEXA.hh>

class NexaController : 
public NEXA::Transmitter, public Link {

public:
  union nexadevice_t {
    struct {
      uint32_t house;
      uint8_t device; 		
      uint8_t onoff; 		
      uint8_t hour;
      uint8_t minute;
    };

    nexadevice_t() {
    };
    
    nexadevice_t(int32_t h, uint8_t d, uint8_t f, uint8_t ho, uint8_t mi) { 
      house = h;
      device = d;
      onoff = f;
      hour = ho;
      minute = mi;
    }

    friend IOStream& operator<<(IOStream& outs, nexadevice_t code);
  };

private:
  static const uint8_t MAX_DEVICES = 16;
  RtcClock* _rtcClock;
  uint8_t numDevices;
  nexadevice_t nexaDevices[MAX_DEVICES];
  
protected:
  virtual void on_event(uint8_t type, uint16_t value);

public:  
  NexaController(Board::DigitalPin nexaTransmitterPin, RtcClock* rtc) : 
  Link(),
  NEXA::Transmitter(nexaTransmitterPin),
  numDevices(0)
  {
    _rtcClock = rtc;
  }

  void add(nexadevice_t* nd);
  void add(int32_t house, uint8_t device, uint8_t onoff, uint8_t hour, uint8_t minute); 
};

#endif


