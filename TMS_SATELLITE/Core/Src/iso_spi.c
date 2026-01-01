/*
 * iso_spi.c
 *
 *  Created on: Dec 9, 2025
 *      Author: christopherrudzki
 */
#include "FreeRTOS.h"
#include "iso_spi.h"
#include <stdio.h>
#include "task.h"


int p = 0;
int k = 20;


void commTaskSend(void* args){

		temp_packet_s temp_packet1 = {0};

		temp_packet_s dummy_packet = {0};


		while(1){

			vTaskDelay(pdMS_TO_TICKS(150));

			p = p + 1;

			if(p > 23){
				p = 0;
			}

			temp_packet1.seg_id = p;

			// dummy heat values
			for(int i = 0; i < 23; i++){
				temp_packet1.temps[i] = k;
				k = k + 1;
			}

			HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&temp_packet1, (uint8_t *)&dummy_packet, sizeof(temp_packet1), 100);
		}
}

void start_comms_iso_spi_send(void){

	xTaskCreate(commTaskSend, "commTaskSend", 512, NULL, 1 , NULL);

}
