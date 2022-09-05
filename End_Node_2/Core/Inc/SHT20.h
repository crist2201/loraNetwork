#include "stm32wlxx_hal.h"

#define SHT20_ADDRESS 0x40<<1

uint16_t SHT20_getTemperature();
uint16_t SHT20_getHumidity();
float SHT20_temperature(uint8_t value[3]);
float SHT20_humidity(uint8_t value[3]);

