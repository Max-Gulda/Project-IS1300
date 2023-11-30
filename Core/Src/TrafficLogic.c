/*
 * TrafficLight_Logic.c
 *
 *  Created on: Nov 27, 2023
 *      Author: gulda
 */

#include "TrafficLogic.h"

static CrossingVersion crossing;

static TrafficCrossingState state;

static TrafficCrossingState nextState;

static bool lightsActivated;

static bool delaySent;

TrafficCrossingAction action;

static uint32_t previousDelayExpireAt;

/*
typedef struct{
	bool TopPedWaiting;
	bool LeftPedWaiting;
	bool delayUntilNextState;
	uint16_t delayTime;
}TrafficCrossingAction;
*/

void initTrafficLogic(CrossingVersion cross){
	previousDelayExpireAt = UINT16_MAX;
	lightsActivated = false;
	delaySent = false;
	crossing = cross;
	state = H_Active;
	action.StartTimerForNextState = false;
	action.KeepStateFor = 0;

}

void TrafficCrossing(ButtonStates _buttonState){
	switch(crossing){
		case PedOneLane:
			OneLane(_buttonState);
			break;
		case NoPedTwoLane:
			TwoLane(_buttonState);
			break;
		case PedTwoLane:
			TwoLanePed(_buttonState);
			break;
	}
}

TrafficCrossingAction* getTrafficAction(){
	return &action;
}

void ActivateNextState(){
	state = nextState;
	delaySent = false;
	lightsActivated = false;
	previousDelayExpireAt = UINT16_MAX;
}


void OneLane(ButtonStates button){
	switch(state){
		case H_Active:
			nextState = Transition_To_V;
			if(!lightsActivated){
				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);

				ControlLight(GREEN_BOTTOM, ON);
				ControlLight(GREEN_TOP, ON);

				ControlLight(RED_TOP, OFF);
				ControlLight(RED_BOTTOM, OFF);

				ControlLight(RED_PED_TOP, ON);
				lightsActivated = true;
			}

			if(button.TopPed && !delaySent){
				action.KeepStateFor = PEDESTRIAN_DELAY;
				action.StartTimerForNextState = true;
				action.TopPedWaiting = true;
				delaySent = true;
			}

			break;
		case V_P_Active:
			nextState = Transition_To_H;
			if(!lightsActivated){
				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);

				ControlLight(GREEN_PED_TOP, ON);
				ControlLight(RED_PED_TOP, OFF);

				ControlLight(RED_TOP, ON);
				ControlLight(RED_BOTTOM, ON);
				lightsActivated = true;
			}
			if(!delaySent){
				action.KeepStateFor = WALKING_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}
			break;
		case Transition_To_H:
			nextState = H_Active;
			if(!lightsActivated){
				ControlLight(GREEN_PED_TOP, OFF);
				ControlLight(RED_PED_TOP, ON);

				ControlLight(RED_TOP, OFF);
				ControlLight(RED_BOTTOM, OFF);

				ControlLight(YELLOW_BOTTOM, ON);
				ControlLight(YELLOW_TOP, ON);
				lightsActivated = true;
			}
			if(!delaySent){
				action.KeepStateFor = YELLOW_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}
			break;
		case Transition_To_V:
			nextState = V_P_Active;
			if(!lightsActivated){
				ControlLight(YELLOW_BOTTOM, ON);
				ControlLight(YELLOW_TOP, ON);

				ControlLight(GREEN_BOTTOM, OFF);
				ControlLight(GREEN_TOP, OFF);
				lightsActivated = true;
			}
			if(!delaySent){
				action.KeepStateFor = YELLOW_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}
			break;
	}
}


void TwoLane(ButtonStates button){
	switch(state){
		case V_Active:

			nextState = Transition_To_H;
			if(!lightsActivated){
				ControlLight(GREEN_BOTTOM, ON);
				ControlLight(GREEN_TOP, ON);

				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);
				ControlLight(YELLOW_RIGHT, OFF);
				ControlLight(YELLOW_LEFT, OFF);

				ControlLight(RED_LEFT, ON);
				ControlLight(RED_RIGHT, ON);
				lightsActivated = true;
			}

			if(!NoCarH(button) && NoCarV(button)){
				action.KeepStateFor = 1;
				action.StartTimerForNextState = true;
				delaySent = true;
			} else if (!NoCarH(button) && !NoCarV(button) && !delaySent) {
				action.KeepStateFor = RED_DELAY_MAX;
				action.StartTimerForNextState = true;
				delaySent = true;

			} else if (NoCarH(button) && NoCarV(button) && !delaySent){
				action.KeepStateFor = GREEN_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			break;
		case H_Active:

			nextState = Transition_To_V;
			if(!lightsActivated){
				ControlLight(GREEN_RIGHT, ON);
				ControlLight(GREEN_LEFT, ON);

				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);
				ControlLight(YELLOW_RIGHT, OFF);
				ControlLight(YELLOW_LEFT, OFF);

				ControlLight(RED_TOP, ON);
				ControlLight(RED_BOTTOM, ON);
				lightsActivated = true;
			}

			if(NoCarH(button) && !NoCarV(button)){
				action.KeepStateFor = 1;
				action.StartTimerForNextState = true;
				delaySent = true;
			} else if (!NoCarH(button) && !NoCarV(button) && !delaySent) {
				action.KeepStateFor = RED_DELAY_MAX;
				action.StartTimerForNextState = true;
				delaySent = true;

			} else if (NoCarH(button) && NoCarV(button) && !delaySent){
				action.KeepStateFor = GREEN_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			break;
		case Transition_To_H:

			nextState = H_Active;
			if(!lightsActivated){
				ControlLight(YELLOW_BOTTOM, ON);
				ControlLight(YELLOW_TOP, ON);
				ControlLight(YELLOW_RIGHT, ON);
				ControlLight(YELLOW_LEFT, ON);

				ControlLight(GREEN_TOP, OFF);
				ControlLight(GREEN_BOTTOM, OFF);

				ControlLight(RED_RIGHT, OFF);
				ControlLight(RED_LEFT, OFF);
				lightsActivated = true;
			}

			if(!delaySent){
				action.KeepStateFor = YELLOW_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			break;
		case Transition_To_V:

			nextState = V_Active;
			if(!lightsActivated){
				ControlLight(YELLOW_BOTTOM, ON);
				ControlLight(YELLOW_TOP, ON);
				ControlLight(YELLOW_RIGHT, ON);
				ControlLight(YELLOW_LEFT, ON);

				ControlLight(GREEN_RIGHT, OFF);
				ControlLight(GREEN_LEFT, OFF);

				ControlLight(RED_TOP, OFF);
				ControlLight(RED_BOTTOM, OFF);
				lightsActivated = true;
			}

			if(!delaySent){
				action.KeepStateFor = YELLOW_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			break;

	}
}

void TwoLanePed(ButtonStates button){
	switch(state){
		case V_Active:
			nextState = Transition_To_H;
			if(!lightsActivated){
				ControlLight(GREEN_BOTTOM, ON);
				ControlLight(GREEN_TOP, ON);

				ControlLight(RED_LEFT, ON);
				ControlLight(RED_RIGHT, ON);

				ControlLight(GREEN_PED_TOP, OFF);
				ControlLight(GREEN_PED_LEFT, OFF);

				ControlLight(RED_PED_TOP, ON);
				ControlLight(RED_PED_LEFT, ON);

				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);
				ControlLight(YELLOW_LEFT, OFF);
				ControlLight(YELLOW_RIGHT, OFF);

				lightsActivated = true;
			}

			if(button.LeftPed){
				nextState = V_P_Active;
				action.KeepStateFor = 1;
				action.StartTimerForNextState = true;
				previousDelayExpireAt = HAL_GetTick() + 1;
			} else if(!NoCarH(button) && NoCarV(button)) {
				action.KeepStateFor = 1;
				action.StartTimerForNextState = true;
				previousDelayExpireAt = HAL_GetTick() + 1;
			}
			if (!NoCarH(button) && !NoCarV(button) && previousDelayExpireAt > (HAL_GetTick() + RED_DELAY_MAX)) {
				action.KeepStateFor = RED_DELAY_MAX;
				action.StartTimerForNextState = true;
				previousDelayExpireAt = HAL_GetTick() + RED_DELAY_MAX;
			} else if (NoCarH(button) && NoCarV(button) && previousDelayExpireAt > (HAL_GetTick() + GREEN_DELAY)) {
				action.KeepStateFor = GREEN_DELAY;
				action.StartTimerForNextState = true;
				previousDelayExpireAt = HAL_GetTick() + GREEN_DELAY;
			}

			if(button.TopPed){
				action.TopPedWaiting = true;
				action.KeepStateFor = PEDESTRIAN_DELAY;
				action.StartTimerForNextState = true;
				previousDelayExpireAt = HAL_GetTick() + PEDESTRIAN_DELAY;
			}

			break;

		case V_P_Active:
			if(action.TopPedWaiting){
				nextState = Transition_To_H;
			} else if (!NoCarV(button) && NoCarH(button)){
				nextState = V_Active;
			}else {
				nextState = Transition_To_H;
			}

			if(!lightsActivated){
				ControlLight(GREEN_BOTTOM, ON);
				ControlLight(GREEN_TOP, ON);

				ControlLight(RED_LEFT, ON);
				ControlLight(RED_RIGHT, ON);

				ControlLight(GREEN_PED_TOP, OFF);
				ControlLight(GREEN_PED_LEFT, ON);
				action.LeftPedWaiting = false;

				ControlLight(RED_PED_TOP, ON);
				ControlLight(RED_PED_LEFT, OFF);

				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);
				ControlLight(YELLOW_LEFT, OFF);
				ControlLight(YELLOW_RIGHT, OFF);

				lightsActivated = true;
			}

			if(!delaySent){
				action.KeepStateFor = WALKING_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			if(button.TopPed){
				action.TopPedWaiting = true;
			}

			break;

		case H_Active:
			nextState = Transition_To_V;
			if(!lightsActivated){
				ControlLight(GREEN_LEFT, ON);
				ControlLight(GREEN_RIGHT, ON);

				ControlLight(RED_TOP, ON);
				ControlLight(RED_BOTTOM, ON);

				ControlLight(GREEN_PED_TOP, OFF);
				ControlLight(GREEN_PED_LEFT, OFF);

				ControlLight(RED_PED_TOP, ON);
				ControlLight(RED_PED_LEFT, ON);

				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);
				ControlLight(YELLOW_LEFT, OFF);
				ControlLight(YELLOW_RIGHT, OFF);

				lightsActivated = true;
			}

			if(button.TopPed){
				nextState = H_P_Active;
				action.KeepStateFor = 1;
				action.StartTimerForNextState = true;
				delaySent = true;
				previousDelayExpireAt = HAL_GetTick() + 1;
			} else if(NoCarH(button) && !NoCarV(button)) {
				action.KeepStateFor = 1;
				action.StartTimerForNextState = true;
				delaySent = true;
				previousDelayExpireAt = HAL_GetTick() + 1;
			}
			if (!NoCarH(button) && !NoCarV(button) && previousDelayExpireAt > (HAL_GetTick() + RED_DELAY_MAX)) {
				action.KeepStateFor = RED_DELAY_MAX;
				action.StartTimerForNextState = true;
				delaySent = true;
				previousDelayExpireAt = HAL_GetTick() + RED_DELAY_MAX;
			} else if (NoCarH(button) && NoCarV(button) && previousDelayExpireAt > (HAL_GetTick() + GREEN_DELAY)) {
				action.KeepStateFor = GREEN_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
				previousDelayExpireAt = HAL_GetTick() + GREEN_DELAY;
			}

			if(button.LeftPed){ //Press pd at same time error
				action.LeftPedWaiting = true;
				action.KeepStateFor = PEDESTRIAN_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
				previousDelayExpireAt = HAL_GetTick() + PEDESTRIAN_DELAY;
			}

			break;

		case H_P_Active:
			if(action.LeftPedWaiting){
				nextState = Transition_To_V;
			} else if (!NoCarH(button) && NoCarV(button)){
				nextState = H_Active;
			} else {
				nextState = Transition_To_V;
			}
			if(!lightsActivated){
				ControlLight(GREEN_LEFT, ON);
				ControlLight(GREEN_RIGHT, ON);

				ControlLight(RED_TOP, ON);
				ControlLight(RED_BOTTOM, ON);

				ControlLight(GREEN_PED_TOP, ON);
				action.TopPedWaiting = false;
				ControlLight(GREEN_PED_LEFT, OFF);

				ControlLight(RED_PED_TOP, OFF);
				ControlLight(RED_PED_LEFT, ON);

				ControlLight(YELLOW_BOTTOM, OFF);
				ControlLight(YELLOW_TOP, OFF);
				ControlLight(YELLOW_LEFT, OFF);
				ControlLight(YELLOW_RIGHT, OFF);

				lightsActivated = true;
			}
			if(!delaySent){
				action.KeepStateFor = WALKING_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			if(button.LeftPed){
				action.LeftPedWaiting = true;
			}

			break;

		case Transition_To_H:
			if(!lightsActivated){
				ControlLight(GREEN_LEFT, OFF);
				ControlLight(GREEN_RIGHT, OFF);
				ControlLight(GREEN_TOP, OFF);
				ControlLight(GREEN_BOTTOM, OFF);

				ControlLight(RED_TOP, OFF);
				ControlLight(RED_BOTTOM, OFF);
				ControlLight(RED_LEFT, OFF);
				ControlLight(RED_RIGHT, OFF);

				ControlLight(GREEN_PED_TOP, OFF);
				ControlLight(GREEN_PED_LEFT, OFF);

				ControlLight(RED_PED_TOP, ON);
				ControlLight(RED_PED_LEFT, ON);

				ControlLight(YELLOW_BOTTOM, ON);
				ControlLight(YELLOW_TOP, ON);
				ControlLight(YELLOW_LEFT, ON);
				ControlLight(YELLOW_RIGHT, ON);
				lightsActivated = true;
			}

			if(button.TopPed){
				action.TopPedWaiting = true;
			}
			if(button.LeftPed){
				action.LeftPedWaiting = true;
			}
			if(action.TopPedWaiting){
				nextState = H_P_Active;
			}else{
				nextState = H_Active;
			}

			if(!delaySent){
				action.KeepStateFor = YELLOW_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			break;

		case Transition_To_V:
			if(!lightsActivated){
				ControlLight(GREEN_LEFT, OFF);
				ControlLight(GREEN_RIGHT, OFF);
				ControlLight(GREEN_TOP, OFF);
				ControlLight(GREEN_BOTTOM, OFF);

				ControlLight(RED_TOP, OFF);
				ControlLight(RED_RIGHT, OFF);
				ControlLight(RED_BOTTOM, OFF);
				ControlLight(RED_LEFT, OFF);

				ControlLight(GREEN_PED_TOP, OFF);
				ControlLight(GREEN_PED_LEFT, OFF);

				ControlLight(RED_PED_TOP, ON);
				ControlLight(RED_PED_LEFT, ON);

				ControlLight(YELLOW_BOTTOM, ON);
				ControlLight(YELLOW_TOP, ON);
				ControlLight(YELLOW_LEFT, ON);
				ControlLight(YELLOW_RIGHT, ON);
				lightsActivated = true;
			}
			if(button.LeftPed){
				action.LeftPedWaiting = true;
			}
			if(button.TopPed){
				action.TopPedWaiting = true;
			}

			if(action.LeftPedWaiting){
				nextState = V_P_Active;
			}else{
				nextState = V_Active;
			}

			if(!delaySent){
				action.KeepStateFor = YELLOW_DELAY;
				action.StartTimerForNextState = true;
				delaySent = true;
			}

			break;
	}
}

bool NoCarV(ButtonStates button){
	return !(button.TopCar | button.BottomCar);
}

bool NoCarH(ButtonStates button){
	return !(button.LeftCar | button.RightCar);
}

