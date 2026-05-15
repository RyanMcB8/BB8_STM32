/** @file       neckControl.h
 *  @author     Ryan McBride
 *  @brief      A file to declare the necessary functions to control
 *              the motion of the neck by changing the servo angles
 *              based upon a mathematical model. 
 */

/* Prevention of recursion. */
#ifndef _NECK_CONTROL_H_
#define _NECK_CONTROL_H_

/*  Adding the necessary header files. */
#include "main.h"
#include "ServoDriver.h"

/*  Defintions and Macros. */
#define MaxServoAngle   150.0f
#define MinServoAngle    30.0f

/*  Adding the definitions of types. */

/**    @brief  Struct containing the timer handle and channel for each
 *             of the control pins for the left and right servos.
 */
typedef struct{
    TIM_HandleTypeDef *leftTimerHandle;
    TIM_HandleTypeDef *rightTimerHandle;
    uint32_t    leftChannel;
    uint32_t    rightChannel;
    float       minAngle;
    float       maxAngle;
} ServoPWM_t;

/*  Adding declarations of functions. */

/** @brief              A function to move the neck to a specified location based
 *                      upon the pitch and roll angle.
 *  @param attributes   A pointer to the ServoPWM_t instance which stores the timer
 *                      and channel data for each of the 2 servos as well as the 
 *                      maximum angles they can reach.
 *  @param  pitch       The vertical angle of rotation. This is measured in degrees
 *                      and when the neck is vertical, the angle is 0.
 *  @param  roll        The horizontal angle of rotation (rotation around the y axis).
 *                      This is measured in degrees and when the neck is vertical, the
 *                      angle is 0. 
 */
void ChangeAngle(ServoPWM_t *attributes, float pitch, float roll);

/** @brief              A function to map a normalised value to the neck.
 *  @param  attributes  A pointer to the ServoPWM_t instance which stores the timer
 *                      and channel data for each of the 2 servos as well as the 
 *                      maximum angles they can reach.
 *  @param  x           The horizontal normalised input which would move the head from
 *                      left to right.
 *  @param  y           The vertical normalised input which would move the head from
 *                      back to front.
 */
void MapAngle(ServoPWM_t *attributes, float x, float y);

/*  End of file. */
#endif