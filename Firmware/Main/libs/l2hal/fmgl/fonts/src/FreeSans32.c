/*
 * zap-cyrillic-vga32.c
 *
 *  Created on: Apr 15, 2025
 *      Author: aravita
 */

#include "../include/FreeSans32.h"

static const uint8_t* FMGL_FontFreeSans32Characters[FMGL_FONT_FREESANS_32_CHARACTERS_TABLE_LENGTH] =
{
	FMGL_FontFreeSans32Characters_Space,
	FMGL_FontFreeSans32Characters_Exclam,
	FMGL_FontFreeSans32Characters_DoubleQuote,
	FMGL_FontFreeSans32Characters_Numbersign,
	FMGL_FontFreeSans32Characters_Dollar,
	FMGL_FontFreeSans32Characters_Percent,
	FMGL_FontFreeSans32Characters_Ampersand,
	FMGL_FontFreeSans32Characters_Apostrophe,
	FMGL_FontFreeSans32Characters_ParenthesisLeft,
	FMGL_FontFreeSans32Characters_ParenthesisRight,
	FMGL_FontFreeSans32Characters_Asterisk,
	FMGL_FontFreeSans32Characters_Plus,
	FMGL_FontFreeSans32Characters_Comma,
	FMGL_FontFreeSans32Characters_Minus,
	FMGL_FontFreeSans32Characters_Period,
	FMGL_FontFreeSans32Characters_Slash,
	FMGL_FontFreeSans32Characters_Zero,
	FMGL_FontFreeSans32Characters_One,
	FMGL_FontFreeSans32Characters_Two,
	FMGL_FontFreeSans32Characters_Three,
	FMGL_FontFreeSans32Characters_Four,
	FMGL_FontFreeSans32Characters_Five,
	FMGL_FontFreeSans32Characters_Six,
	FMGL_FontFreeSans32Characters_Seven,
	FMGL_FontFreeSans32Characters_Eight,
	FMGL_FontFreeSans32Characters_Nine,
	FMGL_FontFreeSans32Characters_Colon,
	FMGL_FontFreeSans32Characters_Semicolon,
	FMGL_FontFreeSans32Characters_Less,
	FMGL_FontFreeSans32Characters_Equal,
};

uint16_t FMGL_FontFreeSans32GetCharacterWidth (uint8_t character)
{
	if ((character < FMGL_FONT_FREESANS_32_FIRST_CHARACTER_CODE) || (character >= FMGL_FONT_FREESANS_32_FIRST_CHARACTER_CODE + FMGL_FONT_FREESANS_32_CHARACTERS_TABLE_LENGTH))
	{
		return FMGL_FONT_FREESANS_32_WRONG_CHARACTER_WIDTH;
	}

	return FMGL_FontFreeSans32CharactersWidths[character - FMGL_FONT_FREESANS_32_FIRST_CHARACTER_CODE];
}

const uint8_t* FMGL_FontFreeSans32GetCharacterRaster(uint8_t character)
{
	if ((character < FMGL_FONT_FREESANS_32_FIRST_CHARACTER_CODE) || (character >= FMGL_FONT_FREESANS_32_FIRST_CHARACTER_CODE + FMGL_FONT_FREESANS_32_CHARACTERS_TABLE_LENGTH))
	{
		return FMGL_FontFreeSans32CharactersWrong;
	}

	return FMGL_FontFreeSans32Characters[character - FMGL_FONT_FREESANS_32_FIRST_CHARACTER_CODE];

	/*switch (character)
	{
		case 0x20: return FMGL_FontFreeSans32Characters_Space;
		case 0x21: return FMGL_FontFreeSans32Characters_Exclam;
		case 0x22: return FMGL_FontFreeSans32Characters_DoubleQuote;
		case 0x23: return FMGL_FontFreeSans32Characters_Numbersign;
		case 0x24: return FMGL_FontFreeSans32Characters_Dollar;

		default: return FMGL_FontFreeSans32CharactersWrong;
	}*/
}

FMGL_API_Font FMGL_FontFreeSans32Init(void)
{
	FMGL_API_Font font;

	font.Height = FMGL_FONT_FREESANS_32_CHARACTER_HEIGHT;
	font.GetCharacterWidth = &FMGL_FontFreeSans32GetCharacterWidth;
	font.GetCharacterRaster = &FMGL_FontFreeSans32GetCharacterRaster;

	return font;
}

