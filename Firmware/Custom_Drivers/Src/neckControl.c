/** @file       neckControl.c
 *  @author     Ryan McBride
 *  @brief      A file to define the necessary functions to control
 *              the motion of the neck by changing the servo angles
 *              based upon a mathematical model. 
 */



/*  Adding the necessary header files. */
#include "neckControl.h"


/*  Adding definitions of functions. */

void ChangeAngle(ServoPWM_t *attributes, float pitch, float roll){

    /*  Clamping the values. */
    pitch = fminf(fmaxf(pitch, MinServoAngle), MaxServoAngle);
    roll = fminf(fmaxf(roll, MinServoAngle), MaxServoAngle);
  
    /*  Setting the angle of the servos. */
    setAngle(attributes->leftTimerHandle, attributes->leftChannel, pitch);
    setAngle(attributes->rightTimerHandle, attributes->rightChannel, roll);

    return;
}

void MapAngle(ServoPWM_t *attributes, float x, float y){

    /*  Clamping the x and y values to be between 0 and 1. */
    x = fminf(fmaxf(x, 0), 1);
    y = fminf(fmaxf(y, 0), 1);

    /*  Mapping the normalised value to the ideal angle.*/
    float pitch = (x * (MaxServoAngle - MinServoAngle)) + MinServoAngle;
    float roll = (y * (MaxServoAngle - MinServoAngle)) + MinServoAngle;

    ChangeAngle(attributes, pitch, roll);
    return;
}

