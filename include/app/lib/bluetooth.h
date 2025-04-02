/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_LIB_BLUETOOTH_H_
#define APP_LIB_BLUETOOTH_H_

#include <app/lib/state_machine.h>
#include <stdint.h>
#include <zephyr/drivers/sensor.h>

#define BT_UUID_SPINSAT_VAL                                                    \
  BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785afeabcd123)

#define BT_UUID_SPINSAT_PUBLISH_VAL                                              \
  BT_UUID_128_ENCODE(0x00001524, 0x1212, 0xefde, 0x1523, 0x785afeabcd124)

#define BT_UUID_SPINSAT_STATE_VAL                                              \
  BT_UUID_128_ENCODE(0x00001525, 0x1212, 0xefde, 0x1523, 0x785afeabcd124)

#define BT_UUID_SPINSAT BT_UUID_DECLARE_128(BT_UUID_SPINSAT_VAL)
#define BT_UUID_SPINSAT_ACCEL BT_UUID_DECLARE_128(BT_UUID_SPINSAT_PUBLISH_VAL)
#define BT_UUID_SPINSAT_STATE BT_UUID_DECLARE_128(BT_UUID_SPINSAT_STATE_VAL)

typedef struct {
  enum state_machine_state state;
  uint32_t value;
} bluetooth_receive;

typedef struct {
  void *data;
  char *name;
  int64_t timestamp;
} bluetooth_payload;

int32_t bluetooth_init(void);
int32_t bluetooth_publish(bluetooth_payload *payload);

#endif /* APP_LIB_BLUETOOTH_H_ */
