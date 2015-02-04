#ifndef __SERIAL_COMMAND_READER_H__
#define __SERIAL_COMMAND_READER_H__

#include "config.h"
#include "commandparser.h"

class SerialCommandReader {
protected:
    CommandParser* _cp;

public:
    SerialCommandReader(CommandParser* cp) {
        _cp = cp;
    }

    void checkSerialCommand();
};

#endif
