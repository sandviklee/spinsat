/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zephyr/sys/printk.h"
#include <app/lib/state_machine.h>
#include <stdint.h>
#include <time.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
// #include <zephyr/sys/poweroff.h>

LOG_MODULE_REGISTER(state_machine_handle, CONFIG_APP_LOG_LEVEL);

state_machine_ctx *_state_machine_ctx_init(void *data) {
  state_machine_ctx *p_ctx =
      (state_machine_ctx *)k_malloc(sizeof(state_machine_ctx));
  if (p_ctx == NULL) {
    LOG_ERR("Failed to allocate memory for state_machine_ctx\n");
  }
  state_machine_ctx ctx = {
      .data = data,
      .time_stamp = k_uptime_get() / 1000,
  };
  p_ctx = &ctx;

  return p_ctx;
}

uint8_t state_machine_ctx_destroy(state_machine *sm) {
  if (sm->ctx != NULL) {
    k_free(sm->ctx);
  }

  return 0;
}

uint8_t state_machine_init(state_machine *sm, state_t s) {
  sm->last_state = CONFIG_STATE_MACHINE_START_STATE;
  sm->current_state = s;
  sm->event = EVENT_OK;
  sm->ctx = _state_machine_ctx_init(NULL);

  if (sm->ctx == NULL) {
    return -1;
  }

  return 0;
}

uint8_t state_machine_handle(state_machine *sm) {
  if (sm->event != EVENT_OK) {
    return sm->event;
  }

  switch (sm->current_state) {
  case STATE_OFF:
    _power_off();
    break;
  case STATE_INIT:
    LOG_INF("State: INIT\n");

    sm->last_state = sm->current_state;
    sm->current_state = STATE_IDLE;
    break;
  case STATE_IDLE:
    if (sm->last_state == STATE_SPIN_LEFT ||
        sm->last_state == STATE_SPIN_RIGHT) {
      motor_reset();
    }
    LOG_INF("State: IDLE\n");
    break;
  case STATE_HOLD:
    if (sm->last_state == STATE_SPIN_LEFT ||
        sm->last_state == STATE_SPIN_RIGHT) {
      motor_reset();
    }
    // TODO: Implement the PID Controller...
    LOG_INF("State: HOLD\n");
    break;
  case STATE_SPIN_LEFT:
    LOG_INF("State: SPIN_LEFT\n");
    motor_spin(sm->ctx->data, LEFT);
    break;
  case STATE_SPIN_RIGHT:
    LOG_INF("State: SPIN_RIGHT\n");
    motor_spin(sm->ctx->data, RIGHT);
    break;
  }

  return 0;
}

void _power_off(void) {
  // TODO: Impl. checks for safe power _power_off
  // - Check if all storage procedures have been finished
  // sys_poweroff();
}
