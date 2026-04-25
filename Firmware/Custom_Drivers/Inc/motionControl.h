/* Header file for controling the motors to move the droid back and forth. */
#ifndef __MOTION_CONTROL_
#define __MOTION_CONTROL_

/* Adding any necessary include files to the header file */
#include "main.h"
#include "motorDriver.h"
/* Creation of any specific types */



/* Error types which may be returned when the Move() function is called upon */
typedef enum{
    MOVE_SUCCESSFUL,            /* Returned when there are no errors */
    leftRightError,             /* Returned when the value of leftRight is not a float between 0 and 1 */
    forwardBackwardError        /* Returned when the value of forwardBackward is not a float between 0 and 1 */
} Move_t;

/* Error types which may be returned when the Spin() function is called. */
typedef enum{
    SPIN_SUCCESSFUL,


} SpinErrors_t;

typedef struct{
    float power;
    motorDirections_t direction;    
} motorPower_t;

typedef struct{
    motorPower_t leftMotorPower;
    motorPower_t rightMotorPower;
} droidMotorPowers_t;


void Forward( MotorPWMChannels_t motorPWMChannels, float duty);

void StopDroid( MotorPWMChannels_t motorPWMChannels);


/* Declaration of any functions within the c file */

/** @brief Function to move the droid around
 * @param leftRight A floating point value between 0 and 1 which
 *  sets the motor speed dependent upon the strength. 0 means the
 *  droid only moves left, 1 means the droid only moves right.
 *  0.5 means both the left and right motor get the same power
 *  supplied to them.
 * @param forwardBackward A floating point value between 0 and 1 which
 *  sets the maximum motor speed for both motors. When the value is 0,
 *  the droid moves backwards, when the value is 1, the droid moves
 *  forwards. When the value is 0.5, the droid is stationary.
 * @param motorPWMChannels A struct containing the timer handles and
 *  channel to control the PWM pins connected to the motors.
 * @retval Returns a value of type Move_t which represents the error
 *  which has occured. 0 represents no error, any other value means
 *  an error has occurred. 
 *  @note This function is built to call upon the motorControl function
 *  which is a low level driver for the SWR4 stepper motor driver   
 */
Move_t Move(float leftRight, float forwardBackward, MotorPWMChannels_t motorPWMChannels);


/** @brief                  A function which can take the normalised values of a joystick (x and y),
 *                          and determine the relative powers to supply to the motor to achieve the
 *                          best solution.
 *  @param  x               The floating point normalised x value of the joystick.
 *  @param  y               The floating point normalised y value of the joystick.
 *  @param  reference_angle The global angle which the droid moves relative to.
 *  @details                The reference angle may be determined using another joystick to
 *                          determine the position the head is looking in or it may be an
 *                          IMU built into a controller than allows the droid to always
 *                          look the same direction as the user. 
 */
void DroidTranslation(float x = 0.5, float y = 0.5, float reference_angle = 0.0);

/** @brief                  A simple function which takes an angle as an argument and maps it
 *                          to remain between 0 and 2*Pi radians.
 *  @param  angle           The floating point angle measured in radians.
 */
float wrap_angle(float angle);

/* Declaration of any variables which may be referenced outside the file */




#endif