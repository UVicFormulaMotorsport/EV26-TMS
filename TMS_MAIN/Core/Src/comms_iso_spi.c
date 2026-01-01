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


void commsTask(void* args){

	temp_packet_s temp_packet1 = {0};
	temp_packet_s dummy_packet = {0};

	// make sure satellite has sent first
	vTaskDelay(pdMS_TO_TICKS(100));

	while(1){

		//transmit for each satellite / ISO SPI
		vTaskDelay(pdMS_TO_TICKS(150));
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&dummy_packet, (uint8_t *)&temp_packet1, sizeof(temp_packet1), 100);

	}
}

void start_comms_iso_spi(void){

	xTaskCreate(commsTask, "commsTask", 512, NULL, 1 , NULL);

}
