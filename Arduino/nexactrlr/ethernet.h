#ifndef __ETHERNET_CLIENT_HH__
#define __ETHERNET_CLIENT_HH__

#include "config.h"

#include "Cosa/INET/DHCP.hh"
#include "Cosa/INET/DNS.hh"
#include "Cosa/INET/NTP.hh"
#include "Cosa/Socket/Driver/W5100.hh"

#define USE_DHCP true
#define IP 192,168,1,178
#define SUBNET 255,255,255,0

static const uint8_t mac[6] __PROGMEM = {0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed};

#define ETHERNET_CLIENT "ArdNexa"

// Time-zone; GMT+1, Oslo
#define ZONE 1

// NTP server
#define NTP_SERVER "no.pool.ntp.org"

class EthernetClient :
public W5100 {
private:
    bool _connected;
protected:

public:

    EthernetClient() :
    W5100(mac, Board::D10),
    _connected(false) {
    }

    bool beginClient();
    void endClient();
    clock_t getTime();
};

#endif


