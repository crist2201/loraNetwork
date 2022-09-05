#include "Arduino.h"
#include "SENSORS.h"


#define BufferSize 8


extern SPI_HandleTypeDef hspi1;

uint8_t spiTxBuffer[BufferSize]={10,20,30,40,50,60,70,80};
uint8_t spiRxBuffer[BufferSize];

float dataCO=0.0,dataNO2=0.0;

bool esp32Status = false;



void ESP32_wakeUp(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
}

void ESP32_getData(uint16_t result[2])
{
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	/*PA4     ------> SPI1_SS*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	
	if(HAL_SPI_TransmitReceive(&hspi1,(uint8_t*) spiTxBuffer,spiRxBuffer,BufferSize,1000)!= HAL_OK){
		Error_Handler();
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	
	if (spiRxBuffer[0]==0xEE){
		if (spiRxBuffer[1]==0xDD){
			esp32Status=true;
			result[0] = ((uint16_t)spiRxBuffer[2] << 8) + spiRxBuffer[3]; //CO
			result[1] = ((uint16_t)spiRxBuffer[4] << 8) + spiRxBuffer[5]; //NO2
			//result[0] = dataCO; //ppm
			//result[1] = dataNO2; //ppm	
		}
	}
}



/*void ESP32_getData(float result[2])
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	
	if(HAL_SPI_TransmitReceive(&hspi1,(uint8_t*) spiTxBuffer,spiRxBuffer,BufferSize,1000)!= HAL_OK){
		Error_Handler();
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	
	if (spiRxBuffer[0]==0xEE){
		if (spiRxBuffer[1]==0xDD){
			coStatus=true;
			no2Status=true;
			dataCO = ((uint16_t)spiRxBuffer[2] << 8) + spiRxBuffer[3];
			dataNO2 = ((uint16_t)spiRxBuffer[4] << 8) + spiRxBuffer[5];
			result[0] = dataCO/10.0; //ppm
			result[1] = dataNO2/10.0; //ppm	
		}
	}
	
	POSITION 19
	AppDataBuffer[19]=spiRxBuffer[0]; // CO VALUE
	AppDataBuffer[20]=spiRxBuffer[1];
	AppDataBuffer[21]=spiRxBuffer[2]; // NO2 VALUE
	AppDataBuffer[22]=spiRxBuffer[3];

}*/

