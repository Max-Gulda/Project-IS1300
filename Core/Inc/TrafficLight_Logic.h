/*
 * TrafficLight_Logic.h
 *
 *  Created on: Nov 27, 2023
 *      Author: gulda
 */

#ifndef INC_TRAFFICLIGHT_LOGIC_H_
#define INC_TRAFFICLIGHT_LOGIC_H_

#include "stdbool.h"

typedef enum{
	LeftRightActive,
	TopBottomActive,
	Transition
}TrafficState;

/*
 * The blue light blink with this frequency to indicate a pedestrian wants to cross.
 */
#define TOGGLE_FREQ 500


/*
 * The signal for pedestrians stays green at least this amount of time.
 */
#define WALKING_DELAY 6000;


/*
 * The signal for pedestrians turn green at least after this amount of time.
 */
#define PEDESTRIAN_DELAY 12000


/*
 * The yellow light is active for this amount of time.
 */
#define YELLOW_DELAY 1500


/*
 * If there is no active cars in either direction, the other lane is turned green after this amount of time.
 */
#define GREEN_DELAY 8000


/*
 * If a car arrives at a red light and there are cars in the other lane, this car has to wait at maximum red_delay_max amount of time
 */
#define RED_DELAY_MAX 10000


bool pedestrianTopWaiting = false;

bool pedestrianLeftWaiting = false;

TrafficState state = TopBottomActive;



#endif /* INC_TRAFFICLIGHT_LOGIC_H_ */
