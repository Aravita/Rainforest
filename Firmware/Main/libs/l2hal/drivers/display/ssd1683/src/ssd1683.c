/*
 * ssd1683.c
 *
 *  Created on: Apr 11, 2025
 *      Author: aravita
 */

#include "../include/ssd1683.h"
#include "../include/ssd1683_private.h"

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
)
{
	context->SPIHandle = spiHandle;

	context->BusyPort = busyPort;
	context->BusyPin = busyPin;

	context->ResetPort = resetPort;
	context->ResetPin = resetPin;

	context->DataCommandPort = dataCommandPort;
	context->DataCommandPin = dataCommandPin;

	context->ChipSelectPort = chipSelectPort;
	context->ChipSelectPin = chipSelectPin;

	context->IsDataTransferInProgress = true;

	context->FramebufferDriverContext = framebufferDriverContext;
	context->FramebufferMemoryWriteFunctionPtr = framebufferMemoryWriteFunctionPtr;
	context->FramebufferMemoryReadFunctionPtr = framebufferMemoryReadFunctionPtr;
	context->FramebufferBaseAddress = framebufferBaseAddress;

	L2HAL_SSD1683_ResetDisplay(context);

	L2HAL_SSD1683_WaitForReadiness(context);

	L2HAL_SSD1683_WriteCommand(context, 0x12); /* Software reset */
	HAL_Delay(L2HAL_SSD1683_DISPLAY_BOOT_TIME);
	L2HAL_SSD1683_WaitForReadiness(context);

	uint8_t byteData;

	L2HAL_SSD1683_WriteCommand(context, 0x21); /* Display Update Control */
	byteData = 0x40;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);
	byteData = 0x00;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x01);  /* Set MUX as 300 */
	byteData = 0x2B;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);
	byteData = 0x01;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);
	byteData = 0x00;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x3C); /* Border waveform */
	byteData = 0x01;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x11); /* Data entry mode */
	byteData = 0x03; /* X-mode */
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_SetRange(context, 0, 0, L2HAL_SSD1683_DISPLAY_WIDTH - 1, L2HAL_SSD1683_DISPLAY_HEIGHT - 1);

	L2HAL_SSD1683_WriteCommand(context, 0x18); /* Read built-in temperature sensor */
	byteData = 0x80;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	/* Initial position */
	L2HAL_SSD1683_SetPosition(context, 0, 0);

	/* Power on */
	L2HAL_SSD1683_PowerOn(context);
}

void L2HAL_SSD1683_ResetDisplay(L2HAL_SSD1683_ContextStruct *context)
{
	L2HAL_SSD1683_SelectChip(context, false);
	HAL_Delay(L2HAL_SSD1683_CS_TO_RESET);

	HAL_GPIO_WritePin(context->ResetPort, context->ResetPin, GPIO_PIN_RESET);
	HAL_Delay(L2HAL_SSD1683_RESET_DURATION);

	HAL_GPIO_WritePin(context->ResetPort, context->ResetPin, GPIO_PIN_SET);
	HAL_Delay(L2HAL_SSD1683_DISPLAY_BOOT_TIME);
}

void L2HAL_SSD1683_SelectChip(L2HAL_SSD1683_ContextStruct *context, bool isSelected)
{
	if (isSelected)
	{
		HAL_GPIO_WritePin(context->ChipSelectPort, context->ChipSelectPin, GPIO_PIN_RESET); /* Active low*/
	}
	else
	{
		HAL_GPIO_WritePin(context->ChipSelectPort, context->ChipSelectPin, GPIO_PIN_SET);
	}
}

void L2HAL_SSD1683_WaitForReadiness(L2HAL_SSD1683_ContextStruct *context)
{
	while (GPIO_PIN_SET == HAL_GPIO_ReadPin(context->BusyPort, context->BusyPin)) {}
}

/**
 * Hang till the DMA transfer completed
 */
void L2HAL_SSD1683_WaitForDataTransferCompletion(L2HAL_SSD1683_ContextStruct *context)
{
	while (context->IsDataTransferInProgress) {} /* First wait for DMA completion */
	while (HAL_SPI_GetState(context->SPIHandle) != HAL_SPI_STATE_READY) { } /* Then wait for SPI ready*/
}

/**
 * Mark transfer as completed (call it from DMA IRQ)
 */
void L2HAL_SSD1683_MarkDataTransferAsCompleted(L2HAL_SSD1683_ContextStruct *context)
{
	context->IsDataTransferInProgress = false;
}

/**
 * Write command to display
 */
void L2HAL_SSD1683_WriteCommand(L2HAL_SSD1683_ContextStruct *context, uint8_t command)
{
	context->IsDataTransferInProgress = true;

	HAL_GPIO_WritePin(context->DataCommandPort, context->DataCommandPin, GPIO_PIN_RESET); /* 0 - Command */

	L2HAL_SSD1683_SelectChip(context, true);

	if (HAL_SPI_Transmit_DMA(context->SPIHandle, &command, 1) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	L2HAL_SSD1683_WaitForDataTransferCompletion(context);

	L2HAL_SSD1683_SelectChip(context, false);

	L2HAL_SSD1683_WaitForReadiness(context);
}

/**
 * Write data to display
 */
void L2HAL_SSD1683_WriteData(L2HAL_SSD1683_ContextStruct *context, uint8_t *data, uint16_t dataSize)
{
	context->IsDataTransferInProgress = true;

	HAL_GPIO_WritePin(context->DataCommandPort, context->DataCommandPin, GPIO_PIN_SET); /* 1 - Data */

	L2HAL_SSD1683_SelectChip(context, true);

	if (HAL_SPI_Transmit_DMA(context->SPIHandle, data, dataSize) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	L2HAL_SSD1683_WaitForDataTransferCompletion(context);

	L2HAL_SSD1683_SelectChip(context, false);

	L2HAL_SSD1683_WaitForReadiness(context);
}

void L2HAL_SSD1683_PowerOn(L2HAL_SSD1683_ContextStruct *context)
{
	uint8_t byteData;

	L2HAL_SSD1683_WriteCommand(context, 0x22); /* Display update control */
	byteData = 0xe0;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x20); /* Activate display update sequence */

	L2HAL_SSD1683_WaitForReadiness(context);
}

/**
 * Set output position
 */
void L2HAL_SSD1683_SetPosition(L2HAL_SSD1683_ContextStruct *context, uint16_t x, uint16_t y)
{
	uint8_t byteData;
	L2HAL_SSD1683_WriteCommand(context, 0x4E);
	byteData = x >> 3;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x4F);
	byteData = y & 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);
	byteData = (y >> 8) & 0x01;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);
}

void L2HAL_SSD1683_SetRange(L2HAL_SSD1683_ContextStruct *context, uint16_t xStart,uint16_t yStart,uint16_t xEnd,uint16_t yEnd)
{
	uint8_t byteData;
	L2HAL_SSD1683_WriteCommand(context, 0x44); /* X start - X end */

	byteData = (xStart >> 3) & 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	byteData = (xEnd>>3) & 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x45); /* Y start - Y end */

	byteData = yStart & 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	byteData = (yStart >> 8) & 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	byteData = yEnd & 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	byteData = (yEnd >> 8) & 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);
}

void L2HAL_SSD1683_Update(L2HAL_SSD1683_ContextStruct *context)
{
	uint8_t byteData;

	L2HAL_SSD1683_WriteCommand(context, 0x22);
	byteData = 0xF7;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x20);

	L2HAL_SSD1683_WaitForReadiness(context);
}

/**
 * Update display partially (not spatial, but "weak update")
 */
void L2HAL_SSD1683_WeakUpdate(L2HAL_SSD1683_ContextStruct *context)
{
	uint8_t byteData;

	L2HAL_SSD1683_WriteCommand(context, 0x22);
	byteData = 0xFF;
	L2HAL_SSD1683_WriteData(context, &byteData, 1);

	L2HAL_SSD1683_WriteCommand(context, 0x20);

	L2HAL_SSD1683_WaitForReadiness(context);
}

/**
 * Get display width
 */
uint16_t L2HAL_SSD1683_GetWidth(void)
{
	return L2HAL_SSD1683_DISPLAY_WIDTH;
}

/**
 * Get display height
 */
uint16_t L2HAL_SSD1683_GetHeight(void)
{
	return L2HAL_SSD1683_DISPLAY_HEIGHT;
}

/**
 * Set color what will be used for drawing.
 */
void L2HAL_SSD1683_SetActiveColor(L2HAL_SSD1683_ContextStruct* context, FMGL_API_ColorStruct color)
{
	context->ActiveColor = color;
}

/**
 * If color is not fully black will return true
 */
bool L2HAL_SSD1683_BinarizeColor(FMGL_API_ColorStruct color)
{
	return (0x00 != (color.R | color.G | color.B));
}

/**
 * Set framebuffer base address (in external RAM). Allows to have multiple framebuffers
 */
void L2HAL_SSD1683_SetFramebufferBaseAddress(L2HAL_SSD1683_ContextStruct* context, uint32_t baseAddress)
{
	context->FramebufferBaseAddress = baseAddress;
	L2HAL_SSD1683_PushFramebuffer(context);
	L2HAL_SSD1683_Update(context);
}

/**
 * Fill framebuffer with active color
 */
void L2HAL_SSD1683_ClearFramebuffer(L2HAL_SSD1683_ContextStruct* context)
{
	uint8_t eightPixels = L2HAL_SSD1683_BinarizeColor(context->ActiveColor) ? 0xFF : 0x00;

	uint8_t line[L2HAL_SSD1683_DISPLAY_LINE_SIZE];
	memset(line, eightPixels, L2HAL_SSD1683_DISPLAY_LINE_SIZE);

	for (uint16_t y = 0; y < L2HAL_SSD1683_DISPLAY_HEIGHT; y++)
	{
		context->FramebufferMemoryWriteFunctionPtr
		(
			context->FramebufferDriverContext,
			context->FramebufferBaseAddress + y * L2HAL_SSD1683_DISPLAY_LINE_SIZE,
			L2HAL_SSD1683_DISPLAY_LINE_SIZE,
			line
		);
	}

	L2HAL_SSD1683_PushFramebuffer(context);
}

/**
 * Push framebuffer to display
 */
void L2HAL_SSD1683_PushFramebuffer(L2HAL_SSD1683_ContextStruct* context)
{
	uint8_t line[L2HAL_SSD1683_DISPLAY_LINE_SIZE];
	for (uint16_t y = 0; y < L2HAL_SSD1683_DISPLAY_HEIGHT; y ++)
	{
		context->FramebufferMemoryReadFunctionPtr
		(
			context->FramebufferDriverContext,
			context->FramebufferBaseAddress + y * L2HAL_SSD1683_DISPLAY_LINE_SIZE,
			L2HAL_SSD1683_DISPLAY_LINE_SIZE,
			line
		);

		L2HAL_SSD1683_SetPosition(context, 0, y);

		 /* RED */
		L2HAL_SSD1683_WriteCommand(context, 0x26);
		L2HAL_SSD1683_WriteData(context, line, L2HAL_SSD1683_DISPLAY_LINE_SIZE);

		/* Black */
		L2HAL_SSD1683_WriteCommand(context, 0x24);
		L2HAL_SSD1683_WriteData(context, line, L2HAL_SSD1683_DISPLAY_LINE_SIZE);
	}

	L2HAL_SSD1683_Update(context);
}

/**
 * Draw pixel
 */
void L2HAL_SSD1683_DrawPixel(L2HAL_SSD1683_ContextStruct* context, uint16_t x, uint16_t y)
{
	uint32_t address = context->FramebufferBaseAddress + (y * L2HAL_SSD1683_DISPLAY_LINE_SIZE + (x >> 3));

	uint8_t pixel;

	context->FramebufferMemoryReadFunctionPtr
	(
		context->FramebufferDriverContext,
		address,
		1,
		&pixel
	);

	uint8_t bitNumber = 7 - (x % 8);

	uint8_t mask = (1 << bitNumber);
	uint8_t antimask = ~(1 << bitNumber);
	uint8_t newPixel = L2HAL_SSD1683_BinarizeColor(context->ActiveColor) ? 0xFF : 0x00;

	pixel = (pixel & antimask) | (mask & newPixel);

	context->FramebufferMemoryWriteFunctionPtr
	(
		context->FramebufferDriverContext,
		address,
		1,
		&pixel
	);
}

/**
 * Get pixel color
 */
FMGL_API_ColorStruct L2HAL_SSD1683_GetPixel(L2HAL_SSD1683_ContextStruct* context, uint16_t x, uint16_t y)
{
	uint32_t address = context->FramebufferBaseAddress + (y * L2HAL_SSD1683_DISPLAY_LINE_SIZE + (x >> 3));

	uint8_t pixel;

	context->FramebufferMemoryReadFunctionPtr
	(
		context->FramebufferDriverContext,
		address,
		1,
		&pixel
	);

	uint8_t bitNumber = x % 8;

	FMGL_API_ColorStruct color;
	if (0 != (pixel & (1 << bitNumber)))
	{
		color.R = FMGL_API_MAX_CHANNEL_BRIGHTNESS;
		color.G = FMGL_API_MAX_CHANNEL_BRIGHTNESS;
		color.B = FMGL_API_MAX_CHANNEL_BRIGHTNESS;
	}
	else
	{
		color.R = 0;
		color.G = 0;
		color.B = 0;
	}

	return color;
}

