/*
 * ssd1683.h
 *
 *  Created on: Apr 11, 2025
 *      Author: aravita
 */

#ifndef DRIVERS_DISPLAY_SSD1683_INCLUDE_SSD1683_H_
#define DRIVERS_DISPLAY_SSD1683_INCLUDE_SSD1683_H_

#include <stdbool.h>
#include "../../../../mcu_dependent/l2hal_mcu.h"
#include "../../../../fmgl/include/fmgl.h"

/**
 * Display sizes
 */
#define L2HAL_SSD1683_DISPLAY_WIDTH 400
#define L2HAL_SSD1683_DISPLAY_HEIGHT 300

#define L2HAL_SSD1683_DISPLAY_LINE_SIZE (L2HAL_SSD1683_DISPLAY_WIDTH / 8)
#define L2HAL_SSD1683_FRAMEBUFFER_SIZE (L2HAL_SSD1683_DISPLAY_LINE_SIZE * L2HAL_SSD1683_DISPLAY_HEIGHT)

/**
 * Display context, SPI connection and various stuff is stored here
 */
typedef struct
{
	/**
	 * SPI bus handle
	 */
	SPI_HandleTypeDef* SPIHandle;

	/**
	 * Busy pin
	 */
	GPIO_TypeDef* BusyPort;
	uint16_t BusyPin;

	/**
	 * Reset pin
	 */
	GPIO_TypeDef* ResetPort;
	uint16_t ResetPin;

	/**
	 * Data / command pin
	 */
	GPIO_TypeDef* DataCommandPort;
	uint16_t DataCommandPin;

	/**
	 * Chip select pin
	 */
	GPIO_TypeDef* ChipSelectPort;
	uint16_t ChipSelectPin;

	/**
	 * Current drawing color. 0x00 - black, 0xFF - white
	 */
	uint8_t BinarizedActiveColor;

	/**
	 * If true, then data transfer in progress and we must wait for next one
	 */
	volatile bool IsDataTransferInProgress;

	/**
	 * Framebuffer driver context pointer (usually it will be RAM-driver context)
	 */
	void* FramebufferDriverContext;

	/**
	 * Pointer to function, writing to framebuffer.
	 * 1st param - pointer to framebuffer driver context (NOT to display driver context)
	 * 2nd param - write start address
	 * 3rd param - write length
	 * 4rd param - pointer to buffer with data to write
	 */
	void (*FramebufferMemoryWriteFunctionPtr)(void*, uint32_t, uint32_t, uint8_t*);

	/**
	 * Pointer to function, reading from framebuffer.
	 * 1st param - pointer to framebuffer driver context (NOT to display driver context)
	 * 2nd param - read start address
	 * 3rd param - read length
	 * 4rd param - pointer to buffer where data will be readed
	 */
	void (*FramebufferMemoryReadFunctionPtr)(void*, uint32_t, uint32_t, uint8_t*);

	/**
	 * Framebuffer base address
	 */
	uint32_t FramebufferBaseAddress;
}
L2HAL_SSD1683_ContextStruct;

/**
 * Initialize SSD1683-based display
 */
void L2HAL_SSD1683_Init
(
	L2HAL_SSD1683_ContextStruct* context,
	SPI_HandleTypeDef *spiHandle,

	GPIO_TypeDef* busyPort,
	uint16_t busyPin,

	GPIO_TypeDef* resetPort,
	uint16_t resetPin,

	GPIO_TypeDef* dataCommandPort,
	uint16_t dataCommandPin,

	GPIO_TypeDef* chipSelectPort,
	uint16_t chipSelectPin,

	void* framebufferDriverContext,

	void (*framebufferMemoryWriteFunctionPtr)(void*, uint32_t, uint32_t, uint8_t*),

	void (*framebufferMemoryReadFunctionPtr)(void*, uint32_t, uint32_t, uint8_t*),

	uint32_t framebufferBaseAddress
);

/**
 * Get display width
 */
uint16_t L2HAL_SSD1683_GetWidth(void);

/**
 * Get display height
 */
uint16_t L2HAL_SSD1683_GetHeight(void);

/**
 * Set color what will be used for drawing.
 */
void L2HAL_SSD1683_SetActiveColor(L2HAL_SSD1683_ContextStruct* context, FMGL_API_ColorStruct color);

/**
 * Set framebuffer base address (in external RAM). Allows to have multiple framebuffers
 */
void L2HAL_SSD1683_SetFramebufferBaseAddress(L2HAL_SSD1683_ContextStruct* context, uint32_t baseAddress);

/**
 * Draw pixel
 */
void L2HAL_SSD1683_DrawPixel(L2HAL_SSD1683_ContextStruct* context, uint16_t x, uint16_t y);

/**
 * Get pixel color
 */
FMGL_API_ColorStruct L2HAL_SSD1683_GetPixel(L2HAL_SSD1683_ContextStruct* context, uint16_t x, uint16_t y);

/**
 * Fill framebuffer with active color
 */
void L2HAL_SSD1683_ClearFramebuffer(L2HAL_SSD1683_ContextStruct* context);

/**
 * Push framebuffer to display
 */
void L2HAL_SSD1683_PushFramebuffer(L2HAL_SSD1683_ContextStruct* context);

/**
 * Mark transfer as completed (call it from DMA IRQ)
 */
void L2HAL_SSD1683_MarkDataTransferAsCompleted(L2HAL_SSD1683_ContextStruct *context);

#endif /* DRIVERS_DISPLAY_SSD1683_INCLUDE_SSD1683_H_ */
