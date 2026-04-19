/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32wbxx_hal.h"

#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* struct which can allow for the motor PWM parameters to be passed through without using multiple parameters*/
typedef struct{
    TIM_HandleTypeDef* motor1PWM;
    uint32_t motor1Channel;
    TIM_HandleTypeDef* motor2PWM;
    uint32_t motor2Channel;
} MotorPWMChannels_t;
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
#define EN_LEFT_MOTOR_Pin GPIO_PIN_13
#define EN_LEFT_MOTOR_GPIO_Port GPIOC
#define RIGHT_SERVO_Pin GPIO_PIN_9
#define RIGHT_SERVO_GPIO_Port GPIOB
#define PS2_ATTN_Pin GPIO_PIN_4
#define PS2_ATTN_GPIO_Port GPIOA
#define PS2_CLK_Pin GPIO_PIN_5
#define PS2_CLK_GPIO_Port GPIOA
#define PS2_DATA_Pin GPIO_PIN_6
#define PS2_DATA_GPIO_Port GPIOA
#define PS2_CMD_Pin GPIO_PIN_7
#define PS2_CMD_GPIO_Port GPIOA
#define PWM_LEFT_MOTOR_Pin GPIO_PIN_8
#define PWM_LEFT_MOTOR_GPIO_Port GPIOA
#define LEFT_SERVO_Pin GPIO_PIN_9
#define LEFT_SERVO_GPIO_Port GPIOA
#define DIR_RIGHT_MOTOR_Pin GPIO_PIN_6
#define DIR_RIGHT_MOTOR_GPIO_Port GPIOC
#define PWM_RIGHT_MOTOR_Pin GPIO_PIN_10
#define PWM_RIGHT_MOTOR_GPIO_Port GPIOA
#define DIR_LEFT_MOTOR_Pin GPIO_PIN_15
#define DIR_LEFT_MOTOR_GPIO_Port GPIOA
#define EN_RIGHT_MOTOR_Pin GPIO_PIN_10
#define EN_RIGHT_MOTOR_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* Adding a struct for the motor power. */

/** @brief A struct which is used to store the analogue data received
 * from the controller or by over BLE.
 */
typedef struct{
  float forward_backward;
  float left_right;
} joyStick_values_t;

extern joyStick_values_t joyStickValues; 
extern MotorPWMChannels_t motorPWMChannels;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
