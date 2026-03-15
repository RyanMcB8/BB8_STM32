/* Adding necessary include files. */
#include "main.h"
#include "IMU_driver.h"
#include "madgwickFilter.h"

/* ==================== Function defintions. ==================== */

void IMUInit(stmdev_ctx_t dev_ctx){
  /* Turning IMU on. */
  uint8_t whoAmI, rst;
  HAL_GPIO_WritePin(IMU_PWR_GPIO_Port, IMU_PWR_Pin, GPIO_PIN_SET);

  /* Ensuring that the expected device ID is being returned. 
      - Implicit unit test essentialy*/
  do {
  lsm6dso32_device_id_get(&dev_ctx, &whoAmI);
  } while (whoamI != LSM6DSO32_ID);
  
  /* Restore default configuration */
  lsm6dso32_reset_set(&dev_ctx, PROPERTY_ENABLE);
  do {
    lsm6dso32_reset_get(&dev_ctx, &rst);
  } while (rst);

  /* Disable I3C interface */
  lsm6dso32_i3c_disable_set(&dev_ctx, LSM6DSO32_I3C_DISABLE);

  /* Enable Block Data Update */
  lsm6dso32_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

  /* Set full scale range and sampling frequency. */
  lsm6dso32_xl_full_scale_set(&dev_ctx, LSM6DSO32_4g);
  lsm6dso32_gy_full_scale_set(&dev_ctx, LSM6DSO32_250dps);
  lsm6dso32_xl_data_rate_set(&dev_ctx, LSM6DSO32_XL_ODR_208Hz_ULTRA_LOW_PW);
  lsm6dso32_gy_data_rate_set(&dev_ctx, LSM6DSO32_GY_ODR_208Hz_NORMAL_MD);
}

static void IMUDeinit(){
  HAL_GPIO_WritePin(IMU_PWR_GPIO_Port, IMU_PWR_Pin, GPIO_PIN_RESET);
}

int32_t IMU_write(void *handle, uint8_t reg, uint8_t *bufp,
                              uint16_t len)
{
  HAL_GPIO_WritePin(IMU_6AX_CS_bar_GPIO_Port, IMU_6AX_CS_bar_Pin,
                    GPIO_PIN_RESET);
  HAL_SPI_Transmit(handle, &reg, 1, timeoutDuration);
  HAL_SPI_Transmit(handle, (uint8_t*) bufp, len, timeoutDuration);
  HAL_GPIO_WritePin(IMU_6AX_CS_bar_GPIO_Port, IMU_6AX_CS_bar_Pin, GPIO_PIN_SET);
  return 0;
}

int32_t IMU_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
  reg |= 0x80;
  HAL_GPIO_WritePin(IMU_6AX_CS_bar_GPIO_Port, IMU_6AX_CS_bar_Pin, 
                     GPIO_PIN_RESET);
  if (HAL_SPI_Transmit(handle, &reg, 1, timeoutDuration) != HAL_OK){
    //Error_Handler();
  }
  if (HAL_SPI_Receive(handle, bufp, len, timeoutDuration) != HAL_OK){
    //Error_Handler();
  }
  HAL_GPIO_WritePin(IMU_6AX_CS_bar_GPIO_Port, IMU_6AX_CS_bar_Pin, GPIO_PIN_SET);
  return 0;
}

static void platform_delay(uint32_t ms)
{
  HAL_Delay(ms);
}

static void IMUUpdate(const stmdev_ctx_t *dev_ctx, eulerAngles_t *angles){
  /* Initialising local variables */
  static int16_t data_raw_acceleration[3], data_raw_angular_rate[3];
  static float acceleration_ms2[3], angular_rate_rads[3], normAcc[3];
  lsm6dso32_reg_t reg;
  
  /* Read output only if new data is available. */
  lsm6dso32_status_reg_get(dev_ctx, &reg.status_reg);
  if (reg.status_reg.xlda && reg.status_reg.gda) {

    /* Read acceleration data and convert it into ms2. */
    lsm6dso32_acceleration_raw_get(dev_ctx, data_raw_acceleration);
    acceleration_ms2[0] = lsm6dso32_from_fs4_to_ms2(data_raw_acceleration[0]);
    acceleration_ms2[1] = lsm6dso32_from_fs4_to_ms2(data_raw_acceleration[1]);
    acceleration_ms2[2] = lsm6dso32_from_fs4_to_ms2(data_raw_acceleration[2]);
    
    /* Read angular rate field data and convert into radians. */
    lsm6dso32_angular_rate_raw_get(dev_ctx, data_raw_angular_rate);
    angular_rate_rads[0] =
      lsm6dso32_from_fs250_to_rads(data_raw_angular_rate[0]);
    angular_rate_rads[1] =
      lsm6dso32_from_fs250_to_rads(data_raw_angular_rate[1]);
    angular_rate_rads[2] =
      lsm6dso32_from_fs250_to_rads(data_raw_angular_rate[2]);
    
    /* Removing predetermined offset values for the accelerometer
     and gyroscope. */
    calAcc[0]  = acceleration_ms2[0] - (float) A_xOffset;
    calAcc[1]  = acceleration_ms2[1] - (float) A_yOffset;
    calAcc[2]  = acceleration_ms2[2] - (float) A_zOffset;
    
    calGyro[0] = angular_rate_rads[0] - (float) G_xOffset;
    calGyro[1] = angular_rate_rads[1] - (float) G_yOffset;
    calGyro[2] = angular_rate_rads[2] - (float) G_zOffset;
    
    /* Normalising the accelerometer values before passing them 
    through the Madgwick filter */
    float denom = sqrt((double) calAcc[0] * (double) calAcc[0]
                       + (double) calAcc[1] * (double) calAcc[1]
                       + (double) calAcc[2] * (double) calAcc[2]);
    normAcc[0] = calAcc[0]/denom;
    normAcc[1] = calAcc[1]/denom;
    normAcc[2] = calAcc[2]/denom;
    
    /* Passing the calibrated data into the Madgwick filter */
    imu_filter(normAcc[0], normAcc[1], (normAcc[2]),
               calGyro[0], calGyro[1], 
               calGyro[2]);
    /* q_est is a global variable defined in the Madgwick filter
    library which is constantly being changed by the functions.
    This should be updated in the future to be passed by reference
    instead. */
    eulerAngles(q_est, &angles->roll,
         &angles->pitch, &angles->yaw);
 
  }
}

static float_t Deg2rad(float angle){
  float angleRad = PI*angle/180;
  return angleRad;
}