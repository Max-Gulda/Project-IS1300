/*
 * TrafficInputs.c
 *
 *  Created on: Nov 27, 2023
 *      Author: gulda
 */
#include "TrafficInputs.h"

static ButtonStates buttonState = {0};


void init_TrafficInputs(){
	trafficInputs_Update();
}

void trafficInputs_Update(){
	buttonState.LeftCar 	= !HAL_GPIO_ReadPin(TL1_CAR_GPIO_Port, TL1_CAR_Pin);
	buttonState.BottomCar 	= !HAL_GPIO_ReadPin(TL2_CAR_GPIO_Port, TL2_CAR_Pin);
	buttonState.RightCar 	= !HAL_GPIO_ReadPin(TL3_CAR_GPIO_Port, TL3_CAR_Pin);
	buttonState.TopCar 		= !HAL_GPIO_ReadPin(TL4_CAR_GPIO_Port, TL4_CAR_Pin);
	buttonState.LeftPed 	= !HAL_GPIO_ReadPin(PL1_Switch_GPIO_Port, PL1_Switch_Pin);
	buttonState.TopPed 		= !HAL_GPIO_ReadPin(PL2_Switch_GPIO_Port, PL2_Switch_Pin);
}

ButtonStates getInputState(){
	return buttonState;
}

bool getLeftCar(){
	return buttonState.LeftCar;
}

bool getBottomCar(){
	return buttonState.BottomCar;
}

bool getRightCar(){
	return buttonState.RightCar;
}

bool getTopCar(){
	return buttonState.TopCar;
}

bool getLeftPed(){
	return buttonState.LeftPed;
}

bool getTopPed(){
	return buttonState.TopPed;
}
