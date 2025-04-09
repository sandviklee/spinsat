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
  STATE_CONNECTING,
  STATE_IDLE,
  STATE_HOLD,
  STATE_SPIN_FORWARD,
  STATE_SPIN_REVERSE,
} state_t;

typedef struct {
  void *data;
  int64_t time_stamp;
} state_machine_ctx_t;

typedef struct {
  state_t last_state;
  state_t current_state;
  event_t event;

  state_machine_ctx_t *ctx;
} state_machine_t;

uint8_t state_machine_init(void);
uint8_t state_machine_handle(void);
state_machine_ctx_t *_state_machine_ctx_init(void *data);

void _state_machine_ctx_destroy(state_machine_t *sm);
void _state_machine_shutdown(void);

#endif // !APP_LIB_STATE_MACHINE_H
