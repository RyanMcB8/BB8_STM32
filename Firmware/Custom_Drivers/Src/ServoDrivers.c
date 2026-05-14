/** @file       ServoDrivers.c
 *  @brief      A file containing the definitions for functions
 *              to control the neck mechanism of the BB8 droid.
 *  @note       This driver requires that the timer used has a low
 *              clock frequency of approximately 1kHz.
 */

 /* Adding the necessary header files to be included. */
 #include "ServoDrivers.h"
 #include <math.h>

 void initServo(TIM_HandleTypeDef* handle, uint32_t channel){
   HAL_TIM_PWM_Start_IT(handle, channel);
   return;
 }

 void deinitServo(TIM_HandleTypeDef* handle, uint32_t channel){
   HAL_TIM_PWM_Stop_IT(handle, channel);
   return;
 }

 void setAngle(TIM_HandleTypeDef* handle, uint32_t channel, float angle){
   /* A pulse width of 1500 µs moves the servo to angle 0. Each 10 µs increase in pulse width
   typically moves the servo 1 degree more clockwise. Each 10 µs decrease in pulse width 
   typically moves the servo 1 degree more anticlockwise. 
   https://raspberrypi.stackexchange.com/questions/108111/what-is-the-relationship-between-angle-and-servo-motor-duty-cycle-how-do-i-impl*/
    
   /* Clamping the angle to be within the correct range. */
   angle = fminf(fmaxf(angle, 0.0f), 180.0f);
   /* Mapping the angle to the duty cycle. */
   /* Angle ranges between 0 and 180 -> 0 to 1. */
   float duty = (angle/180.0f)*1000.0f + 1000.0f;

   /* Applying the newly determined duty cycle. */
   setDuty(handle, channel, duty);
 }

 _Bool setDuty(TIM_HandleTypeDef* handle, uint32_t channel, float duty){
   /* Finding the maximum value which corresponds to 100% duty. */
   uint32_t maxReload = (handle->Instance->ARR);

   /* Finding the value the duty cycle corresponds to relative to the ARR. */
   uint32_t newCCR = (uint32_t)(duty * maxReload);

   /* Clamping the duty to be between 0 and 1. */
   duty = (float) fminf(fmaxf(0.0f, duty), 1.0f);

   /* Applying the new CCR value to the corresponding register.*/
   switch(channel){
      case TIM_CHANNEL_1:
         handle->Instance->CCR1 = newCCR;
         return 0;
      case TIM_CHANNEL_2:
         handle->Instance->CCR2 = newCCR;
         return 0;
      case TIM_CHANNEL_3:
         handle->Instance->CCR3 = newCCR;
         return 0;
      case TIM_CHANNEL_4:
         handle->Instance->CCR4 = newCCR;
         return 0;
      case TIM_CHANNEL_5:
         handle->Instance->CCR5 = newCCR;
         return 0;
      case TIM_CHANNEL_6:
         handle->Instance->CCR6 = newCCR;
         return 0;
      default:
         return 1;
   }
   return 0;
 }

 /* End of file. */
