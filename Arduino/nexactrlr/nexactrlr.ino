#include "rtcclock.h"
#include "clockint.h"
#include "nexacontroller.h"
#include "nexareceiver.h"

#include <Cosa/RTC.hh>
#include <Cosa/Watchdog.hh>
#include <Cosa/Trace.hh>
#include <Cosa/IOBuffer.hh>
#include <Cosa/IOStream/Driver/UART.hh>
#include <Cosa/Driver/NEXA.hh>
#include <Cosa/EEPROM.hh>

EEPROM eeprom;
RtcClock ds3231;
ClockInterrupt clockInt(Board::EXT1);
NexaReceiver receiver(Board::EXT0);
NexaController nexaCtrlr(Board::D12, &ds3231, &receiver, &eeprom);

void setup() {
    uart.begin(9600);
    trace.begin(&uart, PSTR("NexaController started"));
    trace_log_mask = LOG_UPTO(LOG_INFO);
    //trace_log_mask = LOG_UPTO(LOG_DEBUG);

    Watchdog::begin(16, SLEEP_MODE_IDLE, Watchdog::push_timeout_events);
    RTC::begin();

    initNexa();
    ds3231.begin();
    clockInt.attach(&ds3231);
    clockInt.attach(&nexaCtrlr);
    clockInt.enable();

    receiver.enable();
}

void loop() {
    Event event;
    Event::queue.await(&event);
    event.dispatch();

    if (event.get_type() == NexaReceiver::REMOTE_CONTROL && NexaReceiver::queue.available() > 0) {
        NEXA::code_t cmd(0);
        NexaReceiver::queue.dequeue(&cmd);
        trace << PSTR("cmd: ") << cmd << endl;
        nexaCtrlr.sendRc(cmd.house, cmd.device, cmd.onoff);
    }
}

void initNexa() {
    //configure();
    nexaCtrlr.readFromEeprom();
}

void configure() {
    // Stue ovn 1: 32211232.0
    nexaCtrlr.add(32211232, 0, 1, 14, 0, 127); // man - søn
    nexaCtrlr.add(32211232, 0, 0, 22, 0, 127); // man - søn

    // Stue ovn 2: 32211232.1
    nexaCtrlr.add(32211232, 1, 1, 14, 0);
    nexaCtrlr.add(32211232, 1, 0, 22, 0);

    // Kjøkken ovn 32211244.0
    nexaCtrlr.add(32211244, 0, 1, 6, 0, 31); // man - fre
    nexaCtrlr.add(32211244, 0, 0, 8, 0, 31); // man - fre
    nexaCtrlr.add(32211244, 0, 1, 14, 0, 31); // man - fre
    nexaCtrlr.add(32211244, 0, 1, 7, 0, 96); // lør - søn
    nexaCtrlr.add(32211244, 0, 0, 22, 0);

    // Loftstua: 32211240.0
    nexaCtrlr.add(32211240, 0, 1, 6, 0);
    nexaCtrlr.add(32211240, 0, 0, 8, 0);
    nexaCtrlr.add(32211240, 0, 1, 14, 0);
    nexaCtrlr.add(32211240, 0, 0, 22, 0);

    // Soverom 1: 32211236.0
    nexaCtrlr.add(32211236, 0, 1, 5, 30);
    nexaCtrlr.add(32211236, 0, 0, 7, 0);
    nexaCtrlr.add(32211236, 0, 1, 15, 0);
    nexaCtrlr.add(32211236, 0, 0, 22, 0);

    // Kontor: 32211234.0
    nexaCtrlr.add(32211234, 0, 1, 15, 0);
    nexaCtrlr.add(32211234, 0, 0, 22, 0);

    // Remote for stua, kjøkken, loftstue
    nexaCtrlr.addRc(12134882, 0, 32211232, 0);
    nexaCtrlr.addRc(12134882, 0, 32211232, 1);
    nexaCtrlr.addRc(12134882, 0, 32211244, 0);
    nexaCtrlr.addRc(12134882, 0, 32211240, 0);

    // Remote for soverom
    nexaCtrlr.addRc(12134882, 1, 32211236, 0);

    // Remote for kontoret
    nexaCtrlr.addRc(12134882, 2, 32211234, 0);

    nexaCtrlr.writeToEeprom();
}
