#ifndef __CLOCKINT_HH__
#define __CLOCKINT_HH__

#include "config.h"

#include "Cosa/ExternalInterrupt.hh"
#include "Cosa/Linkage.hh"

class ClockInterrupt :
public ExternalInterrupt, public Head {
protected:

    virtual void on_interrupt(uint16_t arg = 0) {
        Event::push(Event::USER_TYPE, this);
    }

public:

    ClockInterrupt(Board::ExternalInterruptPin pin = Board::EXT0) :
    ExternalInterrupt(pin, ExternalInterrupt::ON_FALLING_MODE),
    Head() {
    }

};

#endif



