#include "SO2.h"
#include "SENSORS.h"


extern I2C_HandleTypeDef hi2c3;

uint8_t so2rxBuffer[9];
uint8_t so2dataBuffer[9];
uint16_t dataSO2=0;
uint8_t resSO2=0;
float so2=0.0;
bool so2Status = false;


uint8_t passiveMode[9]={0xFF,0x01,0x78,0x04,0x00,0x00,0x00,0x00,0x83};
uint8_t gasTemp[9]={0xFF,0x01,0x88,0x00,0x00,0x00,0x00,0x00,0x77};
uint8_t gas[9]={0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};

void SO2_setMode(void){
	
	if (HAL_I2C_Master_Transmit(&hi2c3, SO2_ADDRESS, passiveMode, 9, 2000) != HAL_OK){
		Error_Handler();
	}
	HAL_I2C_Master_Receive(&hi2c3, SO2_ADDRESS, so2rxBuffer, 9, 2000);
}


float SO2_value(uint16_t data, uint8_t res){
	float value,resolution;
	if (res == 0){
		resolution = 1.0;
	}
	else if (res == 1){
		resolution = 0.1;
	}
	else {
		resolution = 0.01;
	}
	value = data*res;
	return value;
}

  
uint8_t SO2_getData(){
	
		if (HAL_I2C_Master_Transmit(&hi2c3, SO2_ADDRESS, gasTemp, 9, 2000) != HAL_OK){
		Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c3, SO2_ADDRESS, so2dataBuffer, 9, 2000);
		
	if (so2dataBuffer[4]==0x2B){
		so2Status=true;
		dataSO2 = ((uint16_t)so2dataBuffer[2] << 8) + so2dataBuffer[3];
		so2=SO2_value(dataSO2, so2dataBuffer[5]);
	}
	return (uint8_t)(so2*10);
}

/*float SO2_getData(){
	
	for (uint8_t i=0; i<2; i++){
		if (HAL_I2C_Master_Transmit(&hi2c3, SO2_ADDRESS, gasTemp, 9, 2000) != HAL_OK){
		Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c3, SO2_ADDRESS, so2dataBuffer, 9, 2000);
		dataSO2[i] = ((uint16_t)so2dataBuffer[2] << 8) + so2dataBuffer[3];
		//so2=SO2_value(dataSO2[i],so2dataBuffer[5])+so2;
	}
	if (so2dataBuffer[4]==0x2B){
		so2Status=true;
		so2=so2/50.0; //ppm
	}
	return so2;
	AppDataBuffer[12]=sendSO2>>8;
	AppDataBuffer[13]=sendSO2;
	AppDataBuffer[14]=resSO2;
}*/
    
