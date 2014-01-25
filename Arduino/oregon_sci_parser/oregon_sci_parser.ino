#include "oregonsci_v2.h"
#include "oregonsciparser.h"

#define PORT 2
#define LED_PIN 7

OregonDecoderV2 orscV2;

volatile word pulse;

#if defined(__AVR_ATmega1280__)
void ext_int_1(void) {
#else
ISR(ANALOG_COMP_vect) {
#endif
    static word last;
    // determine the pulse length in microseconds, for either polarity
    pulse = micros() - last;
    last += pulse;
}

void setup () {
    Serial.begin(115200);
    Serial.println("\n[ookDecoder]");
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
#if !defined(__AVR_ATmega1280__)
    pinMode(13 + PORT, INPUT);  // use the AIO pin
    digitalWrite(13 + PORT, 1); // enable pull-up

    // use analog comparator to switch at 1.1V bandgap transition
    ACSR = _BV(ACBG) | _BV(ACI) | _BV(ACIE);

    // set ADC mux to the proper port
    ADCSRA &= ~ bit(ADEN);
    ADCSRB |= bit(ACME);
    ADMUX = PORT - 1;
#else
   attachInterrupt(1, ext_int_1, CHANGE);

   DDRE  &= ~_BV(PE5);
   PORTE &= ~_BV(PE5);
#endif
}

unsigned long lastPulse;

void loop () {
  cli();
  word p = pulse;
  pulse = 0;
  sei();
  if (p == 0) {
    return;
  }

  if (orscV2.nextPulse(p)) {
    unsigned long now = micros();
    unsigned long timing = now - lastPulse;
    lastPulse = now;
    byte pos;
    const byte* data = orscV2.getData(pos);
    digitalWrite(LED_PIN, HIGH);
    Serial.println();
    Serial.println(timing);
    reportSerial("OSV2", orscV2);  
    parseAndPrintOregonScientific(data);
    delay(200);
    digitalWrite(LED_PIN, LOW);
  }
}

void reportSerial (const char* s, class DecodeOOK& decoder) {
  byte pos;
  const byte* data = decoder.getData(pos);
  Serial.print(s);
  Serial.print(' ');
  for (byte i = 0; i < pos; ++i) {
    Serial.print(data[i] >> 4, HEX);
    Serial.print(data[i] & 0x0F, HEX);
  }
  Serial.println();
  decoder.resetDecoder();
}


