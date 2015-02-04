#ifndef __NEXA_CONFIG_STORE_H__
#define __NEXA_CONFIG_STORE_H__

#include "config.h"

#include "Cosa/Trace.hh"
#include "Cosa/EEPROM.hh"

class NexaConfigStore
{
public:
        union nexaconfig_t {

        struct {
            uint8_t numDevices;
            uint8_t numRemotes;
        };

        nexaconfig_t() {
        };

        nexaconfig_t(uint8_t nd, uint8_t nr) {
            numDevices = nd;
            numRemotes = nr;
        };
    };

    union nexadevice_t {

        struct {
            uint32_t house;
            uint8_t device;
            uint8_t onoff;
            uint8_t hour;
            uint8_t minute;
            uint8_t daymask;
        };

        nexadevice_t() {
        };

        nexadevice_t(int32_t h, uint8_t d, uint8_t f, uint8_t ho, uint8_t mi, uint8_t dm) {
            house = h;
            device = d;
            onoff = f;
            hour = ho;
            minute = mi;
            daymask = dm;
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
    uint8_t numDevices;
    uint8_t numRemotes;
    nexadevice_t nexaDevices[MAX_DEVICES];
    nexaremote_t nexaRemotes[MAX_REMOTES];

    EEPROM* _eeprom;

protected:

public:

    NexaConfigStore(EEPROM* ep) :
    numDevices(0),
    numRemotes(0)
    {
        _eeprom = ep;
    }

    uint8_t getNumDevices() { return numDevices; }
    uint8_t getNumRemotes() { return numRemotes; }
    void clearDevices() { numDevices = 0; }
    void clearRemotes() { numRemotes = 0; }
    nexadevice_t getDevice(uint8_t id) { return nexaDevices[id]; }
    nexaremote_t getRemote(uint8_t id) { return nexaRemotes[id]; }

    void writeToEeprom();
    void readFromEeprom();

    void add(nexadevice_t* nd);
    void add(int32_t house, uint8_t device, uint8_t onoff, uint8_t hour, uint8_t minute, uint8_t daymask = 0x7f);
    void addRc(nexaremote_t* nr);
    void addRc(int32_t houseRc, uint8_t deviceRc, int32_t house, uint8_t device);

};

#endif

