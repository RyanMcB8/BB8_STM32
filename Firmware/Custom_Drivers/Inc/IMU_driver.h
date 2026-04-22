/* Header file for the folder which will use the 
LSM6DSO32 library and integrate it with the necessary
hardware specific functions to make it work for this
device. This will also store the function declarations
for the high level functions to complete basic needs
such as reading all sensor data. */

/* Preventing recurssive definitions. */
#ifndef _IMU_DRIVER_H_
#define _IMU_DRIVER_H_

/* Addition of the necessary include header files. */
#include "main.h"
#include "lsm6dso32.h"
#include "lsm6dso32_reg.h"

/* Type definitions. */

/* A struct to hold floating point pitch, yaw and roll
 values. */
typedef struct{
    float pitch;
    float roll;
    float yaw;
} eulerAngles_t;


/* ========== Function  declarations ========== */

/** @brief A function which can read specific addresses in
 *  the LSM6DSO32 IMU.
 *  @param handle A void pointer to the handle of the
 *  communication interface being used.
 *  @param  deviceAddr  The I2C address of the device.
 *  @param reg The value of the register which should be
 *  read from.
 *  @param bufp A pointer to an array which can store the
 *  data being read from the specified register.
 *  @param len The number of bytes which are being read
 *  from the register.
 *  @retval Returns an error value. 0 is successful. 
 */
extern int32_t IMU_read(I2C_HandleTypeDef *handle, uint8_t deviceAddr, 
    uint8_t reg, uint8_t *bufp, uint16_t len);

/** @brief A function which can write to a specific
 *  address in the LSM6DSO32 IMU.
 *  @param handle A void pointer to the handle of the
 *  communication interface being used.
 *  @param  deviceAddr  The I2C address of the device.
 *  @param reg The value of the register which should be
 *  written to.
 *  @param bufp A pointer to an array which stores the data
 *  that should be written to the register.
 *  @param len The number of bytes which are being written
 *  to the register.
 *  @retval Returns an error value. 0 is successful. 
 */
extern int32_t IMU_write(I2C_HandleTypeDef *handle, uint8_t deviceAddr, 
    uint8_t reg, uint8_t *bufp, uint16_t len);
                              
/** @brief A function which may be used by the lower level
 *  driver to add a delay in milliseconds.
 *  @param ms The number of milliseconds for which the delay
 *  should last.
 *  @note This function uses the HAL_Delay function within
 *  it to create a delay which may interrupt other functions.
 */
extern void platform_delay(uint32_t ms);

/** @brief The initialisation function for the IMU.
 *  @param dev_ctx Takes in the instance of the stmdev_ctx_t
 *  struct which is being used which holds the platform
 *  dependent functions for the lower level driver to use.
 *  @note This function is responsible for setting the full
 *  scale range and sampling frequencies for the device. 
 */
extern void IMUInit(const stmdev_ctx_t dev_ctx);

/** @brief A function which shuts down the LSM6DSO32.
 *  @note Currently, this just turns off the power and has
 *  no extra functionality but it may be added in the
 *  future if needed.
 */
extern void IMUDeinit();

/** @brief A function which reads the data from the IMU
 *  and returns it to the user.
 *  @param dev_ctx A pointer to the stmdev_ctx_t instance
 *  being used to store the the platform dependent
 *  functions to allow for the peripheral to be interacted
 *  with.
 *  @param angles A pointer to an instance of the eulerAngles_t
 *  struct which stores the devices current orientation
 *  estimate.
 */
extern void IMUUpdate(const stmdev_ctx_t *dev_ctx, eulerAngles_t *angles);

/** @brief A simple function which can convert from the 
 *  degree unit into radians.
 *  @param angle A floating point value representing the 
 *  angle in degrees which should be converted.
 *  @retval Returns a floating point value of the radian
 *  equivalent value.
 */
extern float_t Deg2rad(float angle);

/* ========== Variable declarations ========== */

/* ========== Definitions ========== */
/* Defining the offset values for this specific IMU. */
#define A_xOffset       0.0f
#define A_yOffset       0.0f
#define A_zOffset       0.0f
#define G_xOffset       0.0f
#define G_yOffset       0.0f
#define G_zOffset       0.0f

#endif  /* End of recurssion prevention. */