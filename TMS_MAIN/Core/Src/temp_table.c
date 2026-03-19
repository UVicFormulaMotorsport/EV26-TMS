/*
 * temp_table.c
 *
 *  Created on: Dec 18, 2025
 *      Author: christopherrudzki
 */

#include "FreeRTOS.h"
#include "comms_iso_spi.h"
#include <stdio.h>
#include "task.h"
#include "main.h"
#include "temp_table.h"
#include "semphr.h"
#include "main.h"


uint8_t last_count_1 = 0;

//int temp_table[138] = {0};

void store_in_table(int sat){

	int table_ind = 0;


	if(xSemaphoreTake(tempPacketMutex, portMAX_DELAY) == pdTRUE){


	if(sat == 1){

	for(int i = 0; i < 23; i++){
		temp_table[table_ind] = temp_packet1.temps[i];
		table_ind = table_ind + 1;
		}
	}//if

//	}else if(sat == 2){
//		table_ind = 22;
//
//		for(int i = 0; i < 23; i++){
//			temp_table[table_ind] = temp_packet2.temps[i];
//			table_ind = table_ind + 1;
//		}

//	}else if(sat == 3){
//		table_ind = 45;
//
//		for(int i = 0; i < 23; i++){
//			temp_table[table_ind] = temp_packet3.temps[i];
//			table_ind = table_ind + 1;
//		}



	xSemaphoreGive(tempPacketMutex);
	}// Mutex

	}

void tempStoreTask(void* arg){


	while(1){
		vTaskDelay(pdMS_TO_TICKS(250));

		//store values for each satellite
		store_in_table(1);

//		store_in_table(2);
//		store_in_table(3);
//		store_in_table(4);
//		store_in_table(5);
//		store_in_table(6);


	}
}

void start_temp_store(void){


	TaskHandle_t myHandle;

	if(xSemaphoreTake(taskMutex, portMAX_DELAY) == pdTRUE){


	task_table[table_count] = myHandle;
	table_count = table_count + 1;


	xTaskCreate(tempStoreTask, "tempstoreTask", 512, NULL, 1 , &myHandle);


	xSemaphoreGive(taskMutex);
	}

}

