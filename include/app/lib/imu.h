#ifndef IMU_API_H
#define IMU_API_H

#include <zephyr/drivers/sensor.h>
#include <stdbool.h>
#include <stdint.h>
//Struct to represent 3D data
typedef struct {
    double x;
    double y;
    double z;
} vector3d_t;

typedef struct {
    const struct device *dev;      // Pointer to the IMU device
    vector3d_t gyro_offsets;       // Gyroscope calibration offsets
    vector3d_t accel_offsets;      // Accelerometer calibration offsets
} imu_context_t;


imu_context_t *  imu_init(const struct device *dev, uint32_t gyro_odr, uint32_t accel_odr, uint32_t mag_odr) {

int imu_calibrate_gyro(imu_context_t *ctx, int samples);
int imu_calibrate_accel(imu_context_t *ctx, int samples);

int imu_read_gyro(imu_context_t *ctx, vector3d_t *data);
int imu_read_accel(imu_context_t *ctx, vector3d_t *data);
int imu_read_mag(imu_context_t *ctx, vector3d_t *data);
int imu_read_all(imu_context_t *ctx, vector3d_t *gyro, vector3d_t *accel, vector3d_t *mag);
//Prob not needed but IDK 
int imu_read_temperature(imu_context_t *ctx, double *temp);

int imu_enable_gyro(imu_context_t *ctx, bool enable);
int imu_enable_accel(imu_context_t *ctx, bool enable);
int imu_enable_mag(imu_context_t *ctx, bool enable);

#endif // IMU_API_H
