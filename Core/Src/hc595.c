/*
 * hc595.c
 *
 *  Created on: Nov 23, 2023
 *      Author: gulda
 */

#include "hc595.h"

static SPI_HandleTypeDef *SPI_Handle;

void hc595_init(SPI_HandleTypeDef *_hspi3){
	SPI_Handle = _hspi3;
    HAL_GPIO_WritePin(Reset_595_GPIO_Port, Reset_595_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Enable_595_GPIO_Port, Enable_595_Pin, GPIO_PIN_RESET);
    //HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_RESET);
}


//void hc595_update(uint8_t* dataToSend){
//	HAL_SPI_Transmit(SPI_Handle, dataToSend, 3, 1);
	//HAL_Delay(10);
//	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_SET);
	//HAL_Delay(1);
//	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_RESET);
//}

void hc595_update(uint8_t* dataToSend){
    HAL_SPI_Transmit_DMA(SPI_Handle, dataToSend, 3);
}

// DMA Transfer Complete callback
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == SPI_Handle) {
        HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_RESET);
    }
}


//void hc595_reset(void) {
//	HAL_GPIO_WritePin(Enable_595_GPIO_Port, Enable_595_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(Reset_595_GPIO_Port, Reset_595_Pin, GPIO_PIN_RESET);
//}
