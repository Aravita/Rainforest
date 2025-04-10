/*
 * hal.c
 *
 *  Created on: Apr 10, 2025
 *      Author: aravita
 */

#include "hal.h"

void HAL_IntiHardware(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* Initializing pSRAM pins */
	/* C/S */
	L2HAL_MCU_ClockPortIn(HAL_PSRAM_CS_PORT);
	GPIO_InitStruct.Pin = HAL_PSRAM_CS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(HAL_PSRAM_CS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_PSRAM_CS_PORT, HAL_PSRAM_CS_PIN, GPIO_PIN_SET); /* 1 - Not selected */
}
