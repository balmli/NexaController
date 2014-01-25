#ifndef _OREGON_SCI_PARSER_H_
#define _OREGON_SCI_PARSER_H_

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

struct OregonSciData_t {
  char* description;
  uint32_t code;
  uint8_t channel;
  bool batteryLow;
  float temperature;
  uint8_t humidity;
  uint8_t checksum;
  bool valid;
};

inline uint8_t to_binary(uint8_t value) {
  uint8_t high = (value >> 4);
  uint8_t low = (value & 0x0f);
  return ((high << 3) + (high << 1) + low);
}

inline uint8_t nibHigh(uint8_t value) {
  uint8_t high = (value >> 4);
  return high;
}

inline uint8_t nibLow(uint8_t value) {
  uint8_t low = (value & 0x0f);
  return low;
}

bool checkBcd(uint8_t value) {
  return ((value & 0x0f) < 0x0a);
}


// Oregon Scientific THR128,THx138
void parseOregonSci_0A4D(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific THRN132N,THWR288,AW131
void parseOregonSci_EA4C(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific THWR800
void parseOregonSci_CA48(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific RTHN318
void parseOregonSci_0ADC(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific THGN122N,THGN132N,THGR122NX,THGR228N,THGR268
void parseOregonSci_1A2D(const byte* data, OregonSciData_t* osData) {
  osData->description = "Oregon THGN122N,THGN132N,THGR122NX,THGR228N,THGR268";
  osData->code = data[0] * 256 + data[1];
  osData->channel = nibHigh(data[2]);
  osData->batteryLow = (nibLow(data[2]) != 0);
  float temperature = (float)to_binary(data[5]) + (float)nibHigh(data[4]) / 10.0;
  if ((data[6] & 0x0F) != 0) {
    temperature = 0.0 - temperature;
  }
  osData->temperature = temperature;
  osData->humidity = nibLow(data[7]) * 10 + nibHigh(data[6]);

  unsigned int checksum = 0;
  for (int i = 0; i < 8; i++) {
    checksum += (data[i] >> 4) + (data[i] & 0x0f);
  }
  checksum = checksum & 0xff;
  checksum -= 10;
  osData->checksum = checksum;

  osData->valid = (checksum == data[8]);
  osData->valid = osData->valid && checkBcd(nibHigh(data[4]));
  osData->valid = osData->valid && checkBcd(nibLow(data[5]));
  osData->valid = osData->valid && checkBcd(nibHigh(data[5]));
  osData->valid = osData->valid && checkBcd(nibHigh(data[6]));
  osData->valid = osData->valid && checkBcd(nibLow(data[7]));
}

// Oregon Scientific THGR810
void parseOregonSci_FA28(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific RTGR328N
void parseOregonSci_0ACC(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific THGR328
void parseOregonSci_CA2C(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific WTGR800
void parseOregonSci_FAB8(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific THGR918
void parseOregonSci_1A3D(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific BTHR918
void parseOregonSci_5A5D(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific BTHR918N,BTHR968
void parseOregonSci_5A6D(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific RGR126,RGR682,RGR918
void parseOregonSci_2A1D(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific PCR800
void parseOregonSci_2A19(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific WTGR800
void parseOregonSci_1A99(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific WGR800
void parseOregonSci_1A89(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific STR918,WGR918
void parseOregonSci_3A0D(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific UVR138,UV138
void parseOregonSci_EA7C(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific UVN800
void parseOregonSci_DA78(const byte* data, OregonSciData_t* osData) {
}

// Oregon Scientific RTGR328 Date and Time
void parseOregonSci_0AEC(const byte* data, OregonSciData_t* osData) {
}

bool parseOregonSciData(const byte* data, OregonSciData_t* osData) {
  uint32_t code = data[0] * 256 + data[1];
  
  switch (code) {
    case 0x0A4D:
      parseOregonSci_0A4D(data, osData);
      break;
    case 0xEA4C:
      parseOregonSci_EA4C(data, osData);
      break;
    case 0xCA48:
      parseOregonSci_CA48(data, osData);
      break;
    case 0x1A2D:
      parseOregonSci_1A2D(data, osData);
      break;
    case 0xFA28:
      parseOregonSci_FA28(data, osData);
      break;
    case 0xCA2C:
      parseOregonSci_CA2C(data, osData);
      break;
    case 0xFAB8:
      parseOregonSci_FAB8(data, osData);
      break;
    case 0x1A3D:
      parseOregonSci_1A3D(data, osData);
      break;
    case 0x5A5D:
      parseOregonSci_5A5D(data, osData);
      break;
    case 0x5A6D:
      parseOregonSci_5A6D(data, osData);
      break;
    case 0x2A1D:
      parseOregonSci_2A1D(data, osData);
      break;
    case 0x2A19:
      parseOregonSci_2A19(data, osData);
      break;
    case 0x1A99:
      parseOregonSci_1A99(data, osData);
      break;
    case 0x1A89:
      parseOregonSci_1A89(data, osData);
      break;
    case 0x3A0D:
      parseOregonSci_3A0D(data, osData);
      break;
    case 0xEA7C:
      parseOregonSci_EA7C(data, osData);
      break;
    case 0xDA78:
      parseOregonSci_DA78(data, osData);
      break;
    default:
      uint32_t code2 = (data[0] & 0x0F) * 256 + data[1];
      switch (code2) {
      case 0x0ADC:
        parseOregonSci_0ADC(data, osData);
        break;
      case 0x0ACC:
        parseOregonSci_0ACC(data, osData);
        break;
      case 0x0AEC:
        parseOregonSci_0AEC(data, osData);
        break;
      default:
        //Serial.print("OregonSciData: unknown code: 0x");
        //Serial.println(code, HEX);
        return 0;
    }
  }
  
  return 1;
}

void printOregonSciData(OregonSciData_t* osData) {
  Serial.print(osData->code, HEX);
  Serial.print(" "); 
  Serial.print(osData->description);
  Serial.println();
  
  Serial.print("Ch: ");
  Serial.print(osData->channel);
  Serial.print(", Batt.: ");
  Serial.print(osData->batteryLow ? "LOW" : "OK");
  Serial.print(", Temp.: ");
  Serial.print(osData->temperature);
  Serial.print(" C, Hum.: ");
  Serial.print(osData->humidity);
  Serial.print(" %, Checksum: 0x");
  Serial.print(osData->checksum, HEX);
  if (osData->valid) {
    Serial.print(" OK");
  } else {
    Serial.print(" ERROR");
  }
  Serial.println();
}

void parseAndPrintOregonScientific(const byte* data) {
  OregonSciData_t osData;
  if (parseOregonSciData(data, &osData)) {
    printOregonSciData(&osData);
  }
}

#endif
