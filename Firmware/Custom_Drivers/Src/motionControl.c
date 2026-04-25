/** @file           motionControl.c
 *  @author         Ryan McBride
 *  @details        A file which sets the definitions for functions which may be
 *                  used to control the motion of the droid. This includes the
 *                  translation and rotation of te droid. However, the control
 *                  of the positioning of the droids head is controlled in another
 *                  file.
 */


/* Adding included files */
#include "motionControl.h"
#include "motorDriver.h"

/* Definition of global variables */
droidMotorPowers_t motorPowers;
/* Function defintions */
/* Test functions. */

void Forward( MotorPWMChannels_t motorPWMChannels, float duty){
    MotorControl(duty, clockwise, motorPWMChannels.motor1PWM, motorPWMChannels.motor1Channel, leftMotor);
    MotorControl(duty, anticlockwise, motorPWMChannels.motor2PWM, motorPWMChannels.motor2Channel, rightMotor);
}

void StopDroid( MotorPWMChannels_t motorPWMChannels){
    MotorControl(0.00, 1, motorPWMChannels.motor1PWM, motorPWMChannels.motor1Channel, leftMotor);
    MotorControl(0.00, 2, motorPWMChannels.motor2PWM, motorPWMChannels.motor2Channel, rightMotor);
    return;
}
/*  Main implementation. */

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
    MotorControl(leftPower, leftDirection, motorPWMChannels.motor1PWM, motorPWMChannels.motor1Channel, leftMotor);

    /* Controlling the right motor */
    MotorControl(rightPower, rightDirection, motorPWMChannels.motor2PWM, motorPWMChannels.motor2Channel, rightMotor);
    return MOVE_SUCCESSFUL;
}


void DroidTranslation(float x, float y, float reference_angle) {

    /*  Recentreing the joystick so that the centre is 0.0. */
    float dx = x - 0.5f;
    float dy = y - 0.5f;

    /*  Computing the magnitude and limiting it to 1. */
    float magnitude = fminf(sqrtf(dx*dx + dy*dy), 1.0f);

    /*  Calculating the angle requested by the user. */
    float desired_angle = atan2f(dy, dx);

    /*  Calculating the global angle with respect to the current reference angle. */
    float relative_angle = wrap_angle(desired_angle - reference_angle);

    /*  Determining the relative power each motor should output. */
    float leftRelativePower  = sinf(relative_angle + (M_PI/4.0f));
    float rightRelativePower = sinf(relative_angle - (M_PI/4.0f));

    // Scale by joystick magnitude
    leftRelativePower  *= magnitude;
    rightRelativePower *= magnitude;

    // Convert to power + direction
    if (leftRelativePower >= 0.0f) {
        motorPowers.leftMotorPower.power = leftRelativePower;
        motorPowers.leftMotorPower.direction = clockwise;
    } else {
        motorPowers.leftMotorPower.power = -leftRelativePower;
        motorPowers.leftMotorPower.direction   = anticlockwise;
    }

    if (rightRelativePower >= 0.0f) {
        motorPowers.rightMotorPower.power = rightRelativePower;
        motorPowers.rightMotorPower.direction   = clockwise;
    } else {
        motorPowers.rightMotorPower.power = -rightRelativePower;
        motorPowers.rightMotorPower.direction   = anticlockwise;
    }

    return;
}

float wrap_angle(float angle) {
    angle = fmodf(angle + M_PI, 2.0f * M_PI);
    if (angle < 0.0f) angle += 2.0f * M_PI;   
    return angle - M_PI;                       
}