#include "nexatransmitter.h"

#include "Cosa/Watchdog.hh"
#include "Cosa/Trace.hh"

void NexaTransmitter::on_event(uint8_t type, uint16_t value) {
    time_t now;
    _rtcClock->get_time(now);
    now.to_binary();

    _rgbLed->yellow();
    for (uint8_t i = 0; i < _nexaConfigStore->getNumDevices(); i++) {
        NexaConfigStore::nexadevice_t nd = _nexaConfigStore->getDevice(i);
        uint8_t dm = (nd.daymask >> (now.day - 1)) & 0x1;
        if (IS_LOG_PRIO(LOG_DEBUG)) {
            trace << PSTR("nexa device: ") << i << PSTR(": ") << nd << endl;
            trace << PSTR("daymask calc: ") << dm << PSTR(" daymask:") << nd.daymask << PSTR(" day:") << now.day << endl;
        }

        if (now.hours == nd.hour && now.minutes == nd.minute && dm == 0x1) {
          sendCommand(nd.house, nd.device, nd.onoff);
        }
    }
    _rgbLed->off();
}

void NexaTransmitter::sendRc(uint32_t houseRc, uint8_t deviceRc, uint8_t onoff) {
    for (uint8_t i = 0; i < _nexaConfigStore->getNumRemotes(); i++) {
        NexaConfigStore::nexaremote_t nr = _nexaConfigStore->getRemote(i);
        if (IS_LOG_PRIO(LOG_DEBUG)) {
            trace << PSTR("nexa remote: ") << i << PSTR(": ") << nr << endl;
        }
        if (nr.houseRc == houseRc && nr.deviceRc == deviceRc) {
          sendCommand(nr.house, nr.device, onoff);
        }
    }
    _rgbLed->blue();
}

void NexaTransmitter::sendCommand(uint32_t house, uint8_t device, uint8_t onoff) {
  _rgbLed->red();
  set_house(house);
  _nexaReceiver->disable();
  for (uint8_t i = 0; i < NEXA_TRANSMIT_REPEAT; i++) {
    send(device, onoff);
    Watchdog::delay(16);
    if (IS_LOG_PRIO(LOG_INFO)) {
        trace << PSTR("Nexa sent: ") << house << PSTR(" ") << device << PSTR(": on/off: ") << onoff << endl;
    }
  }
  _nexaReceiver->enable();
}
