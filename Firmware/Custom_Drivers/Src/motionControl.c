
#ifndef _MOTION_CONTROL_H_
#define _MOTION_CONTROL_H_

/* Adding included files */
#include "motionControl.h"
#include "motorDriver.h"

/* Definition of global variables */

/* Function defintions */


void Forward( MotorPWMChannels_t motorPWMChannels, float duty){
    MotorControl(duty, 1, motorPWMChannels.motor1PWM, motorPWMChannels.motor1Channel, MOTOR_1);
    MotorControl(duty, 2, motorPWMChannels.motor2PWM, motorPWMChannels.motor2Channel, MOTOR_2);
}

void StopDroid( MotorPWMChannels_t motorPWMChannels){
    MotorControl(0.00, 1, motorPWMChannels.motor1PWM, motorPWMChannels.motor1Channel, MOTOR_1);
    MotorControl(0.00, 2, motorPWMChannels.motor2PWM, motorPWMChannels.motor2Channel, MOTOR_2);
    return;
}

Move_t Move(float leftRight, float forwardBackward, MotorPWMChannels_t motorPWMChannels){

    // The leftRight and forwardBackward values should be transmitted via a normalisation between 0 and 1.
    if (leftRight > 1.0 || leftRight < 0.0){
        return leftRightError;
    }
    if (forwardBackward > 1.0 || forwardBackward < 0.0){
        return forwardBackwardError;
    }

    /* If the code reaches this point, no error has occurred from the 
       user entering the values to control motor */

    /* Assumming that the droid will not spin...*/
    float rightPower = fmax(2*(leftRight - 0.5), 0);     /* max function ensures that when the value is */ 
    float leftPower = fmax(2*(0.5 - leftRight), 0);     /* max function ensures that when the value is */

    /* Using this method does not allow for the droid to spin too. 
       That may be done in a separate function when the droid is stationary. */
    _Bool leftDirection = round(forwardBackward + 0.5);
    _Bool rightDirection = round(forwardBackward + 0.5);
    

    /* Controlling the left motor */
    MotorControl(leftPower, leftDirection, motorPWMChannels.motor1PWM, motorPWMChannels.motor1Channel, MOTOR_1);

    /* Controlling the right motor */
    MotorControl(rightPower, rightDirection, motorPWMChannels.motor2PWM, motorPWMChannels.motor2Channel, MOTOR_2);
    return MOVE_SUCCESSFUL;
}

SpinErrors_t Spin(float power, _Bool direction, MotorPWMChannels_t motorPWMChannels){
    MotorControl(power, direction, motorPWMChannels.motor1PWM, motorPWMChannels.motor1Channel, MOTOR_1);
    MotorControl(power, !direction, motorPWMChannels.motor2PWM, motorPWMChannels.motor2Channel, MOTOR_2);


    return SPIN_SUCCESSFUL;
}

#endif