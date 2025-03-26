/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include <app_version.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

static void fetch_and_display(const struct device *dev) {
  struct sensor_value x, y, z;
  static int trig_ctn;

  // lsm9ds1 accel
  sensor_sample_fetch_chan(dev, SENSOR_CHAN_ACCEL_XYZ);
  sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &x);
  sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &y);
  sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &z);

  LOG_INF("accel: x:%f ms/2 y:%f ms/2 z:%f ms/2\n", (double)out_ev(&x),
          (double)out_ev(&y), (double)out_ev(&z));
}

int main(void) {
  const struct device *dev = DEVICE_DT_GET_ANY(st_lsm9ds1);

  if (dev == NULL) {
    LOG_ERR("No LSM9DS1 device found");
    return 0;
  }

  if (!device_is_ready(dev)) {
    LOG_ERR("Device %s is not ready\n", dev->name);
    return 0;
  }

  for (;;) {
    fetch_and_display(dev);
    k_msleep(1000);
  }

  return 0;
}
