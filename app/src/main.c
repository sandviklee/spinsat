/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app/lib/state_machine.h"
#include <app_version.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void) {
  LOG_INF("SPINSAT VERSION - %s - STARTING\n", APP_VERSION_STRING);

  int err = state_machine_init();
  if (err) {
    LOG_ERR("Failed to initialize state machine");
    return err;
  }

  for (;;) {
    err = state_machine_handle();
    if (err) {
      LOG_ERR("State machine handle failed: %d", err);
      return err;
    }
    k_sleep(K_MSEC(100));
  }

  return 0;
}
