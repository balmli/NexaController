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
            send(nd.device, nd.onoff);
            if (IS_LOG_PRIO(LOG_INFO)) {
                trace << PSTR("Nexa sent: ") << nd << endl;
            }
        }
    }
}

