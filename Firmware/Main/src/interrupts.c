/*
 * interrupts.c
 *
 *  Created on: Apr 10, 2025
 *      Author: aravita
 */

#include "interrupts.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	L2HAL_SysTick_Callback();
}

/* SPI1 DMA TX complete */
void DMA2_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(SPI1Handle.hdmatx);

	L2HAL_PSRAMDmaCompleted(SPI1Handle.hdmatx);
}

/* SPI1 DMA RX complete */
void DMA2_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(SPI1Handle.hdmarx);

	L2HAL_PSRAMDmaCompleted(SPI1Handle.hdmatx);
}

/* SPI2 DMA TX complete */
void DMA1_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(SPI2Handle.hdmatx);

	L2HAL_DisplayDmaCompleted(SPI2Handle.hdmatx);
}

/* SPI2 DMA RX complete */
void DMA1_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(SPI2Handle.hdmarx);
}
