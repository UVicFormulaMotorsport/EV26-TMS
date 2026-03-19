/*
 * comms_iso_spi.h
 *
 *  Created on: Dec 9, 2025
 *      Author: christopherrudzki
 */

#ifndef INC_COMMS_ISO_SPI_H_
#define INC_COMMS_ISO_SPI_H_

#include "FreeRTOS.h"
#include "semphr.h"

//#include <cmsis_os.h>
#include "stm32f4xx_hal.h"

void commsTask(void* args);
void start_comms_iso_spi(void);

extern uint8_t rec1;

extern SPI_HandleTypeDef hspi1;

typedef struct {
	uint8_t seg_id;
//	uint8_t count;
	uint8_t temps[23];

} temp_packet_s;

extern temp_packet_s temp_packet1;

extern SemaphoreHandle_t tempPacketMutex;

#endif /* INC_COMMS_ISO_SPI_H_ */
