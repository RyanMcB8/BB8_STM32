/* Adding include files */
#include "motorDriver.h"
#include "main.h"
#include "stm32wb55xx.h"
/* Defining functions */

motorErrors_t MotorControl(float power, motorDirections_t direction, TIM_HandleTypeDef* timerHandle, uint32_t timerChannel, motors_t motor){
    /* Checking past values are between 0 and 1. */
    if (power > 0.0f && power <= 1.0f){

        /* Setting the direction */
        motorErrors_t error = SetMotorDirection(motor, direction);
        if (Sucessful != error) return error;
        
        /*  Setting the speed.  */
        error = SetMotorSpeed(power, timerHandle, timerChannel);
        if (Sucessful != error) return error;
            
        return Sucessful;
    }
    return DutyCycleOutOfRange;
}

void InitMotors(TIM_HandleTypeDef *tim1, uint32_t timer1Channel, TIM_HandleTypeDef *tim2, uint32_t timer2Channel){
    HAL_TIM_PWM_Start(tim1, timer1Channel);  
    HAL_TIM_PWM_Start(tim2, timer2Channel);  
    LeftMotorReset();
    RightMotorReset();
    return;
}

motorErrors_t SetMotorSpeed(float power, TIM_HandleTypeDef *timerHandle, uint32_t timerChannel)
{
    /*  Ensuring that the motor stops if the value passed is approximately 0 or less.*/
    if (power <= 0.000001f)
    {
        HAL_TIM_PWM_Stop(timerHandle, timerChannel);
        return Sucessful;
    }

    /*  Limiting the power gain to be 1. */
    if (power > 1.0f)
        power = 1.0f;

    /*  Setting the speed limits of the motor. */
    uint32_t min_freq = 3000;    // Hz (slowest motor can handle)
    uint32_t max_freq = 90000;   // Hz (fastest motor can handle)

    uint32_t freq = min_freq + (uint32_t)((max_freq - min_freq) * power);

    // Timer clock
    uint32_t timer_clk;
    if (timerHandle->Instance == TIM1 || timerHandle->Instance == TIM16 || timerHandle->Instance == TIM17)
    {
        timer_clk = HAL_RCC_GetPCLK2Freq();
        if ((RCC->CFGR & RCC_CFGR_PPRE2) != 0)
            timer_clk *= 2;
    }
    else
    {
        timer_clk = HAL_RCC_GetPCLK1Freq();
        if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0)
            timer_clk *= 2;
    }

    /*  Finding the timer frequency.    */
    uint32_t timer_freq = timer_clk / (timerHandle->Init.Prescaler + 1);

    /* Finding the clock period.*/
    uint32_t period = (timer_freq / freq) - 1;

    __HAL_TIM_DISABLE(timerHandle);
    __HAL_TIM_SET_AUTORELOAD(timerHandle, period);
    __HAL_TIM_SET_COMPARE(timerHandle, timerChannel, period / 2);
    __HAL_TIM_SET_COUNTER(timerHandle, 0);
    __HAL_TIM_ENABLE(timerHandle);

    HAL_TIM_PWM_Start(timerHandle, timerChannel);
    return Sucessful;
}

motorErrors_t SetMotorDirection(motors_t motor, motorDirections_t direction){
    /* Setting the direction */
    switch(motor){
        case leftMotor:
            HAL_GPIO_WritePin(DIR_LEFT_MOTOR_GPIO_Port, DIR_LEFT_MOTOR_Pin, direction);
            break;

        case rightMotor:
            HAL_GPIO_WritePin(DIR_RIGHT_MOTOR_GPIO_Port, DIR_RIGHT_MOTOR_Pin, direction);
            break;

        default:
            return NoValidMotorChosen;
    }
    return Sucessful;
}

void LeftMotorReset(void){
    HAL_GPIO_WritePin(EN_LEFT_MOTOR_GPIO_Port, EN_LEFT_MOTOR_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(EN_LEFT_MOTOR_GPIO_Port, EN_LEFT_MOTOR_Pin, GPIO_PIN_RESET);
    return;
}

void RightMotorReset(void){
    HAL_GPIO_WritePin(EN_RIGHT_MOTOR_GPIO_Port, EN_RIGHT_MOTOR_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(EN_RIGHT_MOTOR_GPIO_Port, EN_RIGHT_MOTOR_Pin, GPIO_PIN_RESET);
    return;
}