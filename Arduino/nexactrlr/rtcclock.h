#ifndef __RTC_CLOCK_HH__
#define __RTC_CLOCK_HH__

#include "config.h"

#include "Cosa/TWI/Driver/DS3231.hh"
#include "Cosa/Linkage.hh"

class RtcClock :
public DS3231, public Link {
protected:
    void setTime();
    void setAlarms();
    void initConfig();
    void clearInterruptFlags();
    virtual void on_event(uint8_t type, uint16_t value);

public:

    RtcClock() :
    DS3231(),
    Link() {
    }

    void begin();
    void viewClock();
    virtual void on_clock_interrupt();
    void setRtc(clock_t clock);
};

#endif

