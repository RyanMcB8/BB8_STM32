/* Adding include files */
#include "motorDriver.h"
#include "main.h"
#include "stm32wb55xx.h"
/* Defining functions */

motorErrors_t MotorControl(float power, _Bool direction, TIM_HandleTypeDef* timerHandle, uint32_t timerChannel, motors_t motor){
    /* Checking past values are between 0 and 1. */
    if (0 < power || 1 >= power){
        /* Converting from float into uint16_t for the duty cycle*/
        uint16_t compareRegisterValue = power * (pow(2,16) - 1);

        /* Stopping all movement before making changes */
        HAL_TIM_PWM_Stop(timerHandle, timerChannel);

        /* Setting the new duty cycle */
        __HAL_TIM_SET_COMPARE(timerHandle, timerChannel, compareRegisterValue);

        /* Setting the direction */
        switch(motor){
            case MOTOR_1:
                HAL_GPIO_WritePin(DIR_LEFT_MOTOR_GPIO_Port, DIR_LEFT_MOTOR_Pin, direction);

            case MOTOR_2:
                HAL_GPIO_WritePin(DIR_RIGHT_MOTOR_GPIO_Port, DIR_RIGHT_MOTOR_Pin, direction);

            default:
                return NoValidMotorChosen;
        }
        
        /* Starting the PWM movement again */
        HAL_TIM_PWM_Start(timerHandle, timerChannel);
            
        return Sucessful;
    }
    return DutyCycleOutOfRange;
}

void Motor1Reset(void){
    HAL_GPIO_WritePin(EN_LEFT_MOTOR_GPIO_Port, EN_LEFT_MOTOR_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_LEFT_MOTOR_GPIO_Port, EN_LEFT_MOTOR_Pin, GPIO_PIN_RESET);
}

void Motor2Reset(void){
    HAL_GPIO_WritePin(EN_RIGHT_MOTOR_GPIO_Port, EN_RIGHT_MOTOR_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_RIGHT_MOTOR_GPIO_Port, EN_RIGHT_MOTOR_Pin, GPIO_PIN_RESET);
}