/* Adding include files */
#include "motorDriver.h"
#include "main.h"
#include "stm32wb55xx.h"
/* Defining functions */

motorErrors_t MotorControl(Motor_Attributes *attributes, float power, motorDirections_t direction){
    /* Checking past values are between 0 and 1. */
    if (power > 0.0f && power <= 1.0f){

        /* Setting the direction */
        motorErrors_t error = SetMotorDirection(attributes, direction);
        if (Sucessful != error) return error;
        
        /*  Setting the speed.  */
        error = SetMotorSpeed(attributes, power);
        if (Sucessful != error) return error;
            
        return Sucessful;
    }
    return DutyCycleOutOfRange;
}

void InitMotor(Motor_Attributes *attributes){
    HAL_TIM_PWM_Start(&attributes->timerHandle, attributes->timerChannel)
    MotorReset(attributes);
    return;
}

motorErrors_t SetMotorSpeed(Motor_Attributes *attributes, float power)
{
    /*  Ensuring that the motor stops if the value passed is approximately 0 or less.*/
    if (power <= 0.000001f)
    {
        HAL_TIM_PWM_Stop(&attributes->timerHandle, attributes->timerChannel);
        return Sucessful;
    }

    /*  Limiting the power gain to be 1. */
    if (power > 1.0f)
        power = 1.0f;

    // /*  Setting the speed limits of the motor. */
    // uint32_t min_freq = 3000;    // Hz (slowest motor can handle)
    // uint32_t max_freq = 90000;   // Hz (fastest motor can handle)

    uint32_t freq = attributes->min_freq + (uint32_t)((attributes->max_freq - attributes->min_freq) * power);

    // Timer clock
    uint32_t timer_clk;
    if (attributes->timerHandle->Instance == TIM1 || attributes->timerHandle->Instance == TIM16 || attributes->timerHandle->Instance == TIM17)
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
    uint32_t timer_freq = timer_clk / (attributes->timerHandle->Init.Prescaler + 1);

    /* Finding the clock period.*/
    uint32_t period = (timer_freq / freq) - 1;

    __HAL_TIM_DISABLE(attributes->timerHandle);
    __HAL_TIM_SET_AUTORELOAD(attributes->timerHandle, period);
    __HAL_TIM_SET_COMPARE(attributes->timerHandle, attributes->timerChannel, period / 2);
    __HAL_TIM_SET_COUNTER(attributes->timerHandle, 0);
    __HAL_TIM_ENABLE(attributes->timerHandle);

    HAL_TIM_PWM_Start(attributes->timerHandle, attributes->timerChannel);
    return Sucessful;
}

motorErrors_t SetMotorDirection(Motor_Attributes *attributes, motorDirections_t direction){
    HAL_GPIO_WritePin(attributes->Dir_GPIO_Port, attributes->Dir_GPIO_Pin, direction)
    return Sucessful;
}

void MotorReset(Motor_Attributes *attributes){
    HAL_GPIO_WritePin(attributes->En_GPIO_Port, attributes->En_GPIO_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(attributes->En_GPIO_Port, attributes->En_GPIO_Pin, GPIO_PIN_RESET);
    return;
}