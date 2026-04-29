/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM1_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DIR_RIGHT_MOTOR_Pin|DIR_LEFT_MOTOR_Pin|RIGHT_SERVO_Pin|LEFT_SERVO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, PS2_CMD_Pin|EN_RIGHT_MOTOR_Pin|EN_LEFT_MOTOR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PS2_ATTN_Pin|PS2_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DIR_RIGHT_MOTOR_Pin DIR_LEFT_MOTOR_Pin RIGHT_SERVO_Pin LEFT_SERVO_Pin */
  GPIO_InitStruct.Pin = DIR_RIGHT_MOTOR_Pin|DIR_LEFT_MOTOR_Pin|RIGHT_SERVO_Pin|LEFT_SERVO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PS2_CMD_Pin EN_RIGHT_MOTOR_Pin EN_LEFT_MOTOR_Pin */
  GPIO_InitStruct.Pin = PS2_CMD_Pin|EN_RIGHT_MOTOR_Pin|EN_LEFT_MOTOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PS2_DATA_Pin */
  GPIO_InitStruct.Pin = PS2_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PS2_DATA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PS2_ATTN_Pin PS2_CLK_Pin */
  GPIO_InitStruct.Pin = PS2_ATTN_Pin|PS2_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = STEP_RIGHT_MOTOR_Pin;  // PWM output pin for the right motor
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // Alternate function push-pull mode
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;   // Ensure correct Alternate function for TIM1
  HAL_GPIO_Init(STEP_RIGHT_MOTOR_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = STEP_LEFT_MOTOR_Pin;  // PWM output pin for the left motor
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;     // Alternate function push-pull mode
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;  // Ensure correct Alternate function for TIM2
  HAL_GPIO_Init(STEP_LEFT_MOTOR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
