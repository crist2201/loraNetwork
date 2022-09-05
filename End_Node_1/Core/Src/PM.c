#include "PM.h"
#include "SENSORS.h"

extern UART_HandleTypeDef huart1;


uint8_t pmBuffer[32]={0};
bool pmStatus = false;


void PM_getData(void){
	/*	5V SI O SI */
	do{
		HAL_UART_Receive(&huart1, (uint8_t *)pmBuffer, RXBUFFERSIZE,1000);
	} while(pmBuffer[0]!=0x42);

}	
	

void PM1_value(uint8_t buffer[],uint8_t result[2]){
	result[0]=buffer[4];
	result[1]=buffer[5];
	//value = (buffer[4]<<8)+buffer[5];
}

void PM25_value(uint8_t buffer[],uint8_t result[2]){
	uint8_t value=0;
	value = (buffer[6]<<8)+buffer[7];
}

void PM10_value(uint8_t buffer[],uint8_t result[2]){
	uint8_t value=0;
	value = (buffer[8]<<8)+buffer[9];
}


void PM_sendData(uint8_t data[6]){
	
	if (pmBuffer[0]==0x42){
		if (pmBuffer[1]==0x4D){
			pmStatus=true;
			for(uint8_t i=0;i<6;i++){
				data[i]=pmBuffer[i+4];
			}
		}
		else{
			
		}
    //memset(pmBuffer, 0, 32);
	}
}

  
  