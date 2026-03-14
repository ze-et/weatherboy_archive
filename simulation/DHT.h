#ifndef _DHT_H
#define _DHT_H
#include <cstdint>

class DHT {
public:
  DHT(uint8_t pin, uint8_t type, uint8_t count = 6);
  void begin(uint8_t usec = 55);
  float readTemperature(bool S = false, bool force = false);
  float readHumidity(bool force = false);
};
#endif /// _DHT_H
