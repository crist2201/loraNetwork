#include "SHT20.h"
#include "SENSORS.h"



extern I2C_HandleTypeDef hi2c3;

uint8_t SHT20_HUM = 0xE5;
uint8_t SHT20_TEMP = 0xE3;
uint8_t SHT20_RREG = 0xE7;
uint8_t SHT20_WREG = 0xE5;
float dataT=0.00;
float temp=0.00;
float dataH=0.00;
float hum=0.00;
bool tempStatus = false;
bool humStatus = false;


uint8_t humBuffer[3]={0};
uint8_t tempBuffer[3]={0};


float SHT20_temperature(uint8_t value[3]){
	return -46.85 + 175.72 * ( (value[0]<<8 | value[1]) / 65536.0);
}

float SHT20_humidity(uint8_t value[3]){
	
	return -6 + 125.0 * ( (value[0]<<8 | value[1]) / 65536.0);
}

uint16_t SHT20_getTemperature() {
	
	if (HAL_I2C_Master_Transmit(&hi2c3, SHT20_ADDRESS, &SHT20_TEMP, 1, 1000) != HAL_OK){
		Error_Handler();
	}
	HAL_I2C_Master_Receive(&hi2c3, SHT20_ADDRESS, tempBuffer, 3, 1000);
	temp=SHT20_temperature(tempBuffer);
	tempStatus=true;
	return (uint16_t)(temp*10);
}

uint16_t SHT20_getHumidity() {
	if (HAL_I2C_Master_Transmit(&hi2c3, SHT20_ADDRESS, &SHT20_HUM, 1, 1000) != HAL_OK){
		Error_Handler();
	}
	HAL_I2C_Master_Receive(&hi2c3, SHT20_ADDRESS, humBuffer, 3, 1000);
	hum=SHT20_humidity(humBuffer);
	humStatus=true;
	return (uint16_t)(hum*10);
}

/*float SHT20_getTemperature() {
	
	for (uint8_t i=0; i<50; i++){
		if (HAL_I2C_Master_Transmit(&hi2c3, SHT20_ADDRESS, &SHT20_TEMP, 1, 1000) != HAL_OK){
		Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c3, SHT20_ADDRESS, tempBuffer, 3, 1000);
		dataT[i]=SHT20_temperature(tempBuffer);
		temp=dataT[i]+temp;
	}
	tempStatus=true;
	temp=temp/50.0; //Centi
	return temp;
	//AppDataBuffer[15]=sendTemp>>8;
	//AppDataBuffer[16]=sendTemp;
}

float SHT20_getHumidity() {
	

	for (uint8_t i=0; i<50; i++){
		if (HAL_I2C_Master_Transmit(&hi2c3, SHT20_ADDRESS, &SHT20_HUM, 1, 1000) != HAL_OK){
		Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c3, SHT20_ADDRESS, humBuffer, 3, 1000);
		dataH[i]=SHT20_humidity(humBuffer);
		hum=dataH[i]+hum;
	}
	humStatus=true;
	hum=hum/50.0; //%hum relativa
	return hum;
	//AppDataBuffer[17]=sendHum>>8;
	//AppDataBuffer[18]=sendHum;
	
}*/