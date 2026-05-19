
#ifndef __MOTOR_DRIVER_H_
#define __MOTOR_DRIVER_H_

/* MOTOR DRIVER HEADER FILE */
/* Includes */
#include <math.h>
#include "stm32wbxx_hal.h"

/* ======================================== Typedefs ======================================== */

typedef enum{
    leftMotor,
    rightMotor,
} motors_t;

typedef enum{
    clockwise,
    anticlockwise,
} motorDirections_t;

typedef enum{
    Sucessful,
    DutyCycleOutOfRange,
    NoValidMotorChosen,
} motorErrors_t;

typedef struct{
    TIM_HandleTypeDef* timerHandle;
    uint32_t timerChannel;
    uint16_t        En_GPIO_Pin;
    GPIO_TypeDef*   En_GPIO_Port;
    uint16_t        Dir_GPIO_Pin;
    GPIO_TypeDef*   Dir_GPIO_Port;
    uint32_t        min_freq;
    uint32_t        max_freq;
} Motor_Attributes;

/* ======================================== Functions ======================================== */

/** @brief MotorControl
 * @param power The duty cycle of motor control
 * @param direction The direction in which the motor will rotate 0 for CW, 1 for ACW
 * @note This function takes the timer handle and channel as its parameters to be used for any PWM in use.
 * @return Returns 0 if no error is present, 1 if the power value is out of range.
  */
motorErrors_t MotorControl(Motor_Attributes *attributes, float power, motorDirections_t direction);

/** @brief                  A function which starts the PWM timers and resets the motor drivers.
 *  @param  tim1            A pointer to the timer handle for the left motor.
 *  @param  timer1Channel   The channel which is being used to output the generated signal to the left motor.
 *  @param  tim2            A pointer to the timer handle for the right motor.
 *  @param  timer2Channel   The channel which is being used to output the generated signal to the right motor.
 */
void InitMotor(Motor_Attributes *attributes);

/** @brief                  A function to control the speed of the motor.
 *  @param  power           The relative power mutliplier being applied between 0 and 1. 
 *  @param  timerHandle     A pointer to the timer handle for the motor whose speef is being changed.
 *  @param  timerChannel    The channel number which is being used to output the generated signal.
 *  @retval                 Error code of type motorErrors_t.
 */
motorErrors_t SetMotorSpeed(Motor_Attributes *attributes, float power);

/** @brief                  A function to set the direction of the specified motor.
 *  @param  motor           The motor which is being modified.
 *  @param  direction       The direction which the motor should spin in. 0 is clockwise, 1 is anticlockwise.
 *  @retval                 Error code of type motorErrors_t.
 */
motorErrors_t SetMotorDirection(Motor_Attributes *attributes, motorDirections_t direction);

/** @brief Motor1Reset
 *  @note Basic function to reset the 1st motor driver by toggling the enable pin on then off again.
 */
void MotorReset(Motor_Attributes *attributes);

#endif /* end of __MOTOR_DRIVER_H_*/
