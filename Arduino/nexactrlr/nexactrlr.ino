
#include "Cosa/EEPROM.hh"
#include "Cosa/Event.hh"
#include "Cosa/ExternalInterrupt.hh"
#include "Cosa/Linkage.hh"
#include "Cosa/Trace.hh"
#include "Cosa/IOBuffer.hh"
#include "Cosa/IOStream/Driver/UART.hh"
#include "Cosa/IOStream/Driver/UART.cpp"
#include "Cosa/Pins.hh"
#include "Cosa/RTC.hh"
#include "Cosa/Watchdog.hh"

#include "Cosa/Driver/NEXA.hh"
#include "Cosa/Driver/NEXA.cpp"

#include "Cosa/INET/DHCP.hh"
#include "Cosa/INET/DHCP.cpp"
#include "Cosa/INET/DNS.hh"
#include "Cosa/INET/DNS.cpp"
#include "Cosa/INET/NTP.hh"
#include "Cosa/INET/NTP.cpp"
#include "Cosa/Socket/Driver/W5100.hh"
#include "Cosa/Socket/Driver/W5100.cpp"

#include "config.h"
#include "rtcclock.h"
#include "clockint.h"
#include "nexatransmitter.h"
#include "nexareceiver.h"
#include "rgbled.h"
#include "ethernet.h"
#include "initializer.h"
#include "commandparser.h"
#include "nexalogger.h"
#include "nexaconfigstore.h"
#include "serialcommandreader.h"
#include "commandhandler.h"

RgbLed rgbLed(Board::PWM1, Board::PWM2, Board::PWM3);
EthernetClient ethernet;
EEPROM eeprom;
RtcClock ds3231;
ClockInterrupt clockInt(Board::EXT1);
NexaConfigStore nexaConfigStore(&eeprom);
NexaReceiver receiver(Board::EXT0);
NexaTransmitter nexaTransmitter(Board::D7, &ds3231, &nexaConfigStore, &receiver, &rgbLed);
NexaLogger nexaLogger;
CommandHandler commandHandler(&ds3231);
CommandParser commandParser(&commandHandler, &nexaConfigStore);
SerialCommandReader serialCommandReader(&commandParser);

void setup() {
    rgbLed.red();
    uart.begin(9600);
    trace.begin(&uart, PSTR("ArdNexa started"));
    trace_log_mask = LOG_UPTO(LOG_INFO);
    //trace_log_mask = LOG_UPTO(LOG_DEBUG);

    Watchdog::begin(16, SLEEP_MODE_IDLE, Watchdog::push_timeout_events);
    RTC::begin();

    Initializer initializer(&nexaConfigStore);
    initializer.init();
    ds3231.begin();
    clockInt.attach(&ds3231);
    clockInt.attach(&nexaTransmitter);
    clockInt.enable();

    receiver.enable();
    ds3231.setRtc(ethernet.getTime());
    rgbLed.off();
}

void loop() {
    Event event;
    if (Event::queue.dequeue(&event)) {
        event.dispatch();
    }
    else {
        serialCommandReader.checkSerialCommand();
        Power::sleep(SLEEP_MODE_IDLE);
    } 

    if (event.get_type() == NexaReceiver::REMOTE_CONTROL && NexaReceiver::queue.available() > 0) {
        NEXA::code_t cmd(0);
        NexaReceiver::queue.dequeue(&cmd);
        trace << PSTR("cmd: ") << cmd << endl;
        nexaTransmitter.sendRc(cmd.house, cmd.device, cmd.onoff);
    }

}

