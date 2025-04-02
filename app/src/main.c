/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include <app/lib/imu.h>
#include <app_version.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void) {
  LOG_INF("Hello World! %s\n", APP_VERSION_STRING);
  const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(lsm9ds1));
  vector3d_t gyro_data;

  // Initialize IMU with dynamic allocation
  imu_context_t *imu =
      imu_init(dev); // Gyro: 119 Hz, Accel: 119 Hz, Mag: 10 Hz
  if (imu == NULL) {
    LOG_ERR("IMU initialization failed");
    return -ENOMEM;
  }

  // Calibrate gyroscope
  int ret = imu_calibrate_gyro(imu, 100);
  if (ret) {
    LOG_ERR("Gyro calibration failed: %d", ret);
    imu_deinit(imu);
    return ret;
  }

  // Main loop: Read and print gyro data
  while (1) {
    ret = imu_read_gyro(imu, &gyro_data);
    if (ret) {
      LOG_ERR("Gyro read failed: %d", ret);
    } else {
      printk("Gyro (x, y, z) = (%f, %f, %f)\n", gyro_data.x, gyro_data.y,
             gyro_data.z);
    }
    k_sleep(K_MSEC(500));
  }

  // Cleanup (unreachable in this loop, but good practice)
  imu_deinit(imu);
  return 0;
}
