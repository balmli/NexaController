#ifndef __COMMAND_HANDLER_H__
#define __COMMAND_HANDLER_H__

#include "config.h"
#include "rtcclock.h"

#include "Cosa/Event.hh"

class CommandHandler {
private:
    RtcClock* _rtc;
protected:

public:
    CommandHandler(RtcClock* rtc) {
        _rtc = rtc;
    }

    void showInformation();
    void reboot();
    void reset();
};

#endif

