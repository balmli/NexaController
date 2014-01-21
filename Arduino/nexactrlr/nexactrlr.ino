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

RtcClock ds3231;
ClockInterrupt clockInt(Board::EXT1);
NexaController nexaCtrlr(Board::D12, &ds3231);
NexaReceiver receiver(Board::EXT0);

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

void initNexa() {
  // Stue ovn 1: 32211232.0
  nexaCtrlr.add(32211232, 0, 1, 14, 0);
  nexaCtrlr.add(32211232, 0, 0, 22, 0);
  
  // Stue ovn 2: 32211232.1
  nexaCtrlr.add(32211232, 1, 1, 14, 0);
  nexaCtrlr.add(32211232, 1, 0, 22, 0);
  
  // Kjøkken ovn 32211244.0
  nexaCtrlr.add(32211244, 0, 1, 6, 0);
  nexaCtrlr.add(32211244, 0, 0, 8, 0);
  nexaCtrlr.add(32211244, 0, 1, 14, 0);
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
}
