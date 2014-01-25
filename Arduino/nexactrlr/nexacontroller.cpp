#include "nexacontroller.h"
#include <Cosa/Trace.hh>

IOStream& operator<<(IOStream& outs, NexaController::nexadevice_t nd) {
    outs << PSTR("house = ") << nd.house
            << PSTR(", device = ") << nd.device
            << PSTR(", on/off = ") << nd.onoff
            << PSTR(", at ") << nd.hour
            << PSTR(":") << nd.minute;
    return (outs);
}

IOStream& operator<<(IOStream& outs, NexaController::nexaremote_t nd) {
    outs << PSTR("houseRc = ") << nd.houseRc
            << PSTR(", deviceRc = ") << nd.deviceRc
            << PSTR(", house = ") << nd.house
            << PSTR(", device = ") << nd.device;
    return (outs);
}

void NexaController::add(nexadevice_t* nd) {
    add(nd->house, nd->device, nd->onoff, nd->hour, nd->minute);
}

void NexaController::add(int32_t house, uint8_t device, uint8_t onoff, uint8_t hour, uint8_t minute) {
    if (numDevices < MAX_DEVICES) {
        nexaDevices[numDevices].house = house;
        nexaDevices[numDevices].device = device;
        nexaDevices[numDevices].onoff = onoff;
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

void NexaController::addRc(int32_t houseRc, uint8_t deviceRc, int32_t house, uint8_t device) {
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

void NexaController::on_event(uint8_t type, uint16_t value) {
    time_t now;
    _rtcClock->get_time(now);
    now.to_binary();

    for (uint8_t i = 0; i < numDevices; i++) {
        nexadevice_t nd = nexaDevices[i];
        if (IS_LOG_PRIO(LOG_DEBUG)) {
            trace << PSTR("nexa device: ") << i << PSTR(": ") << nd << endl;
        }
        if (now.hours == nd.hour && now.minutes == nd.minute) {
            set_house(nd.house);
            _nexaReceiver->disable();
            send(nd.device, nd.onoff);
            _nexaReceiver->enable();
            if (IS_LOG_PRIO(LOG_INFO)) {
                trace << PSTR("Nexa sent: ") << nd << endl;
            }
        }
    }
}

void NexaController::sendRc(uint32_t houseRc, uint8_t deviceRc, uint8_t onoff) {
    for (uint8_t i = 0; i < numRemotes; i++) {
        nexaremote_t nr = nexaRemotes[i];
        if (IS_LOG_PRIO(LOG_DEBUG)) {
            trace << PSTR("nexa remote: ") << i << PSTR(": ") << nr << endl;
        }
        if (nr.houseRc == houseRc && nr.deviceRc == deviceRc) {
            set_house(nr.house);
            _nexaReceiver->disable();
            send(nr.device, onoff);
            _nexaReceiver->enable();
            if (IS_LOG_PRIO(LOG_INFO)) {
                trace << PSTR("Nexa sent: ") << nr << PSTR(": on/off: ") << onoff << endl;
            }
        }
    }
}

