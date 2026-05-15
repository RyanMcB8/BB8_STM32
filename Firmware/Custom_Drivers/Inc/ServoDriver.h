/** @file       ServoDriver.h
 *  @author     Ryan McBride
 *  @brief      A header file containing the declarations for functions
 *              to control the neck mechanism of the BB8 droid.
 */

 /* Preventing recursive definitions of the file. */
 #ifndef _SERVO_DRIVER_H_
 #define _SERVO_DRIVER_H_

 /* Adding the necessary header files to be included. */
 #include "main.h"

 /* Adding the necessary definitions. */


 /**    @brief          Starting the PWM timer for the selected channel.
  *     @param  handle  A pointer to the timer handle being used to control the servo.
  *     @param  channel An unsigned long integer representing the channel which the PWM 
  *                     signal is being output on
  */
 void initServo(TIM_HandleTypeDef* handle, uint32_t channel);

 /**    @brief          Ending the PWM timer for the selected channel.
  *     @param  handle  A pointer to the timer handle being used to control the servo.
  *     @param  channel An unsigned long integer representing the channel which the PWM 
  *                     signal is being output on
  */
 void deinitServo(TIM_HandleTypeDef* handle, uint32_t channel);

 /* Adding the necesary function declarations. */

 /**    @brief          A function to set the angle of a servo.
  *     @param  handle  A pointer to the timer handle being used to control the servo.
  *     @param  channel An unsigned long integer representing the channel which the PWM 
  *                     signal is being output on.
  */
 void setAngle(TIM_HandleTypeDef* handle, uint32_t channel, float angle);


 /**    @brief          A function to set the duty cycle of the referenced timer and channel.
  *     @param  handle  A pointer to the timer being used.
  *     @param  channel The channel which is outputting the PWM signal.
  *     @param  duty    The floating point representation of the duty cycle normalised to be
  *                     within 0 and 1.
  */
 _Bool setDuty(TIM_HandleTypeDef* handle, uint32_t channel, float duty);

 /* End of file. */
 #endif