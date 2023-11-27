/*
 * TrafficLight_Logic.c
 *
 *  Created on: Nov 27, 2023
 *      Author: gulda
 */

#include "TrafficLogic.h"

void activateTopBottom(){

	turnOffAllLights();
	ControlLight(GREEN_TOP, ON);
	ControlLight(GREEN_BOTTOM, ON);
	ControlLight(RED_PED_LEFT, ON);

	ControlLight(RED_PED_TOP, ON);
	ControlLight(RED_LEFT, ON);
	ControlLight(RED_RIGHT, ON);
}

void activateLeftRight(){
	turnOffAllLights();
	ControlLight(GREEN_LEFT, ON);
	ControlLight(GREEN_RIGHT, ON);

	ControlLight(RED_TOP, ON);
	ControlLight(RED_BOTTOM, ON);

	ControlLight(RED_PED_TOP, ON);
	ControlLight(RED_PED_LEFT, ON);
}

void activatePedLeft(){
	ControlLight(GREEN_PED_LEFT, ON);
	ControlLight(RED_PED_LEFT, OFF);
}

void activatePedTop(){
	ControlLight(GREEN_PED_TOP, ON);
	ControlLight(RED_PED_TOP, OFF);
}

void activateTransition(){
	turnOffAllLights();
	ControlLight(YELLOW_LEFT, ON);
	ControlLight(YELLOW_RIGHT, ON);
	ControlLight(YELLOW_BOTTOM, ON);
	ControlLight(YELLOW_TOP, ON);

	ControlLight(RED_PED_TOP, ON);
	ControlLight(RED_PED_LEFT, ON);

}
