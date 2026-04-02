#include <cstdlib>
#include <cmath>
#include <ctime>
#include "DHT.h"
#include "config.h"

#define MAX_T 50
#define MIN_T 0
#define TARGET_T 25
#define MAX_H 80
#define MIN_H 20
#define TARGET_H 50

DHT::DHT(uint8_t pin, uint8_t type, uint8_t count)
        : temperature(20.0),    humidity(60.0),
          temperature_vel(0.0), humidity_vel(0.0)
{
  srand(time(0));
  return;
}
void DHT::begin(uint8_t usec)
{
  return;
}
float DHT::readTemperature(bool S, bool force)
{
  return random_fluctuation(MIN_T, MAX_T, TARGET_T,
                            temperature, temperature_vel);
}
float DHT::readHumidity(bool force)
{
  return random_fluctuation(MIN_H, MAX_H, TARGET_H,
                            humidity, humidity_vel);
}

// To make semi-random values
float DHT::random_fluctuation(int min, int max, int target,
                              float& value, float& vel)
{
  const float delta = static_cast<float>(max - min) * MEASURING_RATE_MS / 100'000;
  // NOTE: Assumes that target is between max and min
  const float distance = 0.5 * (target - value) / (max - min);
  float random_value = static_cast<float>(rand()) / RAND_MAX;
  random_value = (random_value - 0.5) * 2;

  vel += random_value + distance;
  vel *= 0.8;  // <friction to avoid exponential pendulum
  value += vel * delta;

  value = fminf(max, value);
  value = fmaxf(min, value);
  return static_cast<int>(value); // for rounding
}
