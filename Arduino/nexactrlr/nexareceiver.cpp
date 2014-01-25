
#include "Nexareceiver.h"
#include <Cosa/Event.hh>
#include <Cosa/Driver/NEXA.hh>

void NexaReceiver::on_event(uint8_t type, uint16_t value) {
    if (type != Event::RECEIVE_COMPLETED_TYPE) {
        return;
    }

    NEXA::code_t cmd(m_code);
    //trace << cmd << endl;

    Event::push(NexaReceiver::REMOTE_CONTROL, this, m_code);

}
