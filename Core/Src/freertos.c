/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TrafficLogic.h"
#include "trafficLights.h"
#include "TrafficInputs.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	TopBottomActive,
	LeftRightActive,
	Transition
}trafficState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

trafficState state = TopBottomActive;
trafficState lastActive = TopBottomActive;
bool topPedWaiting = false;
bool leftPedWaiting = false;

bool transitionActivated = false;

ButtonStates inputStates;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for toggleBlueLight */
osThreadId_t toggleBlueLightHandle;
const osThreadAttr_t toggleBlueLight_attributes = {
  .name = "toggleBlueLight",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UpdateInputOutp */
osThreadId_t UpdateInputOutpHandle;
const osThreadAttr_t UpdateInputOutp_attributes = {
  .name = "UpdateInputOutp",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for updateStateTask */
osThreadId_t updateStateTaskHandle;
const osThreadAttr_t updateStateTask_attributes = {
  .name = "updateStateTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for RedDelay */
osTimerId_t RedDelayHandle;
const osTimerAttr_t RedDelay_attributes = {
  .name = "RedDelay"
};
/* Definitions for YellowDelay */
osTimerId_t YellowDelayHandle;
const osTimerAttr_t YellowDelay_attributes = {
  .name = "YellowDelay"
};
/* Definitions for GreenDelay */
osTimerId_t GreenDelayHandle;
const osTimerAttr_t GreenDelay_attributes = {
  .name = "GreenDelay"
};
/* Definitions for PedDelay */
osTimerId_t PedDelayHandle;
const osTimerAttr_t PedDelay_attributes = {
  .name = "PedDelay"
};
/* Definitions for WalkingDelay */
osTimerId_t WalkingDelayHandle;
const osTimerAttr_t WalkingDelay_attributes = {
  .name = "WalkingDelay"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
bool noCar(void);
bool noCarTopBottom(void);
bool noCarLeftRight(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void BlueLight(void *argument);
void inputOutput(void *argument);
void updateState(void *argument);
void RedDelayCallback(void *argument);
void YellowDelayCallback(void *argument);
void GreenDelayCallback(void *argument);
void PedDelayCallback(void *argument);
void WalkingDelayCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of RedDelay */
  RedDelayHandle = osTimerNew(RedDelayCallback, osTimerOnce, NULL, &RedDelay_attributes);

  /* creation of YellowDelay */
  YellowDelayHandle = osTimerNew(YellowDelayCallback, osTimerOnce, NULL, &YellowDelay_attributes);

  /* creation of GreenDelay */
  GreenDelayHandle = osTimerNew(GreenDelayCallback, osTimerOnce, NULL, &GreenDelay_attributes);

  /* creation of PedDelay */
  PedDelayHandle = osTimerNew(PedDelayCallback, osTimerOnce, NULL, &PedDelay_attributes);

  /* creation of WalkingDelay */
  WalkingDelayHandle = osTimerNew(WalkingDelayCallback, osTimerOnce, NULL, &WalkingDelay_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of toggleBlueLight */
  toggleBlueLightHandle = osThreadNew(BlueLight, NULL, &toggleBlueLight_attributes);

  /* creation of UpdateInputOutp */
  UpdateInputOutpHandle = osThreadNew(inputOutput, NULL, &UpdateInputOutp_attributes);

  /* creation of updateStateTask */
  updateStateTaskHandle = osThreadNew(updateState, NULL, &updateStateTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  activateTopBottom();
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_BlueLight */
/**
* @brief Function implementing the toggleBlueLight thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BlueLight */
void BlueLight(void *argument)
{
  /* USER CODE BEGIN BlueLight */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(TOGGLE_FREQ) ; // ms to ticks
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  if(topPedWaiting){
		  toggleBlueTop();
	  }

	  if(leftPedWaiting){
		  toggleBlueLeft();
	  }
	  vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
  /* USER CODE END BlueLight */
}

/* USER CODE BEGIN Header_inputOutput */
/**
* @brief Function implementing the UpdateInputOutp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_inputOutput */
void inputOutput(void *argument)
{
  /* USER CODE BEGIN inputOutput */

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(20) ; // ms to ticks
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
	  trafficInputs_Update();
	  inputStates = getInputState();
	  updateLights();
	  vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
  /* USER CODE END inputOutput */
}

/* USER CODE BEGIN Header_updateState */
/**
* @brief Function implementing the updateStateTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_updateState */
void updateState(void *argument)
{
  /* USER CODE BEGIN updateState */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(200) ; // ms to ticks
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  switch(state){
	  	  case TopBottomActive :
	  		  if(noCar()){
	  			  if(!osTimerIsRunning(GreenDelayHandle)){
	  				osTimerStart(GreenDelayHandle, pdMS_TO_TICKS(GREEN_DELAY));
	  			  }
	  		  } else {
	  			  osTimerStop(GreenDelayHandle);
	  		  }

	  		  if(inputStates.LeftPed){
	  			  activatePedLeft();
	  			  if(!osTimerIsRunning(WalkingDelayHandle)){
	  				  osTimerStart(WalkingDelayHandle, pdMS_TO_TICKS(WALKING_DELAY));
	  			  }
	  		  }

	  		  if(inputStates.TopPed){
	  			  if(!osTimerIsRunning(PedDelayHandle)){
					  osTimerStart(PedDelayHandle, pdMS_TO_TICKS(PEDESTRIAN_DELAY));
				  }
	  			  topPedWaiting = true;
	  		  }

	  		  if(noCarTopBottom() && !noCarLeftRight() && !osTimerIsRunning(WalkingDelayHandle)){
	  			  state = Transition;
	  			  lastActive = TopBottomActive;
	  		  } else if(!noCarTopBottom() && !noCarLeftRight()){
	  			  if(!osTimerIsRunning(RedDelayHandle)){
	  				  osTimerStart(RedDelayHandle, pdMS_TO_TICKS(RED_DELAY_MAX));
	  			  }
	  		  }
	  		  transitionActivated = false;
	  		  lastActive = TopBottomActive;
	  		  break;
	  	  case LeftRightActive :
	  		  if(noCar()){
	  			  if(!osTimerIsRunning(GreenDelayHandle)){
	  				  osTimerStart(GreenDelayHandle, pdMS_TO_TICKS(GREEN_DELAY));
	  			  }
			  } else {
				  osTimerStop(GreenDelayHandle);
			  }

	  		  if(inputStates.TopPed){
	  			activatePedTop();
				if(!osTimerIsRunning(WalkingDelayHandle)){
					osTimerStart(WalkingDelayHandle, pdMS_TO_TICKS(WALKING_DELAY));
				}
	  		  }

	  		  if(inputStates.LeftPed){
	  			  if(!osTimerIsRunning(PedDelayHandle)){
	  				  osTimerStart(PedDelayHandle, pdMS_TO_TICKS(PEDESTRIAN_DELAY));
	  			  }
				  leftPedWaiting = true;
			  }

	  		  if(noCarLeftRight() && !noCarTopBottom() && !osTimerIsRunning(WalkingDelayHandle)){
				  state = Transition;
				  lastActive = TopBottomActive;
	  		  } else if(!noCarTopBottom() && !noCarLeftRight()){
	  			  if(!osTimerIsRunning(RedDelayHandle)){
	  				  osTimerStart(RedDelayHandle, pdMS_TO_TICKS(RED_DELAY_MAX));
	  			  }
	  		  }
	  		  transitionActivated = false;
	  		  lastActive = LeftRightActive;
	  		  break;
	  	  case Transition :
	  		  if(!osTimerIsRunning(WalkingDelayHandle)){
	  			if(!osTimerIsRunning(YellowDelayHandle)){
	  				osTimerStart(YellowDelayHandle, pdMS_TO_TICKS(YELLOW_DELAY));
				}
	  			if(!transitionActivated){
	  				activateTransition();
	  				transitionActivated = true;
	  			}

	  		  }

	  		  break;
	  }

	  vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
  /* USER CODE END updateState */
}

/* RedDelayCallback function */
void RedDelayCallback(void *argument)
{
  /* USER CODE BEGIN RedDelayCallback */
	osTimerStop(GreenDelayHandle);
	osTimerStop(PedDelayHandle);
	state = Transition;
  /* USER CODE END RedDelayCallback */
}

/* YellowDelayCallback function */
void YellowDelayCallback(void *argument)
{
  /* USER CODE BEGIN YellowDelayCallback */
	osTimerStop(GreenDelayHandle);
	osTimerStop(RedDelayHandle);

	if(lastActive == LeftRightActive){
		activateTopBottom();
		state = TopBottomActive;
		if(leftPedWaiting){
			activatePedLeft();
			leftPedWaiting = false;
			if(!osTimerIsRunning(WalkingDelayHandle) ){
				osTimerStart(WalkingDelayHandle, pdMS_TO_TICKS(WALKING_DELAY));
			}
		}
	} else {
		activateLeftRight();
		state = LeftRightActive;
		if(topPedWaiting){
			activatePedTop();
			topPedWaiting = false;
			if(!osTimerIsRunning(WalkingDelayHandle) ){
				osTimerStart(WalkingDelayHandle, pdMS_TO_TICKS(WALKING_DELAY));
			}
		}
	}
  /* USER CODE END YellowDelayCallback */
}

/* GreenDelayCallback function */
void GreenDelayCallback(void *argument)
{
  /* USER CODE BEGIN GreenDelayCallback */
	osTimerStop(RedDelayHandle);
	osTimerStop(PedDelayHandle);
	state = Transition;
  /* USER CODE END GreenDelayCallback */
}

/* PedDelayCallback function */
void PedDelayCallback(void *argument)
{
  /* USER CODE BEGIN PedDelayCallback */
	osTimerStop(RedDelayHandle);
	osTimerStop(GreenDelayHandle);

	state = Transition;


  /* USER CODE END PedDelayCallback */
}

/* WalkingDelayCallback function */
void WalkingDelayCallback(void *argument)
{
  /* USER CODE BEGIN WalkingDelayCallback */

  /* USER CODE END WalkingDelayCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
bool noCar(void){
	return !(inputStates.BottomCar || inputStates.LeftCar || inputStates.TopCar || inputStates.RightCar);
}

bool noCarLeftRight(void){
	return !(inputStates.LeftCar || inputStates.RightCar);
}

bool noCarTopBottom(void){
	return !(inputStates.BottomCar || inputStates.TopCar);
}
/* USER CODE END Application */

