/*
 * test.h
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */

#ifndef INC_TEST_H_
#define INC_TEST_H_

#include "main.h"
#include "stdbool.h"
#include "trafficLights.h"
#include "TrafficInputs.h"
#include "TrafficLogic.h"

void Test_Program();
bool hasError();
void testHC595();
void testLights(void);
void testInputs(void);
void testBlueLightToggle();
void testLightLogic();
void testFasterLight();
void testTrafficLogicOneLane();
void testTrafficLogicTwoLanePed();
void simulateTestScenario(ButtonStates testInput);

#endif /* INC_TEST_H_ */
