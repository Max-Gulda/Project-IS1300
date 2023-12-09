/*
 * test.c
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */
#include "test.h"

#define TEST_SPEED 100



//void activateTopBottom();
//void activateLeftRight();
//void activateTransition();
TrafficCrossingAction *TrafficActionTest;

void Test_Program(){
	for(int i = 0; i < 10; i++) printf("\n\r");
	//initTrafficLogic(PedOneLane);
	//initTrafficLogic(NoPedTwoLane);
	printf("Starting testing! \n \r");
	initTrafficLogic(PedTwoLane);
	updateLights();
	TrafficActionTest = getTrafficAction();
	testStateMachine();
	//testTrafficLogicTwoLanePed();
	while(1){
		turnOnAllLights();
		updateLights();
		HAL_Delay(50);
		turnOffAllLights();
		updateLights();
		HAL_Delay(1000);
		//testTrafficLogicOneLane();
		//testFasterLight();
		//testLightLogic();
		//testBlueLightToggle();
		//testInputs();
		//testLights();
		//testHC595();
	}
}

void printState(void) {
    TrafficCrossingState printState = getState();
    printf("-------------------------State Info START-------------------------\n\r");
    char tempStr[30] = {0};
    switch (printState) {
        case H_Active:
            strcpy(tempStr, "Horizontal Active");
            break;
        case V_Active:
            strcpy(tempStr, "Vertical Active");
            break;
        case V_P_Active:
            strcpy(tempStr, "Vertical Pedestrian Active");
            break;
        case H_P_Active:
            strcpy(tempStr, "Horizontal Pedestrian Active");
            break;
        case Transition_To_H:
            strcpy(tempStr, "Transition to Horizontal Active");
            break;
        case Transition_To_V:
            strcpy(tempStr, "Transition to Vertical Active");
            break;
        default:
            strcpy(tempStr, "Unknown State");
    }
    printf("State = %s \n \r", tempStr);
    if(TrafficActionTest->SwitchImidiate){
		printf("State should switch immediately. \n \r");
		TrafficActionTest->SwitchImidiate = false;
	}else if(TrafficActionTest->AbortTimer){
		TrafficActionTest->AbortTimer = false;
		printf("Timer should be aborted! \n \r");
	}else if(TrafficActionTest->StartTimerForNextState){
    	printf("State should switch in %d ms \n \r", TrafficActionTest->KeepStateFor);
    	TrafficActionTest->StartTimerForNextState = false;
    }



    if(TrafficActionTest->LeftPedWaiting){
    	printf("Left pedestrian button was pressed. \n \r");
    }
    if(TrafficActionTest->TopPedWaiting){
    	printf("Top pedestrian button was pressed. \n \r");
    }

    printf("--------------------------State Info END--------------------------\n\r\n\r");

}


bool testForError(int16_t delay, bool startDelay, bool topPed, bool leftPed, bool abortTimer, bool switchImidiate, TrafficCrossingState testState){
	if(testState != getState()){
		printf("Error code: 1 \n \r");
		return false;
	}

	if(topPed != TrafficActionTest->TopPedWaiting){
		printf("Error code: 2 \n \r");
		return false;
	}

	if(leftPed != TrafficActionTest->LeftPedWaiting){
		printf("Error code: 3 \n \r");
		return false;
	}

	if(switchImidiate && TrafficActionTest->SwitchImidiate){
		return true;
	}

	if(abortTimer && TrafficActionTest->AbortTimer){
		return true;
	}

	if(startDelay){
		if(delay != TrafficActionTest->KeepStateFor){
			printf("Error code: 4 \n \r");
			return false;
		}
		if(!TrafficActionTest->KeepStateFor){
			printf("Error code: 5 \n \r");
			return false;
		}
	}

	return true;
}


void testStateMachine(void){
	ButtonStates testScenarios[10] = {
	    {false, false, 	false, 	false, 	false, 	false},
	    {false, true, 	false, 	false, 	false, 	false},
	    {false, true, 	true, 	false, 	false, 	false},
	    {false, false, 	false, 	true, 	false, 	false},
	    {false, false, 	false, 	false, 	true, 	false},
	    {false, false, 	false, 	false, 	false, 	true },
		{true, 	true, 	true, 	true, 	true, 	true },
		{true, 	true, 	true, 	true, 	false, 	false}
	};

	TrafficCrossing(testScenarios[0]);
	printf("Expected state is Vertical Active \n \r");
	printf("State should switch in %d ms\n\r", GREEN_DELAY);
	printState();
	if(!testForError(GREEN_DELAY,true,false,false,false,false,V_Active)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	TrafficCrossing(testScenarios[1]);
	printf("Expected state is Vertical Active \n \r");
	printf("Previous timer should be aborted \n \r");
	printState();
	if(!testForError(0,false,false,false,true,false,V_Active)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	TrafficCrossing(testScenarios[0]);
	printf("Expected state is Vertical Active \n \r");
	printf("State should switch in %d ms\n\r", GREEN_DELAY);
	printState();

	if(!testForError(GREEN_DELAY,true,false,false,false,false,V_Active)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();

	HAL_Delay(TEST_SPEED);

	TrafficCrossing(testScenarios[0]);
	printf("Expected state is Transition to Horizontal \n \r");
	printf("State should switch in %d ms\n\r", YELLOW_DELAY);
	printState();

	if(!testForError(YELLOW_DELAY,true,false,false,false,false,Transition_To_H)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	TrafficCrossing(testScenarios[5]);
	printf("Expected state is Transition to Horizontal \n \r");
	printf("State should switch in %d ms\n\r", YELLOW_DELAY);
	printState();

	if(!testForError(YELLOW_DELAY,false,true,false,false,false,Transition_To_H)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();

	HAL_Delay(TEST_SPEED);

	TrafficCrossing(testScenarios[0]);
	printf("Expected state is Horizontal Pedestrian Active \n\r");
	printf("State should switch in %d ms\n\r", WALKING_DELAY);
	printState();

	if(!testForError(WALKING_DELAY,false,false,false,false,false,H_P_Active)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();

	HAL_Delay(TEST_SPEED);

	TrafficCrossing(testScenarios[0]);
	printf("Expected state is Transition to Vertical\n\r");
	printf("Expected delay is %d ms\n\r", YELLOW_DELAY);
	printState();

	if(!testForError(YELLOW_DELAY,false,false,false,false,false,Transition_To_V)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();

	HAL_Delay(TEST_SPEED);


	TrafficCrossing(testScenarios[6]);
	printf("Expected state is Vertical active \n\r");
	printf("State should switch immediately\n\r");
	printState();
	if(!testForError(0,false,true,false,false,true,V_Active)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();

	HAL_Delay(TEST_SPEED);

	TrafficCrossing(testScenarios[6]);
	printf("Expected state is Vertical pedestrian active \n \r");
	printf("Expected delay is %d ms \n\r", WALKING_DELAY);
	printf("Top pedestrian is waiting\n\r");
	printState();
	if(!testForError(WALKING_DELAY,false,true,false,false,false,V_P_Active)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();
	TrafficCrossing(testScenarios[7]);
	printf("Expected state is transition to horizontal\n\r");
	printf("Expected delay is %d ms\n\r", YELLOW_DELAY);
	printf("Top pedestrian is waiting \n \r");
	printState();
	if(!testForError(YELLOW_DELAY,false,true,false,false,false,Transition_To_H)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();
	TrafficCrossing(testScenarios[7]);
	printf("Expected state is Horizontal Pedestrian active\n\r");
	printf("Expected delay is %d ms\n\r", WALKING_DELAY);
	printState();
	if(!testForError(WALKING_DELAY,false,false,false,false,false,H_P_Active)){
		programError();
	}

	HAL_Delay(TEST_SPEED);


	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();
	TrafficCrossing(testScenarios[7]);
	printf("Expected state is transition to vertical\n\r");
	printf("Expected delay is %d ms\n\r", YELLOW_DELAY);
	printState();

	if(!testForError(YELLOW_DELAY,false,false,false,false,false,Transition_To_V)){
		programError();
	}

	HAL_Delay(TEST_SPEED);

	printf("\n\r**** Activating next state **** \n\n\r");
	ActivateNextState();
	TrafficCrossing(testScenarios[7]);
	printf("Expected state is vertical active\n\r");
	printf("Expected delay is %d ms\n\r", RED_DELAY_MAX);
	printState();
	if(!testForError(RED_DELAY_MAX,false,false,false,false,false,V_Active)){
		programError();
	}


}

void programError(void){
	while(1){
		turnOnAllLights();
		updateLights();
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
