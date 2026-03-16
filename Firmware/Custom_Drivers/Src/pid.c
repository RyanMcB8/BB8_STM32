#include "pid.h"

void PID_Init(PIDController_t *pid,
              float Kp,
              float Ki,
              float Kd,
              float out_min,
              float out_max)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->integral = 0.0f;
    pid->prev_measurement = 0.0f;

    pid->output_min = out_min;
    pid->output_max = out_max;
}

float PID_Update(PIDController_t *pid,
                 float setpoint,
                 float measurement,
                 float dt)
{
    float error = setpoint - measurement;

    /* Integral term */
    pid->integral += error * dt;

    /* Derivative on measurement */
    float derivative = (measurement - pid->prev_measurement) / dt;

    float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    /* Clamp output */
    if(output > pid->output_max)
        output = pid->output_max;

    if(output < pid->output_min)
        output = pid->output_min;

    pid->prev_measurement = measurement;

    return output;
}
