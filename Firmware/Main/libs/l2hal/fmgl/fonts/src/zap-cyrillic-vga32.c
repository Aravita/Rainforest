/*
 * zap-cyrillic-vga32.c
 *
 *  Created on: Apr 15, 2025
 *      Author: aravita
 */

#include "../include/zap-cyrillic-vga32.h"

uint16_t FMGL_FontZapCyrillic32GetCharacterWidth (uint8_t character)
{
	return FMGL_FONT_ZAP_CYRILLIC_32_CHARACTER_WIDTH;
}

const uint8_t* FMGL_FontZapCyrillic32GetCharacterRaster(uint8_t character)
{
	if (character < FMGL_FONT_ZAP_CYRILLIC_32_FIRST_CHARACTER_CODE)
	{
		return FMGL_ZapCyrillic32Get_WrongCharacterCode;
	}

	return FMGL_FontZapCyrillic32Characters[character - FMGL_FONT_ZAP_CYRILLIC_32_FIRST_CHARACTER_CODE];
}

FMGL_API_Font FMGL_FontZapCyrillic32Init(void)
{
	FMGL_API_Font font;

	font.Height = FMGL_FONT_ZAP_CYRILLIC_32_CHARACTER_HEIGHT;
	font.GetCharacterWidth = &FMGL_FontZapCyrillic32GetCharacterWidth;
	font.GetCharacterRaster = &FMGL_FontZapCyrillic32GetCharacterRaster;

	return font;
}

