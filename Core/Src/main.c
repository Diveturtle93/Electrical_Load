/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dac.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc_input.h"
#include "ili9341.h"
#include "menu.h"
#include "millis.h"
#include "my_math.h"
#include "temperatur.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// IMD Variablen fuer Timer
static volatile uint16_t rising = 0, falling = 0;
static volatile uint8_t pwm_change = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	// Timer Variablen
	uint32_t timer3Periode = 0;
	uint16_t frequency = 0, rpm = 0, encoder = 0;
	uint32_t lasttimeloop = 0;

	// AD-Wandler Variablen
	double spannung = 0, strom = 0;
	uint16_t vint = 0, stm_temp = 0, pcb_temp = 0, vcc = 0;
	uint16_t ADC_Wert = 0, adc_spannung = 0, adc_strom = 0;

	// Min. Luefter PWM fuer Anlauf
	uint16_t count = 10000;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_CAN1_Init();
  MX_DAC_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

	// DAC auf null setzen
	DAC1->DHR12R1 = 0;
	DAC1->DHR12R2 = 0;

	// Timer Starten
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

	// DAC Starten
//	HAL_DAC_Start(&hdac, DAC1_CHANNEL_1);
//	HAL_DAC_Start(&hdac, DAC1_CHANNEL_2);

	// ADC kalibrieren, damit Ergebnis valide. Zwingend erforderlich
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADCEx_Calibration_Start(&hadc2);

	// Display einschalten und initialisieren
	ILI9341_LedON();
	ILI9341_Unselect();
	ILI9341_Init();

	// Willkommen
	RenderWelcome();

	// Freigabe loeschen
	HAL_GPIO_WritePin(IFRG_GPIO_Port, IFRG_Pin, GPIO_PIN_RESET);

	// Timer 3 fuer Luefter PWM-Eingang aktivieren
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK);
	if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1) != HAL_OK);
	if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2) != HAL_OK);
	timer3Periode = (HAL_RCC_GetPCLK2Freq() / htim3.Init.Prescaler);

	// Menu Ausgeben
	RenderMenu();

	ILI9341_WriteString(40, 134, " 120.4V", Font_16x26, ILI9341_BLACK, ILI9341_RED);
	ILI9341_WriteString(200, 134, " 120.4A", Font_16x26, ILI9341_BLACK, ILI9341_YELLOW);
	ILI9341_WriteString(40, 164, " 120.4W", Font_16x26, ILI9341_BLACK, ILI9341_BLUE);
	ILI9341_WriteString(5, 220, "24,6 C", Font_11x18, ILI9341_BLACK, ILI9341_GREEN);
	ILI9341_WriteString(85, 220, "25,4 C", Font_11x18, ILI9341_BLACK, ILI9341_GREEN);
	ILI9341_WriteString(225, 220, "     RPM", Font_11x18, ILI9341_BLACK, ILI9341_YELLOW);

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

	// Min. Luefter PWM fuer Betrieb
//	count = 5000;
	count = 0;
	TIM4->CCR1 = count;
	HAL_Delay(500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  adc_spannung = readADC(ADC_CHANNEL_8);
	  adc_strom = readADC(ADC_CHANNEL_9);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (adc_strom >= 100)
		  DAC->DHR12R1 = 0;

	  if (millis() > (lasttimeloop + 1000))
	  {
		  encoder = __HAL_TIM_GET_COUNTER(&htim5);
		  RenderInt(225, 10, encoder);
		  drehencoder(encoder);

		  // Wenn sich der Pegel am Luefter Tacho aendert
		  if (pwm_change == 1)
		  {
			  if (rising != 0 && falling != 0)
			  {
				  // Berechne Frequenzy von Luefter-Tacho, Pro Umdrehung 2 Pulse, daher mal 30 um U/min
				  frequency = timer3Periode / rising;
				  rpm = frequency * 30;
			  }

			  pwm_change = 0;
			  RenderInt(225, 220, rpm);
		  }

		  ADC_Wert = readADC(ADC_CHANNEL_VREFINT);
		  vint = ADC_Wert * ADC_VREF / ADC_MAX_VALUE;
		  vcc = vint * ADC_MAX_VALUE / ADC_Wert;
		  RenderInt(85, 10, vint);
		  RenderInt(165, 10, vcc);

		  ADC_Wert = readADC(ADC_CHANNEL_TEMPSENSOR);
		  stm_temp = temperatur(ADC_Wert, STM32F105);
		  RenderInt(165, 195, stm_temp);

		  ADC_Wert = readADC(ADC_CHANNEL_9);
		  strom = ((((ADC_Wert / ADC_MAX_VALUE) * (VOLTAGE_MAX / 100.0)) / (1.0 + (CURRENT_R1 / CURRENT_R2))) / (CURRENT_SHUNT / 100.0));
		  RenderFloat(85, 195, strom);

		  ADC_Wert = readADC(ADC_CHANNEL_3);
		  pcb_temp = temperatur(ADC_Wert, NTCS0603E3472FHT);
		  RenderInt(245, 195, pcb_temp);

		  HAL_Delay(1000);
		  ADC_Wert = readADC(ADC_CHANNEL_8);
//		  ADC_old = (ADC_old + ((ADC_Wert- ADC_old) / 10));
//		  spannung = ((((float)ADC_Wert / ADC_MAX_VALUE) * ((VOLTAGE_PRERESISTOR + VOLTAGE_R0) / VOLTAGE_R0)) * (VOLTAGE_MAX / 100.0));
		  spannung = ((((float)ADC_Wert * (float)vcc) / ADC_MAX_VALUE)/100 * 45/27);
		  spannung = (spannung * 462/22);
		  RenderFloat(5, 195, spannung);
		  RenderInt(5, 10, ADC_Wert);

		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

		  lasttimeloop = millis();
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_ENABLE();
}

/* USER CODE BEGIN 4 */
// Timer Interrupt
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// Timer 3 fuer Luefter PWM-Auswertung
	if (htim == &htim3)
	{
		pwm_change = 1;
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			rising = calculateMovingAverage(rising, HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1), 10);
		}
		else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			falling = calculateMovingAverage(falling, HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2), 10);
		}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
