/*
 * iso_spi.h
 *
 *  Created on: Dec 16, 2025
 *      Author: christopherrudzki
 */

#ifndef INC_ISO_SPI_H_
#define INC_ISO_SPI_H_

#include "stm32l1xx_hal.h"
#include <stdint.h>

void commTaskSend(void* args);
void start_comms_iso_spi_send(void);

extern SPI_HandleTypeDef hspi1;

typedef struct {
	uint8_t seg_id; //id for satellite
//	uint8_t count;
	uint8_t temps[23];
} temp_packet_s;

extern temp_packet_s temp_packet;

#endif /* INC_ISO_SPI_H_ */
