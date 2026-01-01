/*
 * can_format.h
 *
 *  Created on: Dec 18, 2025
 *      Author: christopherrudzki
 */

#ifndef INC_CAN_FORMAT_H_
#define INC_CAN_FORMAT_H_

void canFormSendTask(void* args);
void start_form_send(void);
void store_and_format(int index);

#endif /* INC_CAN_FORMAT_H_ */
