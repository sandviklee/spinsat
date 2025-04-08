/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include <app/lib/bluetooth.h>
#include <app_version.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

void state_machine_write(uint8_t state, uint8_t *data, uint16_t len) {
  LOG_INF("State machine write: %d, len: %d", state, len);
}

int main(void) {
  LOG_INF("Hello World! %s\n", APP_VERSION_STRING);

  callbacks_t *callbacks = callback_init(state_machine_write);

  int8_t err = bluetooth_init(callbacks);
  if (err) {
    LOG_ERR("Bluetooth init failed: %d", err);
    return err;
  }

  return 0;
}
