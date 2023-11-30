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

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

ButtonStates inputState;

ButtonStates tempInputState;

TrafficCrossingAction *TrafficAction;

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
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for updateStateTask */
osThreadId_t updateStateTaskHandle;
const osThreadAttr_t updateStateTask_attributes = {
  .name = "updateStateTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Delay */
osTimerId_t DelayHandle;
const osTimerAttr_t Delay_attributes = {
  .name = "Delay"
};
/* Definitions for Semaphore */
osSemaphoreId_t SemaphoreHandle;
const osSemaphoreAttr_t Semaphore_attributes = {
  .name = "Semaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void BlueLight(void *argument);
void inputOutput(void *argument);
void updateState(void *argument);
void DelayCallback(void *argument);

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

  /* Create the semaphores(s) */
  /* creation of Semaphore */
  SemaphoreHandle = osSemaphoreNew(1, 1, &Semaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Delay */
  DelayHandle = osTimerNew(DelayCallback, osTimerOnce, NULL, &Delay_attributes);

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
  //initTrafficLogic(PedOneLane);
  //initTrafficLogic(NoPedTwoLane);
  initTrafficLogic(PedTwoLane);
  TrafficAction = getTrafficAction();

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
	  if(TrafficAction->LeftPedWaiting){
		  toggleBlueLeft();
	  } else {
		  ControlLight(BLUE_PED_LEFT, OFF);
	  }
	  if(TrafficAction->TopPedWaiting){
		  toggleBlueTop();
	  } else {
		  ControlLight(BLUE_PED_TOP, OFF);
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
	  if(osSemaphoreAcquire(SemaphoreHandle, 0)){
		  tempInputState = getInputState();
		  if(tempInputState.BottomCar){
			  inputState.BottomCar = true;
		  }
		  if(tempInputState.LeftCar){
			  inputState.LeftCar = true;
		  }
		  if(tempInputState.LeftPed){
			  inputState.LeftPed = true;
		  }
		  if(tempInputState.RightCar){
			  inputState.RightCar = true;
		  }
		  if(tempInputState.TopCar){
			  inputState.TopCar = true;
		  }
		  if(tempInputState.TopPed){
			  inputState.TopPed = true;
		  }
		  osSemaphoreRelease(SemaphoreHandle);
	  }
	  trafficInputs_Update();

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
	  TrafficCrossing(inputState);
	  if(TrafficAction->StartTimerForNextState){
		  osTimerStart(DelayHandle, pdMS_TO_TICKS(TrafficAction->KeepStateFor));
		  TrafficAction->StartTimerForNextState = false;
	  }
	  if(osSemaphoreAcquire(SemaphoreHandle, 0)){
		  inputState.BottomCar = false;
		  inputState.LeftCar = false;
		  inputState.LeftPed = false;
		  inputState.RightCar = false;
		  inputState.TopCar = false;
		  inputState.TopPed = false;

		  osSemaphoreRelease(SemaphoreHandle);
	  }
	  vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
  /* USER CODE END updateState */
}

/* DelayCallback function */
void DelayCallback(void *argument)
{
  /* USER CODE BEGIN DelayCallback */
	ActivateNextState();
  /* USER CODE END DelayCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

