#ifndef __NEXA_CONTROLLER_H__
#define __NEXA_CONTROLLER_H__

#include "rtcclock.h"
#include "nexareceiver.h"

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

    union nexaremote_t {

        struct {
            uint32_t houseRc;
            uint8_t deviceRc;
            uint32_t house;
            uint8_t device;
        };

        nexaremote_t() {
        };

        nexaremote_t(int32_t hRc, uint8_t dRc, int32_t h, uint8_t d) {
            houseRc = hRc;
            deviceRc = dRc;
            house = h;
            device = d;
        }

        friend IOStream& operator<<(IOStream& outs, nexaremote_t code);
    };

private:
    static const uint8_t MAX_DEVICES = 20;
    static const uint8_t MAX_REMOTES = 20;
    RtcClock* _rtcClock;
    NexaReceiver* _nexaReceiver;
    uint8_t numDevices;
    uint8_t numRemotes;
    nexadevice_t nexaDevices[MAX_DEVICES];
    nexaremote_t nexaRemotes[MAX_REMOTES];

protected:
    virtual void on_event(uint8_t type, uint16_t value);

public:

    NexaController(Board::DigitalPin nexaTransmitterPin, RtcClock* rtc, NexaReceiver* nr) :
    Link(),
    NEXA::Transmitter(nexaTransmitterPin),
    numDevices(0) {
        _rtcClock = rtc;
        _nexaReceiver = nr;
    }

    void add(nexadevice_t* nd);
    void add(int32_t house, uint8_t device, uint8_t onoff, uint8_t hour, uint8_t minute);
    void addRc(int32_t houseRc, uint8_t deviceRc, int32_t house, uint8_t device);
    void sendRc(uint32_t houseRc, uint8_t deviceRc, uint8_t onoff);
};

#endif


