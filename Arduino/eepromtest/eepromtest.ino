
#include <Cosa/RTC.hh>
#include <Cosa/Watchdog.hh>
#include <Cosa/Trace.hh>
#include <Cosa/IOBuffer.hh>
#include <Cosa/IOStream/Driver/UART.hh>
#include <Cosa/EEPROM.hh>

EEPROM eeprom;

class TestData {
public:
  union nexadevice_t {

    struct {
      uint32_t house;
      uint8_t device;
      uint8_t onoff;
      uint8_t hour;
      uint8_t minute;
      uint8_t daymask;
    };

    nexadevice_t() {
    };

    nexadevice_t(int32_t h, uint8_t d, uint8_t f, uint8_t ho, uint8_t mi, uint8_t dm) {
      house = h;
      device = d;
      onoff = f;
      hour = ho;
      minute = mi;
      daymask = dm;
    }

    friend IOStream& operator<<(IOStream& outs, nexadevice_t code);
  };
};


void setup() {
  uart.begin(9600);
  trace.begin(&uart, PSTR("NexaController started"));
  //trace_log_mask = LOG_UPTO(LOG_INFO);
  trace_log_mask = LOG_UPTO(LOG_DEBUG);

  Watchdog::begin(16, SLEEP_MODE_IDLE, Watchdog::push_timeout_events);
  RTC::begin();


  trace << PSTR("Is ready: ") << eeprom.is_ready() << endl;

  TestData::nexadevice_t nd(123, 1, 1, 7, 0, 127);
  
  int ret = eeprom.write(0, &nd, sizeof(nd));
  eeprom.write_await();
  trace << PSTR("write: ") << ret << endl;
  
  TestData::nexadevice_t nd2;
  
  int ret2 = eeprom.read(&nd2, 0, sizeof(nd2));
  trace << PSTR("read: ") << ret << endl;
  trace << PSTR("read result: ") << nd2.house << PSTR(" ") << nd2.device << PSTR(" ") << nd2.onoff << PSTR(" ") << nd2.hour << PSTR(" ") << nd2.minute << PSTR(" ") << nd2.daymask << endl; 
}




