
#ifndef __MOTOR_DRIVER_H_
#define __MOTOR_DRIVER_H_

/* MOTOR DRIVER HEADER FILE */
/* Includes */
#include "main.h"
#include <math.h>

/* ======================================== Typedefs ======================================== */

typedef enum{
    MOTOR_1,
    MOTOR_2,
} motors_t;

typedef enum{
    Sucessful,
    DutyCycleOutOfRange,
    NoValidMotorChosen,
} motorErrors_t;


/* ======================================== Functions ======================================== */

/** @brief MotorControl
 * @param power The duty cycle of motor control
 * @param direction The direction in which the motor will rotate 0 for CW, 1 for ACW
 * @note This function takes the timer handle and channel as its parameters to be used for any PWM in use.
 * @return Returns 0 if no error is present, 1 if the power value is out of range.
  */
motorErrors_t MotorControl(float power, _Bool direction, TIM_HandleTypeDef* timerHandle, uint32_t timerChannel, motors_t motor);

/** @brief                  A function which starts the PWM timers and resets the motor drivers.
 *  @param  tim1            A pointer to the timer handle for the left motor.
 *  @param  timer1Channel   The channel which is being used to output the generated signal to the left motor.
 *  @param  tim2            A pointer to the timer handle for the right motor.
 *  @param  timer2Channel   The channel which is being used to output the generated signal to the right motor.
 */
void InitMotors(TIM_HandleTypeDef *tim1, uint32_t timer1Channel, TIM_HandleTypeDef *tim2, uint32_t timer2Channel);

/** @brief                  A function to control the speed of the motor.
 *  @param  power           The relative power mutliplier being applied between 0 and 1. 
 *  @param  timerHandle     A pointer to the timer handle for the motor whose speef is being changed.
 *  @param  timerChannel    The channel number which is being used to output the generated signal.
 *  @retval                 Error code of type motorErrors_t.
 */
motorErrors_t SetMotorSpeed(float power, TIM_HandleTypeDef *timerHandle, uint32_t timerChannel);

/** @brief                  A function to set the direction of the specified motor.
 *  @param  motor           The motor which is being modified.
 *  @param  direction       The direction which the motor should spin in. 0 is clockwise, 1 is anticlockwise.
 *  @retval                 Error code of type motorErrors_t.
 */
motorErrors_t SetMotorDirection(motors_t motor, GPIO_PinState direction);

/** @brief Motor1Reset
 *  @note Basic function to reset the 1st motor driver by toggling the enable pin on then off again.
 */
void Motor1Reset(void);

/** @brief Motor1Reset
 *  @note Basic function to reset the 2nd motor driver by toggling the enable pin on then off again.
 */
void Motor2Reset(void);

#endif /* end of __MOTOR_DRIVER_H_*/
