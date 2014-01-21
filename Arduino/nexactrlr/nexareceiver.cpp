
#include "Nexareceiver.h"
#include <Cosa/Driver/NEXA.hh>

void NexaReceiver::on_event(uint8_t type, uint16_t value) { 
  NEXA::code_t cmd(m_code);
  trace << cmd << endl;
}
