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
  LOG_INF("Spinsat Starting...  - %s\n", APP_VERSION_STRING);

  state_machine sm;
  state_machine_init(&sm, STATE_INIT);

  for (;;) {
    state_machine_handle(&sm);
    k_sleep(K_MSEC(1000));
  }

  // Cleanup (unreachable in this loop, but good practice)
  imu_deinit(imu);
  return 0;
}
