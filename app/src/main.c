/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include <app/lib/bluetooth.h>
#include <app_version.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void) {
  LOG_INF("Hello World! %s\n", APP_VERSION_STRING);

  return 0;
}
