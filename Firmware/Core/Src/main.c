/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "ipcc.h"
#include "lptim.h"
#include "usart.h"
#include "rf.h"
#include "rng.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PS2_driver.h"
#include "motionControl.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define minChangeJoy  5u
#define DEADZONE  5u
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*  Storing the live joystick values. */
joyStick_values_t joyStickValues = {0.5, 0.5}; 

/*  Storing the PWM timers and channels for each drive motor.*/
MotorPWMChannels_t motorPWMChannels;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Config code for STM32_WPAN (HSE Tuning must be done before system clock configuration) */
  MX_APPE_Config();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* IPCC initialisation */
  MX_IPCC_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_RNG_Init();
  MX_CRC_Init();
  MX_LPTIM1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM17_Init();
  MX_TIM16_Init();
  MX_I2C1_Init();
  MX_RF_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(1000);

  /** @brief              A function which takes the users controller instance
   *                      and sets the appropriate pins defintions and feedback
   *                      zsettings.
   *  @param  controller  A pointer to the controller instance which the user
   *                      is looking to initialise.
   */
  void controllerInit(PS2ControllerStates_t *controller);

  /*  Defining what timer and channel each motor is connected to.*/
  motorPWMChannels.motor1PWM = &htim2;
  motorPWMChannels.motor1Channel = TIM_CHANNEL_3;
  motorPWMChannels.motor2PWM =  &htim1;
  motorPWMChannels.motor2Channel = TIM_CHANNEL_1;

  /*  Creating the local variables for checking the analogue stick values. */
  uint8_t last_joystickLeftX = 0;
  uint8_t last_joystickLeftY = 0;
  uint8_t last_joystickRight = 0;
  
  uint8_t new_joystickLeftX = 0;
  uint8_t new_joystickLeftY = 0;
  uint8_t new_joystickRight = 0;

  /*  Creating the controller instance and initialising it. */
  PS2ControllerStates_t controller;
  controllerInit(&controller);
  while (0 != config_gamepad(&controller, controller.feedback.en_Pressures, controller.feedback.en_Rumble)){
    HAL_Delay(100);
  }


  /* USER CODE END 2 */

  /* Init code for STM32_WPAN */
  MX_APPE_Init();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_APPE_Process();

    /* USER CODE BEGIN 3 */

    /*  Reading the most recent data from the analogue stick. */
    new_joystickLeftX = Analogue(&controller, PS_LX);
    new_joystickLeftY = Analogue(&controller, PS_LY);

    /*  Checking if the joystick position is in the deadzone. */
    if ((new_joystickLeftX > (0x80 - DEADZONE) && new_joystickLeftX < (0x80 + DEADZONE)) ||
        (new_joystickLeftY > (0x80 - DEADZONE) && new_joystickLeftY < (0x80 + DEADZONE))){
      /*  Stop the droid from moving. */
      DroidTranslation(0.5, 0.5, 0);
    }

    /*  Checking if there has been a large enough change from the last joystick position. */
    else if (minChangeJoy <= fabsf(new_joystickLeftX - last_joystickLeftX) || minChangeJoy <= fabsf(new_joystickLeftY - last_joystickLeftY)){
      /*  Updating the recent joystick value. */
      last_joystickLeftX = new_joystickLeftX;
      last_joystickLeftY = new_joystickLeftY;

      /*  Normalising the joystick controllers to be within a value between 0 and 1 and not 0 to 255. */
      joyStickValues.left_right = ((float)new_joystickLeftX / (float)0xff);
      joyStickValues.forward_backward = ((float)new_joystickLeftY / (float)0xff);

      /*  Updating the droid's motor values. */
      DroidTranslation(joyStickValues.left_right, joyStickValues.forward_backward, 0);
    }

    /*  Adding a short delay to reduce the sampling rate.
        This must be replaced to be thread based/ timer
        based in the future for optimal performance. */
    HAL_Delay(5); // ~200Hz sampling rate.

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
                              |RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_RFWAKEUP;
  PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSE;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/* USER CODE BEGIN 4 */

void controllerInit(PS2ControllerStates_t *controller){
    /*  Setting the pin defintions for the controller. */
  controller->pins.att_GPIO_Pin = PS2_ATTN_Pin;
  controller->pins.att_GPIO_Port = PS2_ATTN_GPIO_Port;
  controller->pins.clk_GPIO_Pin = PS2_CLK_Pin;
  controller->pins.clk_GPIO_Port = PS2_CLK_GPIO_Port;
  controller->pins.cmd_GPIO_Pin = PS2_CMD_Pin;
  controller->pins.cmd_GPIO_Port = PS2_CMD_GPIO_Port;
  controller->pins.dat_GPIO_Pin = PS2_DATA_Pin;
  controller->pins.dat_GPIO_Port = PS2_DATA_GPIO_Port;

  /*  Disabling the feedback. */
  controller->feedback.en_Pressures = false;
  controller->feedback.en_Rumble = false;
  return;
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
