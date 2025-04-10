/*
 * hal.h
 *
 *  Created on: Apr 10, 2025
 *      Author: aravita
 */

#ifndef INCLUDE_HAL_H_
#define INCLUDE_HAL_H_

#include "../libs/l2hal/l2hal_config.h"

/**********************
 *  pSRAM - LY68L6400 *
 *
 *  SPI 1
 *  PB4 - CS (Active 0)
 *
 **********************/
#define HAL_PSRAM_CS_PIN GPIO_PIN_4
#define HAL_PSRAM_CS_PORT GPIOB

/**
 * Init project-specific hardware here
 */
void HAL_IntiHardware(void);



#endif /* INCLUDE_HAL_H_ */
