//----------------------------------------------------------------------
// Titel	:	temperatur.c
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	08.06.2025
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	Temperatur
//----------------------------------------------------------------------

// Einfuegen der standard Include-Dateien
//----------------------------------------------------------------------
#include <math.h>
//----------------------------------------------------------------------

// Einfuegen der STM Include-Dateien
//----------------------------------------------------------------------
#include "main.h"
//----------------------------------------------------------------------

// Einfuegen der eigenen Include Dateien
//----------------------------------------------------------------------
#include "temperatur.h"
//----------------------------------------------------------------------

// STM32F105RCT6 Temperatur zurueckgeben
//----------------------------------------------------------------------
static uint16_t temp_stm32f105 (uint16_t ADCValue)
{
	uint16_t temp = 0;

	temp = ((STM32F105_TEMP_V25 - (float)ADCValue * ADC_VINT / ADC_MAX_VALUE) / STM32F105_TEMP_AVG_SLOPE) + 25.0;

	return temp;
}
//----------------------------------------------------------------------

// NTCS0603E3472FHT Temperatur zurueckgeben
//----------------------------------------------------------------------
static uint16_t temp_ntcs0603e3472fht (uint16_t ADCValue)
{
	float temp = 0, resistor = 0;

	resistor = (((ADC_MAX_VALUE * NTCS0603E3472FHT_R1) - ((ADC_MAX_VALUE - (float)ADCValue) * NTCS0603E3472FHT_R1)) / (ADC_MAX_VALUE - (float)ADCValue));

	temp = (1.0 / ((1.0 / (NTCS0603E3472FHT_T0 + ABS_TEMP_OFF)) + (1.0 / NTCS0603E3472FHT_B * log(resistor / NTCS0603E3472FHT_R0))) - ABS_TEMP_OFF);

	return temp;
}
//----------------------------------------------------------------------

// Temperatur auswerten und zurueckgeben
//----------------------------------------------------------------------
uint16_t temperatur (uint16_t adcvalue, uint8_t sensor)
{
	switch (sensor)
	{
		case STM32F105:
		{
			return temp_stm32f105(adcvalue);
		}

		case NTCS0603E3472FHT:
		{
			return temp_ntcs0603e3472fht(adcvalue);
		}

		default:
		{
			return 255;
		}
	}
}
//----------------------------------------------------------------------
