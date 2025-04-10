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

#include "../include/l2hal_custom.h"

void L2HAL_InitCustomHardware(void)
{
	L2HAL_SetupSPI();
}

/* Begin SPI-related stuff */

void L2HAL_SetupSPI(void)
{
	/* SPI1 */
	SPI1Handle.Instance = SPI1;
	SPI1Handle.Init.Mode = SPI_MODE_MASTER;
	SPI1Handle.Init.Direction = SPI_DIRECTION_2LINES;
	SPI1Handle.Init.DataSize =  SPI_DATASIZE_8BIT;
	SPI1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	SPI1Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	SPI1Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	SPI1Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPI1Handle.Init.TIMode = SPI_TIMODE_DISABLE;
	SPI1Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SPI1Handle.Init.NSS = SPI_NSS_SOFT;

	if(HAL_SPI_Init(&SPI1Handle) != HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		/**
		 * SPI1 at PA5 (SCK), PA6 (MISO) and PA7 (MOSI)
		 */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_SPI1_CLK_ENABLE();
		__HAL_RCC_DMA2_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_7;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* TX DMA */
		SPI1TxDmaHandle.Instance = DMA2_Stream3;
		SPI1TxDmaHandle.Init.Channel = DMA_CHANNEL_3;
		SPI1TxDmaHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
		SPI1TxDmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
		SPI1TxDmaHandle.Init.MemInc = DMA_MINC_ENABLE;
		SPI1TxDmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		SPI1TxDmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		SPI1TxDmaHandle.Init.Mode = DMA_NORMAL;
		SPI1TxDmaHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		SPI1TxDmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		SPI1TxDmaHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		SPI1TxDmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;
		SPI1TxDmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;

		if (HAL_DMA_Init(&SPI1TxDmaHandle) != HAL_OK)
		{
			L2HAL_Error(Generic);
		}

		__HAL_LINKDMA(hspi, hdmatx, SPI1TxDmaHandle);

		HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

		/* RX DMA */
		SPI1RxDmaHandle.Instance = DMA2_Stream2;
		SPI1RxDmaHandle.Init.Channel = DMA_CHANNEL_3;
		SPI1RxDmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
		SPI1RxDmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
		SPI1RxDmaHandle.Init.MemInc = DMA_MINC_ENABLE;
		SPI1RxDmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		SPI1RxDmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		SPI1RxDmaHandle.Init.Mode = DMA_NORMAL;
		SPI1RxDmaHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		SPI1RxDmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		SPI1RxDmaHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		SPI1RxDmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;
		SPI1RxDmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;

		if (HAL_DMA_Init(&SPI1RxDmaHandle) != HAL_OK)
		{
			L2HAL_Error(Generic);
		}

		__HAL_LINKDMA(hspi, hdmarx, SPI1RxDmaHandle);

		HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

		HAL_DMA_DeInit(&SPI1TxDmaHandle);
		HAL_NVIC_DisableIRQ(DMA2_Stream3_IRQn);

		HAL_DMA_DeInit(&SPI1RxDmaHandle);
		HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
	}
}

void L2HAL_PSRAMDmaCompleted(DMA_HandleTypeDef *hdma)
{
	L2HAL_LY68L6400_MarkDataTransferAsCompleted(&RamContext);
}

