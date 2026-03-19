/*
 * can_format.h
 *
 *  Created on: Dec 18, 2025
 *      Author: christopherrudzki
 */

#ifndef INC_CAN_FORMAT_H_
#define INC_CAN_FORMAT_H_

void canFormSendTask(void* args);
void start_make_can_format(void);
void store_and_format(int index);

/** @brief Representative of a CAN message
 *
 */


#endif /* INC_CAN_FORMAT_H_ */
