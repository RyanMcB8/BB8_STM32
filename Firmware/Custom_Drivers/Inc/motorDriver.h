
#ifndef __MOTOR_DRIVER_H_
#define __MOTOR_DRIVER_H_

/* MOTOR DRIVER HEADER FILE */
/* Includes */
#include "main.h"


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


/** @brief Motor1Reset
 *  @note Basic function to reset the 1st motor driver by toggling the enable pin on then off again.
 */
void Motor1Reset(void);

/** @brief Motor1Reset
 *  @note Basic function to reset the 2nd motor driver by toggling the enable pin on then off again.
 */
void Motor2Reset(void);

#endif /* end of __MOTOR_DRIVER_H_*/
