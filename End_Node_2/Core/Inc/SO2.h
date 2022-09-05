#include "stm32wlxx_hal.h"

#define SO2_ADDRESS 0x74<<1

void SO2_setMode(void);
uint8_t SO2_getData();
float SO2_value(uint16_t data, uint8_t res);


