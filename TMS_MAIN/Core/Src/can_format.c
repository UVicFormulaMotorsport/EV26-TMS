/*
 * can_format.c
 *
 *  Created on: Dec 18, 2025
 *      Author: christopherrudzki
 */

#include "main.h"
#include "FreeRTOS.h"
#include "can_format.h"
#include <stdio.h>
#include "comms_iso_spi.h"
#include "can.h"
#include "semphr.h"
#include "main.h"

//#include "uvfr_utils.h"


void unload_satt_1(void){

	uv_CAN_msg msg1;
	uv_CAN_msg msg2;
	uv_CAN_msg msg3;

	msg1.flags = 0b00000100;
	msg2.flags = 0b00000100;
	msg3.flags = 0b00000100;

	msg1.dlc = 0x08;
	msg2.dlc = 0x08;
	msg3.dlc = 0x08;

	msg1.msg_id = 0x30;
	msg2.msg_id = 0x31;
	msg3.msg_id = 0x32;

	int p = 0;


	if(xSemaphoreTake(tempPacketMutex, portMAX_DELAY) == pdTRUE){

	//populate can bus data from satellite data
	for(int i = 0; i < 8; i++){
		msg1.data[p] = temp_packet1.temps[i];
		p++;

	}

	p = 0;

	for(int i = 8; i < 16; i++){
		msg2.data[p] = temp_packet1.temps[i];
		p++;

	}

	p = 0;

	for(int i = 16; i < 23; i++){
		msg3.data[p] = temp_packet1.temps[i];
		p++;

	}

	uvSendCanMSG(&msg1);
	uvSendCanMSG(&msg2);
	uvSendCanMSG(&msg3);


	xSemaphoreGive(tempPacketMutex);
	}// Mutex


}

void unload_satt_2(void){

}

void unload_satt_3(void){

}

//loads and sends can messages
void canFormSendTask(void* args){

	while(1){

		unload_satt_1();
//		unload_satt_2();
//		unload_satt_3();

		}
}


void start_make_can_format(void){

	TaskHandle_t myHandle;

	if(xSemaphoreTake(taskMutex, portMAX_DELAY) == pdTRUE){


	task_table[table_count] = myHandle;
	table_count = table_count + 1;


	xTaskCreate(canFormSendTask, "canFormSendTask", 256, NULL, 1 , &myHandle);

	xSemaphoreGive(taskMutex);
	}

}
