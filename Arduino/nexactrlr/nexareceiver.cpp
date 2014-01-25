
#include "Nexareceiver.h"
#include <Cosa/Event.hh>
#include <Cosa/Driver/NEXA.hh>

Queue<NEXA::code_t, NexaReceiver::QUEUE_MAX> NexaReceiver::queue;

void NexaReceiver::on_event(uint8_t type, uint16_t value) {
    if (type != Event::RECEIVE_COMPLETED_TYPE) {
        return;
    }

    push(m_code);
    Event::push(NexaReceiver::REMOTE_CONTROL, this);

}
