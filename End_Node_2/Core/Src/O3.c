#include "O3.h"
#include "SENSORS.h"


extern I2C_HandleTypeDef hi2c3;

uint8_t O3_MODE_REGISTER = 0x03;
uint8_t O3_PASSIVE_MODE = 0x01;
uint8_t O3_SET_PASSIVE_REGISTER = 0x04;
uint8_t O3_PASSIVE_READ_DATA = 0x01;
uint8_t O3_PASSIVE_DATA_REGISTER = 0x07;
uint8_t count=0;

uint8_t o3Buffer[2]={0};
float dataO3[50]={0};
float o3=0.0;
bool o3Status = false;


void O3_setMode(){
	uint8_t buffer[2]={};
	buffer[0]=O3_MODE_REGISTER;
	buffer[1]=O3_PASSIVE_MODE;
	if (HAL_I2C_Master_Transmit(&hi2c3, O3_ADDRESS, buffer, 2, 1000) != HAL_OK){
		Error_Handler();
	}
}

uint16_t O3_getData(){
	uint8_t buffer[2]={};
	buffer[0]=O3_SET_PASSIVE_REGISTER;
	buffer[1]=O3_PASSIVE_READ_DATA;
	if (HAL_I2C_Master_Transmit(&hi2c3, O3_ADDRESS, buffer, 2, 1000) != HAL_OK){
		Error_Handler();
	}
	o3Status=true;
	for (uint8_t i=0; i<50; i++){
		if (HAL_I2C_Master_Transmit(&hi2c3, O3_ADDRESS, &O3_PASSIVE_DATA_REGISTER, 1, 1000) != HAL_OK){
			Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c3, O3_ADDRESS, o3Buffer, 2, 500);	
		dataO3[i] = ((uint16_t)o3Buffer[0] << 8) + o3Buffer[1];
		o3=dataO3[i]+o3;
	}
	o3 =o3/50.0;
	
	return (uint16_t)(o3*10);//ppb
	//AppDataBuffer[10]=sendO3>>8;
	//AppDataBuffer[11]=sendO3;
}

/*float O3_getData(){
	uint8_t buffer[2]={};
	buffer[0]=O3_SET_PASSIVE_REGISTER;
	buffer[1]=O3_PASSIVE_READ_DATA;
	if (HAL_I2C_Master_Transmit(&hi2c3, O3_ADDRESS, buffer, 2, 1000) != HAL_OK){
		Error_Handler();
	}
	o3Status=true;
	for (uint8_t i=0; i<50; i++){
		if (HAL_I2C_Master_Transmit(&hi2c3, O3_ADDRESS, &O3_PASSIVE_DATA_REGISTER, 1, 1000) != HAL_OK){
			Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c3, O3_ADDRESS, o3Buffer, 2, 500);	
		dataO3[i] = ((uint16_t)o3Buffer[0] << 8) + o3Buffer[1];
		o3=dataO3[i]+o3;
	}
	o3 =o3/50.0;
	
	return o3;//ppb
	//AppDataBuffer[10]=sendO3>>8;
	//AppDataBuffer[11]=sendO3;
}*/





