/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zephyr/kernel.h"
#include <app/lib/state_machine.h>
#include <stdint.h>
#include <time.h>

state_machine_ctx *_state_machine_ctx_init(void *data) {
  state_machine_ctx *p_ctx =
      (state_machine_ctx *)k_malloc(sizeof(state_machine_ctx));
  state_machine_ctx ctx = {
      .data = data,
      .time_stamp = time((long *)k_uptime_get()),
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
  sm->old_state = STATE_MACHINE_START_STATE;
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
    if (sm->old_state != STATE_OFF) {
      // Do something
    }
    break;
  case STATE_IDLE:
    if (sm->old_state != STATE_IDLE) {
      // Do something
    }
    break;
  case STATE_HOLD:
    if (sm->old_state != STATE_HOLD) {
      // Do something
    }
    break;
  case STATE_SPIN_LEFT:
    if (sm->old_state != STATE_SPIN_LEFT) {
      // Do something
    }
    break;
  case STATE_SPIN_RIGHT:
    if (sm->old_state != STATE_SPIN_RIGHT) {
      // Do something
    }
    break;
  }

  return 0;
}
