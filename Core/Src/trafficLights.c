/*
 * trafficLights.c
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */

#include "trafficLights.h"
#include "stdbool.h"

static const LightConfig lightConfigs[] = {
    {0, 0}, // RED_RIGHT
    {0, 1}, // YELLOW_RIGHT
    {0, 2}, // GREEN_RIGHT
    {0, 3}, // RED_TOP
    {0, 4}, // YELLOW_TOP
    {0, 5}, // GREEN_TOP

    {1, 0}, // RED_BOTTOM
    {1, 1}, // YELLOW_BOTTOM
    {1, 2}, // GREEN_BOTTOM
    {1, 3}, // RED_PED_TOP
    {1, 4}, // GREEN_PED_TOP
    {1, 5}, // BLUE_PED_TOP

    {2, 0}, // RED_LEFT
    {2, 1}, // YELLOW_LEFT
    {2, 2}, // GREEN_LEFT
    {2, 3}, // RED_PED_LEFT
    {2, 4}, // GREEN_PED_LEFT
    {2, 5}  // BLUE_PED_LEFT
};

static uint8_t buffer[3] = {0, 0, 0};
static bool lightsChanged;

void initTrafficLights(SPI_HandleTypeDef *_hspi3){
	lightsChanged = false;
	hc595_init(_hspi3);
	for(int i = 0; i < 3; i++){
		buffer[i] = 0;
	}
}

void setLight(LightConfig config, LightState state) {
    uint8_t currentMask = (1 << config.bitPosition);
    bool isLightCurrentlyOn = (buffer[config.bufferIndex] & currentMask) != 0;
    bool isLightBeingTurnedOn = (state == ON);

    if (isLightCurrentlyOn != isLightBeingTurnedOn) {
        lightsChanged = true;

        if (isLightBeingTurnedOn) {
            buffer[config.bufferIndex] |= currentMask;
        } else {
            buffer[config.bufferIndex] &= ~currentMask;
        }
    }
}


void ControlLight(int lightId, LightState state) {
    setLight(lightConfigs[lightId], state);
}

void updateLights(){
	if(lightsChanged){
		hc595_update(buffer);
		lightsChanged = false;
	}
}

void toggleBlueLeft() {
	buffer[2] ^= 0b00100000;
	lightsChanged = true;
}

void toggleBlueTop() {
	buffer[1] ^= 0b00100000;
	lightsChanged = true;
}

void turnOffAllLights() {
	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;
	lightsChanged = true;
}


void turnOnAllLights(){
	buffer[0] = 0b11111111;
	buffer[1] = 0b11111111;
	buffer[2] = 0b11111111;
	lightsChanged = true;
}

void setLightsState(uint8_t lightMasks[3], LightState state) {
    for (int i = 0; i < 3; i++) {
        if (state == ON) {
            buffer[i] |= lightMasks[i]; // Turn on lights specified in the mask
        } else {
            buffer[i] &= ~lightMasks[i]; // Turn off lights specified in the mask
        }
    }
    lightsChanged = true;
}

/********* Functions only used for testing **************/
bool GetRedRight() {
    return (buffer[lightConfigs[RED_RIGHT].bufferIndex] 		& (1 << lightConfigs[RED_RIGHT].bitPosition)) != 0;
}

bool GetYellowRight() {
	return (buffer[lightConfigs[YELLOW_RIGHT].bufferIndex] 		& (1 << lightConfigs[YELLOW_RIGHT].bitPosition)) != 0;
}

bool GetGreenRight() {
    return (buffer[lightConfigs[GREEN_RIGHT].bufferIndex] 		& (1 << lightConfigs[GREEN_RIGHT].bitPosition)) != 0;
}

bool GetRedTop() {
    return (buffer[lightConfigs[RED_TOP].bufferIndex] 			& (1 << lightConfigs[RED_TOP].bitPosition)) != 0;
}

bool GetYellowTop() {
    return (buffer[lightConfigs[YELLOW_TOP].bufferIndex] 		& (1 << lightConfigs[YELLOW_TOP].bitPosition)) != 0;
}

bool GetGreenTop() {
    return (buffer[lightConfigs[GREEN_TOP].bufferIndex] 		& (1 << lightConfigs[GREEN_TOP].bitPosition)) != 0;
}

bool GetRedBottom() {
    return (buffer[lightConfigs[RED_BOTTOM].bufferIndex] 		& (1 << lightConfigs[RED_BOTTOM].bitPosition)) != 0;
}

bool GetYellowBottom() {
    return (buffer[lightConfigs[YELLOW_BOTTOM].bufferIndex] 	& (1 << lightConfigs[YELLOW_BOTTOM].bitPosition)) != 0;
}

bool GetGreenBottom() {
    return (buffer[lightConfigs[GREEN_BOTTOM].bufferIndex] 		& (1 << lightConfigs[GREEN_BOTTOM].bitPosition)) != 0;
}

bool GetRedPedTop() {
    return (buffer[lightConfigs[RED_PED_TOP].bufferIndex] 		& (1 << lightConfigs[RED_PED_TOP].bitPosition)) != 0;
}

bool GetGreenPedTop() {
    return (buffer[lightConfigs[GREEN_PED_TOP].bufferIndex] 	& (1 << lightConfigs[GREEN_PED_TOP].bitPosition)) != 0;
}

bool GetBluePedTop() {
    return (buffer[lightConfigs[BLUE_PED_TOP].bufferIndex] 		& (1 << lightConfigs[BLUE_PED_TOP].bitPosition)) != 0;
}

bool GetRedLeft() {
    return (buffer[lightConfigs[RED_LEFT].bufferIndex] 			& (1 << lightConfigs[RED_LEFT].bitPosition)) != 0;
}

bool GetYellowLeft() {
    return (buffer[lightConfigs[YELLOW_LEFT].bufferIndex] 		& (1 << lightConfigs[YELLOW_LEFT].bitPosition)) != 0;
}

bool GetGreenLeft() {
    return (buffer[lightConfigs[GREEN_LEFT].bufferIndex] 		& (1 << lightConfigs[GREEN_LEFT].bitPosition)) != 0;
}

bool GetRedPedLeft() {
    return (buffer[lightConfigs[RED_PED_LEFT].bufferIndex] 		& (1 << lightConfigs[RED_PED_LEFT].bitPosition)) != 0;
}

bool GetGreenPedLeft() {
    return (buffer[lightConfigs[GREEN_PED_LEFT].bufferIndex] 	& (1 << lightConfigs[GREEN_PED_LEFT].bitPosition)) != 0;
}

bool GetBluePedLeft() {
    return (buffer[lightConfigs[BLUE_PED_LEFT].bufferIndex] 	& (1 << lightConfigs[BLUE_PED_LEFT].bitPosition)) != 0;
}

