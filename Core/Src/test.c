/*
 * test.c
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */
#include "test.h"

#define TEST_SPEED 200

static void testLights(void);

void Test_Program(){
	while(1){
		//testBlueLightToggle();
		testInputs();
		//testLights();
		//testHC595();
	}
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
