#ifndef __NEXA_RECEIVER_H__
#define __NEXA_RECEIVER_H__

#include <Cosa/Driver/NEXA.hh>
#include <Cosa/Trace.hh>

class NexaReceiver : public NEXA::Receiver {
public:
  NexaReceiver(Board::ExternalInterruptPin pin) : NEXA::Receiver(pin) {}
  virtual void on_event(uint8_t type, uint16_t value);
};

#endif
