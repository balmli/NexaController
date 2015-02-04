#ifndef __COMMAND_PARSER_H__
#define __COMMAND_PARSER_H__

#include "config.h"
#include "commandhandler.h"
#include "nexaconfigstore.h"

#include "Cosa/Event.hh"

class CommandParser : public Event::Handler {
private:
    CommandHandler* _ch;
    NexaConfigStore* _ncs;
    
protected:
    virtual void on_event(uint8_t type, uint16_t value);
    bool starts_with(const char* string, const char* prefix);
    uint16_t get_num(const char* string, uint8_t start);
  
public:

    enum {
        COMMAND_RECEIVED = 100
    } __attribute__((packed));

    CommandParser(CommandHandler* ch, NexaConfigStore* ncs) {
        _ch = ch;
        _ncs = ncs;
    }

    void parseCommand(char* cmd);
    char* getRegister(uint16_t regnum);
    void setRegister(uint16_t regnum, char* value);
};

#endif

