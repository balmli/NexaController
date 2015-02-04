#include "commandparser.h"

#include "Cosa/Trace.hh"

void CommandParser::on_event(uint8_t type, uint16_t value) {
    char* cmd = (char *)value;
    parseCommand(cmd);
}

void CommandParser::parseCommand(char* cmd) {
    if (cmd == NULL) {
        return;
    }
    else if (starts_with(cmd, "ATS")) {
      uint16_t num = get_num(cmd, 3);
      //trace << PSTR("ATS ") << num << endl;
      char* str1 = strstr(cmd, "?");
      if (str1) {
        //trace << PSTR(" query ") << num << endl;
        getRegister(num);
      }
      char* str2 = strstr(cmd, "=");
      if (str2) {
        *str2++;
        //trace << PSTR(" set ") << num << PSTR(" = ") << str2 << endl;
        setRegister(num, str2);
      }
    }
    else if (strcmp(cmd, "ATD?") == 0) {
        trace << _ncs->getNumDevices() << endl;
    }
    else if (starts_with(cmd, "ATD?")) {
        uint16_t num = get_num(cmd, 3);
        trace << PSTR("ATD? ") << num << endl;
    }
    else if (strcmp(cmd, "ATDC") == 0) {
        _ncs->clearDevices();
        trace << PSTR("OK") << endl;
    }
    else if (strcmp(cmd, "ATR?") == 0) {
        trace << _ncs->getNumRemotes() << endl;
    }
    else if (starts_with(cmd, "ATR?")) {
        uint16_t num = get_num(cmd, 3);
        trace << PSTR("ATR? ") << num << endl;
    }
    else if (strcmp(cmd, "ATRC") == 0) {
        _ncs->clearRemotes();
        trace << PSTR("OK") << endl;
    }
    else if (strcmp(cmd, "ATI") == 0) {
        _ch->showInformation();
    }
    else if (strcmp(cmd, "ATX") == 0) {
        _ch->reboot();
    }
    else if (strcmp(cmd, "ATZ") == 0) {
        _ch->reset();
    } 
    else {
      trace << PSTR("UNKNOWN") << endl;
    }
}

bool CommandParser::starts_with(const char* string, const char* prefix) {
    while(*prefix) {
        if(*prefix++ != *string++) return 0;
    }
    return 1;
}

uint16_t CommandParser::get_num(const char* string, uint8_t start) {
    uint16_t ret = 0;
    while(*string) {
      if (start > 0) {
        start--;
      } 
      else if (*string >= '0' && *string <= '9') {
          ret *= 10;
          ret += ((*string) - '0');
      }
      else {
        return ret;
      }
      *string++;
    }
    return ret;
}

char* CommandParser::getRegister(uint16_t regnum) {
    trace << PSTR("getreg ") << regnum << endl;
    return "";
}

void CommandParser::setRegister(uint16_t regnum, char* value) {
    trace << PSTR("setreg ") << regnum << PSTR(" = ") << value << endl;
}

