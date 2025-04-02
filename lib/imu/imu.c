#include <app/lib/imu.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>


imu_context_t *imu_init(const struct device *dev) {
  imu_context_t *ctx = k_malloc(sizeof(imu_context_t));
  if (ctx == NULL) {
    LOG_ERR("Failed to allocate memory for IMU context");
    return NULL;
  }

  if (!device_is_ready(dev)) {
    LOG_ERR("IMU device %s not ready", dev->name);
    k_free(ctx);
    return NULL;
  }

  ctx->dev = dev;
  ctx->gyro_odr = CONFIG_IMU_GYRO_ODR;
  ctx->accel_odr = CONFIG_IMU_ACCEL_ODR;
  ctx->mag_odr = CONFIG_IMU_MAG_ODR;
  ctx->gyro_offsets = (vector3d_t){0.0, 0.0, 0.0};
  ctx->accel_offsets = (vector3d_t){0.0, 0.0, 0.0};

  struct sensor_value odr_val;
  int ret;

  odr_val.val1 = CONFIG_IMU_GYRO_ODR;
  odr_val.val2 = 0;
  ret = sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ,
                        SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_val);
  if (ret) {
    k_free(ctx);
    return NULL;
  }

  odr_val.val1 = CONFIG_IMU_ACCEL_ODR;
  ret = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ,
                        SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_val);
  if (ret) {
    k_free(ctx);
    return NULL;
  }

  odr_val.val1 = CONFIG_IMU_MAG_ODR;
  ret = sensor_attr_set(dev, SENSOR_CHAN_MAGN_XYZ,
                        SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_val);
  if (ret) {
    k_free(ctx);
    return NULL;
  }

  LOG_INF("IMU initialized with gyro=%u Hz, accel=%u Hz, mag=%u Hz",
          CONFIG_IMU_GYRO_ODR, CONFIG_IMU_ACCEL_ODR, CONFIG_IMU_MAG_ODR);
  return ctx;
}

int imu_calibrate_gyro(imu_context_t *ctx, int samples) {
  if (ctx == NULL || samples <= 0) {
    return -EINVAL;
  }

  vector3d_t sum = {0.0, 0.0, 0.0};
  struct sensor_value val_x, val_y, val_z;

  for (int i = 0; i < CONFIG_IMU_CALIBRATION_SAMPLES; i++) {
    int ret = sensor_sample_fetch_chan(ctx->dev, SENSOR_CHAN_GYRO_XYZ);
    if (ret)
      return ret;

    sensor_channel_get(ctx->dev, SENSOR_CHAN_GYRO_X, &val_x);
    sensor_channel_get(ctx->dev, SENSOR_CHAN_GYRO_Y, &val_y);
    sensor_channel_get(ctx->dev, SENSOR_CHAN_GYRO_Z, &val_z);

    sum.x += sensor_value_to_double(&val_x);
    sum.y += sensor_value_to_double(&val_y);
    sum.z += sensor_value_to_double(&val_z);

    k_sleep(K_MSEC(10));
  }

  ctx->gyro_offsets.x = sum.x / CONFIG_IMU_CALIBRATION_SAMPLES;
  ctx->gyro_offsets.y = sum.y / CONFIG_IMU_CALIBRATION_SAMPLES;
  ctx->gyro_offsets.z = sum.z / CONFIG_IMU_CALIBRATION_SAMPLES;

  return 0;
}
int imu_read_gyro(im recuerdo_context_t *ctx, vector3d_t *data) {
  struct sensor_value val_x, val_y, val_z;
  int ret = sensor_sample_fetch_chan(ctx->dev, SENSOR_CHAN_GYRO_XYZ);
  if (ret)
    return ret;

  sensor_channel_get(ctx->dev, SENSOR_CHAN_GYRO_X, &val_x);
  sensor_channel_get(ctx->dev, SENSOR_CHAN_GYRO_Y, &val_y);
  sensor_channel_get(ctx->dev, SENSOR_CHAN_GYRO_Z, &val_z);

  data->x = sensor_value_to_double(&val_x) - ctx->gyro_offsets.x;
  data->y = sensor_value_to_double(&val_y) - ctx->gyro_offsets.y;
  data->z = sensor_value_to_double(&val_z) - ctx->gyro_offsets.z;
  return 0;
}
