/*
 * trafficLights.h
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */

#ifndef INC_TRAFFICLIGHTS_H_
#define INC_TRAFFICLIGHTS_H_

#include "main.h"
#include "hc595.h"
#include "stdbool.h"

#define RED_RIGHT 		0
#define YELLOW_RIGHT 	1
#define GREEN_RIGHT 	2
#define RED_TOP 		3
#define YELLOW_TOP 		4
#define GREEN_TOP 		5

#define RED_BOTTOM 		6
#define YELLOW_BOTTOM 	7
#define GREEN_BOTTOM 	8
#define RED_PED_TOP 	9
#define GREEN_PED_TOP 	10
#define BLUE_PED_TOP 	11

#define RED_LEFT 		12
#define YELLOW_LEFT 	13
#define GREEN_LEFT 		14
#define RED_PED_LEFT 	15
#define GREEN_PED_LEFT 	16
#define BLUE_PED_LEFT 	17

typedef enum {
    OFF = 	0,
    ON = 	1
} LightState;

typedef struct {
    int bufferIndex;
    int bitPosition;
} LightConfig;

void initTrafficLights(SPI_HandleTypeDef *_hspi3);

void setLight(LightConfig config, LightState state);

void ControlLight(int lightId, LightState state);

void turnOnAllLights(void);

void updateLights(void);

void turnOffAllLights(void);

void toggleBlueLeft(void);

void toggleBlueTop(void);

void setLightsState(uint8_t lightMasks[3], LightState state);


/*********** Testing functions **************/

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


#endif /* INC_TRAFFICLIGHTS_H_ */
