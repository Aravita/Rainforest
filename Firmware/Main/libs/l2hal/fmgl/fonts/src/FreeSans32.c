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
	FMGL_FontFreeSans32Characters_Greater,
	FMGL_FontFreeSans32Characters_Question,
	FMGL_FontFreeSans32Characters_At,
	FMGL_FontFreeSans32Characters_A,
	FMGL_FontFreeSans32Characters_B,
	FMGL_FontFreeSans32Characters_C,
	FMGL_FontFreeSans32Characters_D,
	FMGL_FontFreeSans32Characters_E,
	FMGL_FontFreeSans32Characters_F,
	FMGL_FontFreeSans32Characters_G,
	FMGL_FontFreeSans32Characters_H,
	FMGL_FontFreeSans32Characters_I,
	FMGL_FontFreeSans32Characters_J,
	FMGL_FontFreeSans32Characters_K,
	FMGL_FontFreeSans32Characters_L,
	FMGL_FontFreeSans32Characters_M,
	FMGL_FontFreeSans32Characters_N,
	FMGL_FontFreeSans32Characters_O,
	FMGL_FontFreeSans32Characters_P,
	FMGL_FontFreeSans32Characters_Q,
	FMGL_FontFreeSans32Characters_R,
	FMGL_FontFreeSans32Characters_S,
	FMGL_FontFreeSans32Characters_T,
	FMGL_FontFreeSans32Characters_U,
	FMGL_FontFreeSans32Characters_V,
	FMGL_FontFreeSans32Characters_W,
	FMGL_FontFreeSans32Characters_X,
	FMGL_FontFreeSans32Characters_Y,
	FMGL_FontFreeSans32Characters_Z,
	FMGL_FontFreeSans32Characters_BracketLeft,
	FMGL_FontFreeSans32Characters_Backslash,
	FMGL_FontFreeSans32Characters_BracketRight,
	FMGL_FontFreeSans32Characters_Circumflex,
	FMGL_FontFreeSans32Characters_LowLine,
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
}

FMGL_API_Font FMGL_FontFreeSans32Init(void)
{
	FMGL_API_Font font;

	font.Height = FMGL_FONT_FREESANS_32_CHARACTER_HEIGHT;
	font.GetCharacterWidth = &FMGL_FontFreeSans32GetCharacterWidth;
	font.GetCharacterRaster = &FMGL_FontFreeSans32GetCharacterRaster;

	return font;
}

