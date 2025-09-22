/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define POWER_ON_Pin GPIO_PIN_13
#define POWER_ON_GPIO_Port GPIOC
#define OUT3_Pin GPIO_PIN_0
#define OUT3_GPIO_Port GPIOC
#define OUT2_Pin GPIO_PIN_1
#define OUT2_GPIO_Port GPIOC
#define OUT1_Pin GPIO_PIN_2
#define OUT1_GPIO_Port GPIOC
#define OUT0_Pin GPIO_PIN_3
#define OUT0_GPIO_Port GPIOC
#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_1
#define BUTTON2_GPIO_Port GPIOA
#define TEMPERATUR_Pin GPIO_PIN_2
#define TEMPERATUR_GPIO_Port GPIOA
#define TEMPERATUR_PCB_Pin GPIO_PIN_3
#define TEMPERATUR_PCB_GPIO_Port GPIOA
#define KL15_Pin GPIO_PIN_6
#define KL15_GPIO_Port GPIOA
#define DIS_CS_Pin GPIO_PIN_7
#define DIS_CS_GPIO_Port GPIOA
#define DIS_LED_Pin GPIO_PIN_4
#define DIS_LED_GPIO_Port GPIOC
#define DIS_RESET_Pin GPIO_PIN_5
#define DIS_RESET_GPIO_Port GPIOC
#define SPANNUNG_Pin GPIO_PIN_0
#define SPANNUNG_GPIO_Port GPIOB
#define STROM_Pin GPIO_PIN_1
#define STROM_GPIO_Port GPIOB
#define BUTTON3_Pin GPIO_PIN_2
#define BUTTON3_GPIO_Port GPIOB
#define PWM_IN1_Pin GPIO_PIN_6
#define PWM_IN1_GPIO_Port GPIOC
#define PWM_IN2_Pin GPIO_PIN_7
#define PWM_IN2_GPIO_Port GPIOC
#define SD_CS_Pin GPIO_PIN_8
#define SD_CS_GPIO_Port GPIOC
#define TOUCH_IRQ_Pin GPIO_PIN_9
#define TOUCH_IRQ_GPIO_Port GPIOC
#define TOUCH_CS_Pin GPIO_PIN_8
#define TOUCH_CS_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOD
#define IFRG_Pin GPIO_PIN_5
#define IFRG_GPIO_Port GPIOB
#define PWM_OUT1_Pin GPIO_PIN_6
#define PWM_OUT1_GPIO_Port GPIOB
#define PWM_OUT2_Pin GPIO_PIN_7
#define PWM_OUT2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef enum
{
	false,
	true,
} bool;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
