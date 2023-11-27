/*
 * TrafficInputs.h
 *
 *  Created on: Nov 27, 2023
 *      Author: gulda
 */

#ifndef INC_TRAFFICINPUTS_H_
#define INC_TRAFFICINPUTS_H_

#include "main.h"
#include "stdbool.h"

typedef struct {
    bool LeftCar;
    bool BottomCar;
    bool RightCar;
    bool TopCar;
    bool LeftPed;
    bool TopPed;
} ButtonStates;

void init_TrafficInputs(void);

void trafficInputs_Update(void);

bool getLeftCar(void);

bool getBottomCar(void);

bool getRightCar(void);

bool getTopCar(void);

bool getLeftPed(void);

bool getTopPed(void);

ButtonStates getInputState(void);

#endif /* INC_TRAFFICINPUTS_H_ */
