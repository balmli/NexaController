#ifndef __NEXA_LOGGER_H__
#define __NEXA_LOGGER_H__

#include "config.h"

#include "Cosa/Linkage.hh"

class NexaLogger :
public Link {
protected:
    virtual void on_event(uint8_t type, uint16_t value);

public:

    NexaLogger() :
    Link() {
    }

    void logNexa(char* cmd);
};

#endif

