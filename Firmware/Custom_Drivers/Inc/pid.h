#ifndef __PID_H__
#define __PID_H__

typedef struct
{
    float Kp;
    float Ki;
    float Kd;

    float integral;
    float prev_measurement;

    float output_min;
    float output_max;

} PIDController_t;

void PID_Init(PIDController_t *pid,
              float Kp,
              float Ki,
              float Kd,
              float out_min,
              float out_max);

float PID_Update(PIDController_t *pid,
                 float setpoint,
                 float measurement,
                 float dt);

#endif
