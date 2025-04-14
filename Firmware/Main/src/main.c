/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "diag/trace.h"
#include "main.h"
#include "global_variables.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via NONE).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
	// At this stage the system clock should have already been configured
	// at high speed.

	/* Mandatory for STM32 HAL */
	HAL_Init();

	/* Our hardware */
	L2HAL_Init();
	HAL_IntiHardware();

	/* RAM initialization */
	L2HAL_LY68L6400_Init
	(
		&RamContext,
		&SPI1Handle,

		HAL_PSRAM_CS_PORT,
		HAL_PSRAM_CS_PIN
	);

	/* Display initialization */
	L2HAL_SSD1683_Init
	(
		&DisplayContext,
		&SPI2Handle,

		HAL_DISPLAY_BUSY_PORT,
		HAL_DISPLAY_BUSY_PIN,

		HAL_DISPLAY_RESET_PORT,
		HAL_DISPLAY_RESET_PIN,

		HAL_DISPLAY_DC_PORT,
		HAL_DISPLAY_DC_PIN,

		HAL_DISPLAY_CS_PORT,
		HAL_DISPLAY_CS_PIN
	);

	/* Hardware self-test */
	HAL_HardwareSelfTest();

	/* FMGL initialization */
	FMGL_API_ColorStruct OffColor;
	OffColor.R = 0xFF;
	OffColor.G = 0xFF;
	OffColor.B = 0xFF;

	FMGL_API_ColorStruct OnColor;
	OnColor.R = 0x00;
	OnColor.G = 0x00;
	OnColor.B = 0x00;

	/* Display clean */
	L2HAL_SSD1683_SetActiveColor(&DisplayContext, OffColor);
	L2HAL_SSD1683_ClearFramebuffer(&DisplayContext);

	/* Attaching FMGL to display */
	FmglContext = FMGL_API_AttachToDriver
	(
		&DisplayContext,
		&L2HAL_SSD1683_GetWidth,
		&L2HAL_SSD1683_GetHeight,
		(void (*) (void* deviceContext, FMGL_API_ColorStruct color))&L2HAL_SSD1683_SetActiveColor,
		(void (*) (void* deviceContext, uint16_t x, uint16_t y))&L2HAL_SSD1683_DrawPixel,
		(FMGL_API_ColorStruct (*) (void* deviceContext, uint16_t x, uint16_t y))&L2HAL_SSD1683_GetPixel,
		(void (*) (void* deviceContext))&L2HAL_SSD1683_PushFramebufferPartial,
		OffColor
	);


	FMGL_API_FontSettings font;
	FMGL_API_Font fontData= FMGL_FontTerminusRegular12Init();
	FMGL_API_XBMTransparencyMode transparencyMode = FMGL_XBMTransparencyModeNormal;

	/* Font settings */
	font.Font = &fontData;
	font.Scale = 1;
	font.CharactersSpacing = 0;
	font.LinesSpacing = 0;
	font.FontColor = &OnColor;
	font.BackgroundColor = &OffColor;
	font.Transparency = &transparencyMode;

	uint16_t width;
	uint16_t height;

	FMGL_API_RenderTextWithLineBreaks(&FmglContext, &font, 0, 0, &width, &height, false, "4.2 \xC4\xC0\xCA\xCD\xCF\xD7\xD9\xCA E-Ink \xC4\xC9\xD3\xD0\xCC\xC5\xCA, \xCB\xC1\xC4\xD2 1");
	FMGL_API_PushFramebuffer(&FmglContext);

	// Saving framebuffer 1
	L2HAL_SSD1683_SaveFramebuffer(&DisplayContext, &RamContext, 0, (void (*)(void *, uint32_t,  uint32_t,  uint8_t *))L2HAL_LY68L6400_MemoryWrite);

	//FMGL_API_SetActiveColor(&FmglContext, OffColor);
	//L2HAL_SSD1683_ClearFramebuffer(&DisplayContext);


	FMGL_API_RenderTextWithLineBreaks(&FmglContext, &font, 0, 0, &width, &height, false, "4.2 \xC4\xC0\xCA\xCD\xCF\xD7\xD9\xCA E-Ink \xC4\xC9\xD3\xD0\xCC\xC5\xCA, \xCB\xC1\xC4\xD2 2");

	// Saving framebuffer 2
	L2HAL_SSD1683_SaveFramebuffer(&DisplayContext, &RamContext, 100000, (void (*)(void *, uint32_t,  uint32_t,  uint8_t *))L2HAL_LY68L6400_MemoryWrite);

	//FMGL_API_DrawRectangleFilled(&FmglContext, 100, 100, 150, 150, OnColor, OnColor);
	//FMGL_API_PushFramebuffer(&FmglContext);

	// Infinite loop
	uint32_t baseAddress = 0;
	while (true)
	{
		// Load framebuffer
		L2HAL_SSD1683_LoadFramebuffer(&DisplayContext, &RamContext, baseAddress, (void (*)(void *, uint32_t,  uint32_t,  uint8_t *))L2HAL_LY68L6400_MemoryRead);
		FMGL_API_PushFramebuffer(&FmglContext);

		if (0 == baseAddress)
		{
			baseAddress = 100000;
		}
		else
		{
			baseAddress = 0;
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
