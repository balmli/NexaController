#include "nexaconfigstore.h"

#include "Cosa/Trace.hh"


IOStream& operator<<(IOStream& outs, NexaConfigStore::nexadevice_t nd) {
    outs << PSTR("house = ") << nd.house
            << PSTR(", device = ") << nd.device
            << PSTR(", on/off = ") << nd.onoff
            << PSTR(", at ") << nd.daymask
            << PSTR(" ") << nd.hour
            << PSTR(":") << nd.minute;
    return (outs);
}

IOStream& operator<<(IOStream& outs, NexaConfigStore::nexaremote_t nd) {
    outs << PSTR("houseRc = ") << nd.houseRc
            << PSTR(", deviceRc = ") << nd.deviceRc
            << PSTR(", house = ") << nd.house
            << PSTR(", device = ") << nd.device;
    return (outs);
}

void NexaConfigStore::writeToEeprom() {
    uint8_t* pntr = 0;
    unsigned int numBytes = 0;
    nexaconfig_t cfg(numDevices, numRemotes);
    int ret = _eeprom->write(pntr, &cfg, sizeof (cfg));
    _eeprom->write_await();
    pntr += ret;
    numBytes += ret;

    for (uint8_t i = 0; i < numDevices; i++) {
        nexadevice_t nd = nexaDevices[i];
        ret = _eeprom->write(pntr, &nd, sizeof (nd));
        _eeprom->write_await();
        pntr += ret;
        numBytes += ret;
    }
    for (uint8_t i = 0; i < numRemotes; i++) {
        nexaremote_t nr = nexaRemotes[i];
        ret = _eeprom->write(pntr, &nr, sizeof (nr));
        _eeprom->write_await();
        pntr += ret;
        numBytes += ret;
    }

    if (IS_LOG_PRIO(LOG_INFO)) {
        trace << PSTR("Written config to EEPROM: ") << numDevices << PSTR(" devices. ") << numRemotes << PSTR(" remotes. ") << numBytes << PSTR(" bytes.") << endl;
    }
}

void NexaConfigStore::readFromEeprom() {
    uint8_t* pntr = 0;
    unsigned int numBytes = 0;
    nexaconfig_t cfg;
    int ret = _eeprom->read(&cfg, pntr, sizeof (cfg));
    pntr += ret;
    numBytes += ret;

    numDevices = 0;
    numRemotes = 0;

    nexadevice_t nd;
    for (uint8_t i = 0; i < cfg.numDevices; i++) {
        ret = _eeprom->read(&nd, pntr, sizeof (nd));
        pntr += ret;
        numBytes += ret;
        add(&nd);
    }
    nexaremote_t nr;
    for (uint8_t i = 0; i < cfg.numRemotes; i++) {
        ret = _eeprom->read(&nr, pntr, sizeof (nr));
        pntr += ret;
        numBytes += ret;
        addRc(&nr);
    }

    if (IS_LOG_PRIO(LOG_INFO)) {
        trace << PSTR("Read config from EEPROM: ") << numDevices << PSTR(" devices. ") << numRemotes << PSTR(" remotes. ") << numBytes << PSTR(" bytes.") << endl;
    }
}

void NexaConfigStore::add(nexadevice_t* nd) {
    add(nd->house, nd->device, nd->onoff, nd->hour, nd->minute);
}

void NexaConfigStore::add(int32_t house, uint8_t device, uint8_t onoff, uint8_t hour, uint8_t minute, uint8_t daymask) {
    if (numDevices < MAX_DEVICES) {
        nexaDevices[numDevices].house = house;
        nexaDevices[numDevices].device = device;
        nexaDevices[numDevices].onoff = onoff;
        nexaDevices[numDevices].daymask = daymask;
        nexaDevices[numDevices].hour = hour;
        nexaDevices[numDevices].minute = minute;
        if (IS_LOG_PRIO(LOG_DEBUG)) {
            trace << PSTR("NexaController::add: ") << numDevices << PSTR(": ") << nexaDevices[numDevices] << endl;
        }
        numDevices++;
    } else {
        TRACE("Max number of Nexa devices reached");
    }
}

void NexaConfigStore::addRc(nexaremote_t* nr) {
    addRc(nr->houseRc, nr->deviceRc, nr->house, nr->device);
}

void NexaConfigStore::addRc(int32_t houseRc, uint8_t deviceRc, int32_t house, uint8_t device) {
    if (numRemotes < MAX_REMOTES) {
        nexaRemotes[numRemotes].houseRc = houseRc;
        nexaRemotes[numRemotes].deviceRc = deviceRc;
        nexaRemotes[numRemotes].house = house;
        nexaRemotes[numRemotes].device = device;
        if (IS_LOG_PRIO(LOG_DEBUG)) {
            trace << PSTR("NexaController::addRc: ") << numRemotes << PSTR(": ") << nexaRemotes[numRemotes] << endl;
        }
        numRemotes++;
    } else {
        TRACE("Max number of Nexa remotes reached");
    }
}
