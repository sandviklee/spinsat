/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_LIB_BLUETOOTH_H_
#define APP_LIB_BLUETOOTH_H_

#include <stdint.h>
#include <sys/types.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/drivers/sensor.h>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#define BT_UUID_SPINSAT_VAL BT_UUID_128_ENCODE(0x00001545, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

#define BT_UUID_SPINSAT_STATE_VAL                                              \
  BT_UUID_128_ENCODE(0x00001546, 0x1212, 0xefde, 0x1523, 0x785afeabcd124)

#define BT_UUID_SPINSAT BT_UUID_DECLARE_128(BT_UUID_SPINSAT_VAL)
#define BT_UUID_SPINSAT_STATE BT_UUID_DECLARE_128(BT_UUID_SPINSAT_STATE_VAL)

typedef struct {
  void (*connected)(struct bt_conn *conn, uint8_t err);
  void (*disconnected)(struct bt_conn *conn, uint8_t reason);
} bluetooth_connection_t;

typedef struct {
  void (*state_machine_write)(uint8_t state, uint8_t data);
} callbacks_t;

int8_t bluetooth_init(callbacks_t *callbacks);
int8_t bluetooth_deinit(void);
callbacks_t *callback_init(void *state_machine_write);

#ifdef __cplusplus
extern "C" {
#endif

#endif /* APP_LIB_BLUETOOTH_H_ */
