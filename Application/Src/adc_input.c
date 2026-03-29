//----------------------------------------------------------------------
// Titel	:	adc_input.c
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	14.08.2021
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	AD-Wandler
//----------------------------------------------------------------------

// Einfuegen der standard Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der STM Include-Dateien
//----------------------------------------------------------------------
#include "adc.h"
//----------------------------------------------------------------------

// Einfuegen der eigenen Include Dateien
//----------------------------------------------------------------------
#include "adc_input.h"
//----------------------------------------------------------------------

// ADC einlesen
//----------------------------------------------------------------------
uint16_t readADC (ADC_HandleTypeDef* hadc, uint8_t channel)
{
	// Temporaere Variable anlegen
	uint16_t ADC_Data = 0;

	// ADC1 konfigurieren
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;												// ADC Kanal einstellen
	sConfig.Rank = 1;														// Rang einstellen
	sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;						// ADC Sampletime einstellen
	if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)					// Wenn Config nicht Ok ist
	{
		Error_Handler();													// Fehler HAL ausgeben
	}

	// ADC kalibrieren, damit Ergebnis valide. Zwingend erforderlich
	HAL_ADCEx_Calibration_Start(hadc);										// Mit Kalibrierung bessere Ergebnisse

	// ADC Starten, Wert einlesen und ADC Stoppen
	HAL_ADC_Start(hadc);													// ADC starten
	if(HAL_ADC_PollForConversion(hadc, 100) == HAL_OK)						// Wenn ADC funktioniert
	{
		// ADC Wert holen
		ADC_Data = HAL_ADC_GetValue(hadc);									// ADC Wert Abrufen
	}
	else																	// Falls ADC nicht funktioniert
	{
		Error_Handler();
	}
	HAL_ADC_Stop(hadc);														// ADC stoppen

	//ADC Wert zurueck geben
	return ADC_Data;
}
//----------------------------------------------------------------------
