/*
	This file is part of Aravita's STM32 level 2 HAL.

	STM32 level 2 HAL is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	-------------------------------------------------------------------------

	Created by Aravita Cor

	Feel free to contact: aravita_cor@proton.me

	Repository: https://github.com/Aravita/stm32-l2hal

	-------------------------------------------------------------------------
 */

/**
 * @file
 * @biref File for custom hardware-related stuff.
 */
#ifndef L2HAL_INCLUDE_L2HAL_CUSTOM_H_
#define L2HAL_INCLUDE_L2HAL_CUSTOM_H_

#include "../drivers/ram/ly68l6400/include/l2hal_ly68l6400.h"

extern SPI_HandleTypeDef SPI1Handle;
extern DMA_HandleTypeDef SPI1TxDmaHandle;
extern DMA_HandleTypeDef SPI1RxDmaHandle;
extern L2HAL_LY68L6400_ContextStruct RamContext;

/**
 * Put custom hardware initialization stuff here,
 */
void L2HAL_InitCustomHardware(void);

/* SPI-related stuff */
void L2HAL_SetupSPI(void);
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);
void L2HAL_PSRAMDmaCompleted(DMA_HandleTypeDef *hdma); /* Called when transmission via PSRAM SPI is completed */

#endif /* L2HAL_INCLUDE_L2HAL_CUSTOM_H_ */
