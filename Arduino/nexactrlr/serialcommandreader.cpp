#include "config.h"
#include "serialcommandreader.h"

#include "Cosa/Event.hh"
#include "Cosa/IOStream/Driver/UART.hh"

static char scr_buffer[32];

void SerialCommandReader::checkSerialCommand() {
    if (uart.available()) {
      if (uart.gets(scr_buffer, sizeof(scr_buffer)) == NULL) return;
      Event::push(CommandParser::COMMAND_RECEIVED, _cp, scr_buffer);
    }
}
