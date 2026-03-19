/*
 * comms_iso_spi.c
 *
 *  Created on: Dec 9, 2025
 *      Author: christopherrudzki
 */

#include "FreeRTOS.h"
#include "comms_iso_spi.h"
#include <stdio.h>
#include "task.h"
#include "semphr.h"
#include "main.h"

	temp_packet_s temp_packet1 = {0}; // sattlite edit
	temp_packet_s dummy_packet = {0};


void commsTask(void* args){


	// make sure satellite has sent first
	vTaskDelay(pdMS_TO_TICKS(100));

	while(1){
		//transmit for each satellite / ISO SPI


		vTaskDelay(pdMS_TO_TICKS(150));

		if(xSemaphoreTake(tempPacketMutex, portMAX_DELAY) == pdTRUE){
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&dummy_packet, (uint8_t *)&temp_packet1, sizeof(temp_packet1), 100);
		xSemaphoreGive(tempPacketMutex);
		}

	}
}



void start_comms_iso_spi(void){
	TaskHandle_t myHandle;

	if(xSemaphoreTake(taskMutex, portMAX_DELAY) == pdTRUE){


	task_table[table_count] = myHandle;
	table_count = table_count + 1;

	xTaskCreate(commsTask, "commsTask", 512, NULL, 1 , &myHandle);

	xSemaphoreGive(taskMutex);
	}


}
