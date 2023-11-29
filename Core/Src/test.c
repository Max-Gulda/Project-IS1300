/*
 * test.c
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */
#include "test.h"

#define TEST_SPEED 200

static void testLights(void);


//void activateTopBottom();
//void activateLeftRight();
//void activateTransition();
TrafficCrossingAction *TrafficActionTest;

void Test_Program(){
	//initTrafficLogic(PedOneLane);
	//initTrafficLogic(NoPedTwoLane);
	initTrafficLogic(PedTwoLane);
	updateLights();
	TrafficActionTest = getTrafficAction();
	testTrafficLogicTwoLanePed();
	while(1){
		turnOnAllLights();
		updateLights();
		HAL_Delay(50);
		turnOffAllLights();
		updateLights();
		HAL_Delay(50);
		//testTrafficLogicOneLane();
		//testFasterLight();
		//testLightLogic();
		//testBlueLightToggle();
		//testInputs();
		//testLights();
		//testHC595();
	}
}

// Function to simulate one test scenario
void simulateTestScenario(ButtonStates testInput) {

    // Run your traffic logic
    TrafficCrossing(testInput); // Use the fake input state
    if(TrafficActionTest->LeftPedWaiting){
		ControlLight(BLUE_PED_LEFT, ON);
	} else {
		ControlLight(BLUE_PED_LEFT, OFF);
	}

	if(TrafficActionTest->TopPedWaiting){
		ControlLight(BLUE_PED_TOP, ON);
	} else {
		ControlLight(BLUE_PED_TOP, OFF);
	}
    updateLights();
}


void testTrafficLogicTwoLanePed() {
	int startAt = 1;
    for (int i = 0; i < 64; i++) { //2^6 different scenarios
        ButtonStates scenario;
        scenario.LeftCar = (i & 0x01) != 0; // 1st bit
        scenario.BottomCar = (i & 0x02) != 0; // 2nd bit
        scenario.RightCar = (i & 0x04) != 0; // 3rd bit
        scenario.TopCar = (i & 0x08) != 0; // 4th bit
        scenario.LeftPed = (i & 0x10) != 0; // 5th bit
        scenario.TopPed = (i & 0x20) != 0; // 6th bit

        for(int j = 0; j < 10; j++){ //Test each scenario ten times.
        	simulateTestScenario(scenario);

			if (TrafficActionTest->StartTimerForNextState) {
				if(TrafficActionTest->KeepStateFor == 1){ //Indicate "no delay" through blinking both blue lights.
					toggleBlueLeft();
					toggleBlueTop();
					updateLights();
					HAL_Delay(200/10);
					toggleBlueLeft();
					toggleBlueTop();
					updateLights();
					HAL_Delay(200/10);
				}else{
					HAL_Delay(TrafficActionTest->KeepStateFor/10);
				}

				TrafficActionTest->StartTimerForNextState = false;
				ActivateNextState();
			}else{
				toggleBlueLeft();
				toggleBlueTop();
				updateLights();
				HAL_Delay(200/10);
				toggleBlueLeft();
				toggleBlueTop();
				updateLights();
				HAL_Delay(200/10);
			}
			if(hasError()){
				while(1){
					turnOnAllLights();
					updateLights();
				}
			}

        }


    }
}




void testTrafficLogicOneLane(){
	trafficInputs_Update();
	TrafficCrossing(getInputState());
	updateLights();

	if(TrafficActionTest->LeftPedWaiting){
		ControlLight(BLUE_PED_LEFT, ON);
		updateLights();
	} else {
		ControlLight(BLUE_PED_LEFT, OFF);
		updateLights();
	}

	if(TrafficActionTest->TopPedWaiting){
		ControlLight(BLUE_PED_TOP, ON);
		updateLights();
	} else {
		ControlLight(BLUE_PED_TOP, OFF);
		updateLights();
	}
	if(TrafficActionTest->StartTimerForNextState){
		HAL_Delay(TrafficActionTest->KeepStateFor);
		TrafficActionTest->StartTimerForNextState = false;
		ActivateNextState();
	}

}

void testFasterLight(){
	uint8_t lightsToTurnOn[3] = {0b00000101, 0b00000111, 0b00011111};
	setLightsState(lightsToTurnOn, ON);
	updateLights();
	HAL_Delay(1000);

	setLightsState(lightsToTurnOn, OFF);
	updateLights();
	HAL_Delay(500);
}

void testLightLogic(){
	activateTopBottom();
	updateLights();
	HAL_Delay(2000);

	activateTransition();
	updateLights();
	HAL_Delay(2000);

	activateLeftRight();
	updateLights();
	HAL_Delay(2000);

	activateTransition();
	updateLights();
	HAL_Delay(2000);
}

void testBlueLightToggle(){
	toggleBlueLeft();
	toggleBlueTop();
	updateLights();
	HAL_Delay(TEST_SPEED);
}

void testInputs(void){
	trafficInputs_Update();
	ButtonStates inputStates = getInputState();

			if(inputStates.LeftCar){
				ControlLight(RED_LEFT, ON);
				ControlLight(GREEN_LEFT, ON);
				ControlLight(YELLOW_LEFT, ON);
			}else{
				ControlLight(RED_LEFT, OFF);
				ControlLight(GREEN_LEFT, OFF);
				ControlLight(YELLOW_LEFT, OFF);
			}

			if(inputStates.BottomCar){
				ControlLight(RED_BOTTOM, ON);
				ControlLight(GREEN_BOTTOM, ON);
				ControlLight(YELLOW_BOTTOM, ON);
			}else{
				ControlLight(RED_BOTTOM, OFF);
				ControlLight(GREEN_BOTTOM, OFF);
				ControlLight(YELLOW_BOTTOM, OFF);
			}

			if(inputStates.RightCar){
				ControlLight(RED_RIGHT, ON);
				ControlLight(GREEN_RIGHT, ON);
				ControlLight(YELLOW_RIGHT, ON);
			}else{
				ControlLight(RED_RIGHT, OFF);
				ControlLight(GREEN_RIGHT, OFF);
				ControlLight(YELLOW_RIGHT, OFF);
			}

			if(inputStates.TopCar){
				ControlLight(RED_TOP, ON);
				ControlLight(GREEN_TOP, ON);
				ControlLight(YELLOW_TOP, ON);
			}else{
				ControlLight(RED_TOP, OFF);
				ControlLight(GREEN_TOP, OFF);
				ControlLight(YELLOW_TOP, OFF);
			}

			if(inputStates.LeftPed){
				ControlLight(BLUE_PED_LEFT, ON);
				ControlLight(GREEN_PED_LEFT, ON);
				ControlLight(RED_PED_LEFT, ON);
			}else{
				ControlLight(BLUE_PED_LEFT, OFF);
				ControlLight(GREEN_PED_LEFT, OFF);
				ControlLight(RED_PED_LEFT, OFF);
			}

			if(inputStates.TopPed){
				ControlLight(BLUE_PED_TOP, ON);
				ControlLight(GREEN_PED_TOP, ON);
				ControlLight(RED_PED_TOP, ON);
			}else{
				ControlLight(BLUE_PED_TOP, OFF);
				ControlLight(GREEN_PED_TOP, OFF);
				ControlLight(RED_PED_TOP, OFF);
			}
			updateLights();
}

void testLights(void) {

	ControlLight(RED_RIGHT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(RED_RIGHT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(YELLOW_RIGHT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(YELLOW_RIGHT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/


	ControlLight(GREEN_RIGHT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(GREEN_RIGHT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(RED_TOP,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(RED_TOP,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(YELLOW_TOP,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(YELLOW_TOP,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(GREEN_TOP,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(GREEN_TOP,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(RED_BOTTOM,ON); // grön ?
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(RED_BOTTOM,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(YELLOW_BOTTOM,ON); //Röd ped top
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(YELLOW_BOTTOM,OFF); //Röd ped top
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(GREEN_BOTTOM,ON); //Grön Top
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(GREEN_BOTTOM,OFF); //Grön top
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(RED_PED_TOP,ON); //Blå top
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(RED_PED_TOP,OFF); //Blå Top
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(GREEN_PED_TOP,ON); //Röd left
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(GREEN_PED_TOP,OFF); //Röd left
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(BLUE_PED_TOP,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(BLUE_PED_TOP,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(RED_LEFT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(RED_LEFT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(YELLOW_LEFT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(YELLOW_LEFT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(GREEN_LEFT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(GREEN_LEFT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(RED_PED_LEFT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(RED_PED_LEFT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(GREEN_PED_LEFT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(GREEN_PED_LEFT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/

	ControlLight(BLUE_PED_LEFT,ON);
	updateLights();
	HAL_Delay(TEST_SPEED);

	ControlLight(BLUE_PED_LEFT,OFF);
	updateLights();
	HAL_Delay(TEST_SPEED);

	/*********************************************/
	turnOnAllLights();
	updateLights();
	HAL_Delay(TEST_SPEED);


    turnOffAllLights();
    updateLights();
}

void testHC595(){
	uint8_t buff[3] = {0};
	buff[0] = 0b00011111;
	buff[1] = 0b00011111;
	buff[2] = 0b00011111;
	hc595_update(buff);
	HAL_Delay(TEST_SPEED);

	buff[0] = 0b11100000;
	buff[1] = 0b11100000;
	buff[2] = 0b11100000;
	hc595_update(buff);
	HAL_Delay(TEST_SPEED);
}
/*
bool GetRedRight();
bool GetYellowRight();
bool GetGreenRight();

bool GetRedTop();
bool GetYellowTop();
bool GetGreenTop();

bool GetRedBottom();
bool GetYellowBottom();
bool GetGreenBottom();

bool GetRedPedTop();
bool GetGreenPedTop();
bool GetBluePedTop();

bool GetRedLeft();
bool GetYellowLeft();
bool GetGreenLeft();

bool GetRedPedLeft();
bool GetGreenPedLeft();
bool GetBluePedLeft();
*/
bool hasError(){
	//Two conflicting lanes green at the same time
	if(GetGreenTop() || GetGreenBottom()){
		if(!(GetRedLeft() && GetRedRight())){
			return true;
		}
	}

	if(GetGreenLeft() || GetGreenRight()){
		if(!(GetRedTop() && GetRedBottom())){
			return true;
		}
	}

	if(GetGreenPedLeft()){
		if(!(GetRedLeft() && GetRedRight())){
			return true;
		}
	}

	if(GetGreenPedTop()){
		if(!(GetRedTop() && GetRedBottom())){
			return true;
		}
	}

	//Two lights on the same traffic light is on at the same time
	if (!((GetRedTop() ^ GetYellowTop() ^ GetGreenTop()) && !(GetRedTop() && GetYellowTop() && GetGreenTop()))) {
		return true;
	}
	if (!((GetRedBottom() ^ GetYellowBottom() ^ GetGreenBottom()) && !(GetRedBottom() && GetYellowBottom() && GetGreenBottom()))) {
		return true;
	}
	if (!((GetRedLeft() ^ GetYellowLeft() ^ GetGreenLeft()) && !(GetRedLeft() && GetYellowLeft() && GetGreenLeft()))) {
		return true;
	}
	if (!((GetRedRight() ^ GetYellowRight() ^ GetGreenRight()) && !(GetRedRight() && GetYellowRight() && GetGreenRight()))) {
		return true;
	}

	// Check for pedestrian lights
	if (GetGreenPedTop() && GetRedPedTop()) {
		return true;
	}
	if (GetGreenPedLeft() && GetRedPedLeft()) {
		return true;
	}

	return false;
}
