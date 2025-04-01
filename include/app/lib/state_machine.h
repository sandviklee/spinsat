/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_LIB_STATE_MACHINE_H
#define APP_LIB_STATE_MACHINE_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  EVENT_OK = 0x00,
  EVENT_IGNORED = 0xFE,
  CANNOT_HAPPEN = 0xFF
} event_t;

typedef enum {
  STATE_OFF,
  STATE_INIT,
  STATE_IDLE,
  STATE_HOLD,
  STATE_SPIN_LEFT,
  STATE_SPIN_RIGHT,
} state_t;

typedef struct {
  void *data;
  int64_t time_stamp;
} state_machine_ctx;

typedef struct {
  state_t last_state;
  state_t current_state;
  event_t event;

  state_machine_ctx *ctx;
} state_machine;

uint8_t state_machine_init(state_machine *sm, state_t s);
uint8_t state_machine_handle(state_machine *sm);
uint8_t state_machine_ctx_destroy(state_machine *sm);
state_machine_ctx *_state_machine_ctx_init(void *data);
void _power_off(void);

#endif // !APP_LIB_STATE_MACHINE_H
