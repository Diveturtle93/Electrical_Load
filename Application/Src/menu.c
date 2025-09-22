//----------------------------------------------------------------------
// Titel	:	menu.c
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	06.06.2025
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	Elektrische Last
//----------------------------------------------------------------------

// Einfuegen der standard Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der STM Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der eigenen Include Dateien
//----------------------------------------------------------------------
#include "menu.h"
#include "ili9341.h"
//----------------------------------------------------------------------

// Increment
//----------------------------------------------------------------------
static void increment (void)
{

}
//----------------------------------------------------------------------

// Decrement
//----------------------------------------------------------------------
static void decrement (void)
{

}
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Willkommensbildschirm und TFT initialisieren
//----------------------------------------------------------------------
void RenderWelcome (void)
{
	ILI9341_FillScreen(ILI9341_BLUE);

#if (ILI9341_Orientation == 2 || ILI9341_Orientation == 3)
	ILI9341_WriteString(77, 100, "Initialisierung", Font_11x18, ILI9341_BLACK, ILI9341_BLUE);
	ILI9341_FillRectangle(20, 140, 280, 30, ILI9341_DARKGREY);
#elif (ILI9341_Orientation == 1 || ILI9341_Orientation == 4)
	ILI9341_WriteString(37, 100, "Initialisierung", Font_11x18, ILI9341_BLACK, ILI9341_BLUE);
	ILI9341_FillRectangle(20, 140, 200, 30, ILI9341_DARKGREY);
#endif

	for (uint8_t i = 0; i < 20; i++)
	{
#if (ILI9341_Orientation == 2 || ILI9341_Orientation == 3)
		ILI9341_FillRectangle((20 + (14*i)), 140, 14, 30, ILI9341_RED);
#elif (ILI9341_Orientation == 1 || ILI9341_Orientation == 4)
		ILI9341_FillRectangle((20 + (10*i)), 140, 10, 30, ILI9341_RED);
#endif

		HAL_Delay(100);
	}
}
//----------------------------------------------------------------------

// Menu auf Display erstellen
//----------------------------------------------------------------------
void RenderMenu (void)
{
	ILI9341_FillScreen(ILI9341_WHITE);

	for(int x = 0; x < ILI9341_WIDTH; x++)
	{
		ILI9341_DrawPixel(x, 0, ILI9341_RED);
		ILI9341_DrawPixel(x, ILI9341_HEIGHT-1, ILI9341_RED);
	}

	for(int y = 0; y < ILI9341_HEIGHT; y++)
	{
		ILI9341_DrawPixel(0, y, ILI9341_RED);
		ILI9341_DrawPixel(ILI9341_WIDTH-1, y, ILI9341_RED);
	}

	ILI9341_FillRectangle(2, 130, 158, 30, ILI9341_RED);
	ILI9341_FillRectangle(160, 130, 158, 30, ILI9341_YELLOW);
	ILI9341_FillRectangle(2, 160, 158, 30, ILI9341_BLUE);
	ILI9341_FillRectangle(160, 160, 158, 30, ILI9341_GREEN);

	ILI9341_WriteString(25, 40, "Start", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341_WriteString(25, 60, "Protect", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341_WriteString(25, 80, "Max V:100.0V", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341_WriteString(25, 100, "Max A:100.0A", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);

	ILI9341_WriteString(184, 40, "Start", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341_WriteString(184, 60, "Protect", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341_WriteString(184, 80, "Max V:100.0V", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341_WriteString(184, 100, "Max A:100.0A", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
}
//----------------------------------------------------------------------

// Zahl auf Display ausgeben
//----------------------------------------------------------------------
void RenderInt (uint16_t x, uint16_t y, uint16_t number)
{
	char string[5];

	uint8_t tausender = 0, hunderter = 0, zehner = 0, einer = 0;

	tausender = number / 1000;
	hunderter = (number - tausender * 1000) / 100;
	zehner = (number - tausender * 1000 - hunderter * 100) / 10;
	einer = number - tausender * 1000 - hunderter * 100 - zehner * 10;

	string[0] = tausender + 0x30;
	string[1] = hunderter + 0x30;
	string[2] = zehner + 0x30;
	string[3] = einer + 0x30;
	string[4] = '\0';

	ILI9341_WriteString(x, y, string, Font_11x18, ILI9341_BLACK, ILI9341_YELLOW);
}
//----------------------------------------------------------------------

// Zahl mit Nachkommastellen auf Display ausgeben, eine Ziffer hinter Komma
//----------------------------------------------------------------------
void RenderFloat (uint16_t x, uint16_t y, uint16_t number)
{
	char string[6];
	uint8_t hunderter = 0, zehner = 0, einer = 0, nachkomma = 0;

	hunderter = number / 1000;
	zehner = (number - hunderter * 1000) / 100;
	einer = (number - hunderter * 1000 - zehner * 100) / 10;
	nachkomma = number - hunderter * 1000 - zehner * 100 - einer * 10;

	string[0] = hunderter + 0x30;
	string[1] = zehner + 0x30;
	string[2] = einer + 0x30;
	string[3] = '.';
	string[4] = nachkomma + 0x30;
	string[5] = '\0';

	ILI9341_WriteString(x, y, string, Font_11x18, ILI9341_BLACK, ILI9341_YELLOW);
}
//----------------------------------------------------------------------

// Increment und Decrement
//----------------------------------------------------------------------
void drehencoder (uint16_t encoder)
{
	static uint16_t lastencoder;

	if ((encoder > lastencoder) || ((lastencoder == 65535) && (encoder == 0)))
	{
		increment();
	}
	else if ((encoder < lastencoder) || ((lastencoder == 0) && (encoder == 65535)))
	{
		decrement();
	}
	else if (encoder == lastencoder)
	{

	}

	lastencoder = encoder;
}
//----------------------------------------------------------------------
