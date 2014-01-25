#ifndef __NEXA_RECEIVER_H__
#define __NEXA_RECEIVER_H__

#include "Cosa/Driver/NEXA.hh"
#include "Cosa/Trace.hh"

class NexaReceiver :
public NEXA::Receiver {
public:
    static const uint8_t QUEUE_MAX = 16;

    enum {
        REMOTE_CONTROL = 65
    } __attribute__((packed));

    NexaReceiver(Board::ExternalInterruptPin pin) :
    NEXA::Receiver(pin) {
    }

    virtual void on_event(uint8_t type, uint16_t value);

    static bool push(uint32_t mCode) {
        NEXA::code_t cmd(mCode);
        return (queue.enqueue(&cmd));
    }

    static Queue<NEXA::code_t, QUEUE_MAX> queue;
};

#endif

