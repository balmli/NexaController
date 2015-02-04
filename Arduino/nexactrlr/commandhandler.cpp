#include "commandhandler.h"

#include "Cosa/Trace.hh"

void CommandHandler::showInformation() {
    trace << PSTR("ArdNexa v 0.0.1") << endl;
    _rtc->viewClock();
}

void CommandHandler::reboot() {
}

void CommandHandler::reset() {
}
