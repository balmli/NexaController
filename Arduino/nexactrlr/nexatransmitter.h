#ifndef __NEXA_TRANSMITTER_H__
#define __NEXA_TRANSMITTER_H__

#include "config.h"
#include "rtcclock.h"
#include "nexareceiver.h"
#include "rgbled.h"
#include "nexaconfigstore.h"

#include "Cosa/Trace.hh"
#include "Cosa/Linkage.hh"
#include "Cosa/Driver/NEXA.hh"

#define NEXA_TRANSMIT_REPEAT 2

class NexaTransmitter :
public NEXA::Transmitter, public Link {
public:

private:
    RtcClock* _rtcClock;
    NexaConfigStore* _nexaConfigStore;
    NexaReceiver* _nexaReceiver;
    RgbLed* _rgbLed;

protected:
    virtual void on_event(uint8_t type, uint16_t value);

public:

    NexaTransmitter(Board::DigitalPin nexaTransmitterPin, RtcClock* rtc, 
        NexaConfigStore* nexaConfigStore, NexaReceiver* nr, RgbLed* rgbLed) :
    NEXA::Transmitter(nexaTransmitterPin),
    Link()
    {
        _rtcClock = rtc;
        _nexaConfigStore = nexaConfigStore;
        _nexaReceiver = nr;
        _rgbLed = rgbLed;
    }

    void sendRc(uint32_t houseRc, uint8_t deviceRc, uint8_t onoff);
    void sendCommand(uint32_t houseRc, uint8_t deviceRc, uint8_t onoff);
};

#endif


