/*
 * global_variables.h
 *
 *  Created on: Apr 10, 2025
 *      Author: aravita
 */

#ifndef INCLUDE_GLOBAL_VARIABLES_H_
#define INCLUDE_GLOBAL_VARIABLES_H_

#include "../libs/l2hal/l2hal_config.h"

/* Put global variables (like contexts) here */

/**
 * SysTick driver context.
 */
L2HAL_SysTick_ContextStruct L2HAL_SysTick_Context = { 0 };

/**
 * SPI1 bus handle.
 */
SPI_HandleTypeDef SPI1Handle = { 0 };

/**
 * SPI1 TX DMA handle.
 */
DMA_HandleTypeDef SPI1TxDmaHandle = { 0 };

/**
 * SPI1 RX DMA handle.
 */
DMA_HandleTypeDef SPI1RxDmaHandle = { 0 };

/**
 * pSRAM context
 */
L2HAL_LY68L6400_ContextStruct RamContext;

#endif /* INCLUDE_GLOBAL_VARIABLES_H_ */
