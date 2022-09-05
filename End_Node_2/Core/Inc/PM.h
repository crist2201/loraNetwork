#include "stm32wlxx_hal.h"


#define RXBUFFERSIZE 32

void PM_getData(void);
void PM_sendData(uint8_t data[6]);
void PM1_value(uint8_t buffer[],uint8_t result[2]);
void PM25_value(uint8_t buffer[],uint8_t result[2]);
void PM10_value(uint8_t buffer[],uint8_t result[2]);