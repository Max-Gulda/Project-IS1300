/*
 * hc595.h
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */

#ifndef INC_HC595_H_
#define INC_HC595_H_
#include "main.h"

void hc595_init(SPI_HandleTypeDef *_hspi3);

void hc595_update(uint8_t* dataToSend);

//void hc595_reset(void);


#endif /* INC_HC595_H_ */
