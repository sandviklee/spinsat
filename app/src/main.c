#include "zephyr/sys/printk.h"
#include <example.h>
#include <zephyr/device.h>

void fetch_and_display(const struct device*){
  struct lsm9ds_data data;
  //Read gyro data
  data.gyro[0] = sensor_sample_fetch(dev,Sensor_CHAN_GYRO_X);
  data.gyro[1] = sensor_sample_fetch(dev,Sensor_CHAN_GYRO_Y);
  data.gyro[2] = sensor_sample_fetch(dev,Sensor_CHAN_GYRO_Z);
  prinf("Gyro X: %d\n", data.gyro[0]);
  prinf("Gyro Y: %d\n", data.gyro[1]);
  prinf("Gyro Z: %d\n", data.gyro[2]);
  //Read accelerometer data
  data.acc[0] = sensor_sample_fetch(dev,Sensor_CHAN_ACCEL_X);
  data.acc[1] = sensor_sample_fetch(dev,Sensor_CHAN_ACCEL_Y);
  data.acc[2] = sensor_sample_fetch(dev,Sensor_CHAN_ACCEL_Z);
  prinf("Accel X: %d\n", data.acc[0]);
  prinf("Accel Y: %d\n", data.acc[1]);
  prinf("Accel Z: %d\n", data.acc[2]);
  //Maybe change mode of something after this

}


int main(int argc, char *argv[]) {
  //Test for IMU
  const struct device *lsm9ds1_dev = DEVICE_DT_GET(DT_NODELABEL(lsm9ds1));
  if (!device_is_ready(lsm9ds1_dev)) {
    printk("Device %s is not ready\n", lsm9ds1_dev->name);
    return -ENODEV;
  }
  printk("Device %s is ready\n", lsm9ds1_dev->name);
  fetch_and_display(lsm9ds1_dev);
  return 0;
}
