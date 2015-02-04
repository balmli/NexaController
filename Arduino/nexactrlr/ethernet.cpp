#include "ethernet.h"
#include "Cosa/Trace.hh"

bool EthernetClient::beginClient() {
  if (_connected) {
    return _connected;
  }
  
  if (USE_DHCP) {
    // Initiate the Ethernet Controller using DHCP
    _connected = this->begin_P(PSTR(ETHERNET_CLIENT));
#ifndef NDEBUG
     trace << PSTR("Ethernet connected: dhcp: ") << _connected << endl;
#endif
  }
  else {
    uint8_t ip[4] = { IP };
    uint8_t subnet[4] = { SUBNET };
    _connected = this->begin(ip, subnet);  
#ifndef NDEBUG
      trace << PSTR("Ethernet connected: ") << _connected << endl;
#endif
  }
  
  uint8_t ip[4];
  uint8_t subnet[4];
  
  this->get_addr(ip, subnet);
  
#ifndef NDEBUG
  trace << PSTR("IP: ") << ip[0] << PSTR(".") << ip[1] << PSTR(".") << ip[2] << PSTR(".") << ip[3] << endl;
#endif

  return _connected;
}

void EthernetClient::endClient() {
  _connected = false;
  this->end();
}

clock_t EthernetClient::getTime() {
  if (!beginClient()) return 0L;
  
  uint8_t server[4];

  // Use DNS to get the NTP server network address
  DNS dns;
  this->get_dns_addr(server);
  if (!dns.begin(this->socket(Socket::UDP), server)) return 0L;
  if (dns.gethostbyname_P(PSTR(NTP_SERVER), server) != 0) return 0L;

  // Connect to the NTP server using given socket
  NTP ntp(this->socket(Socket::UDP), server, ZONE);

  // Get current time. Allow a number of retries
  const uint8_t RETRY_MAX = 20;
  clock_t clock;
  for (uint8_t retry = 0; retry < RETRY_MAX; retry++) {
    if ((clock = ntp.time()) != 0L) break;
  }

#ifndef NDEBUG
    time_t now(clock);
    trace << now.day << ' ' << now << endl;
#endif

  return clock;
}


