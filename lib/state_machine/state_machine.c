/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app/lib/bluetooth.h"
#include "app/lib/motor.h"
#include <app/lib/state_machine.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/poweroff.h>

LOG_MODULE_REGISTER(state_machine_handle, CONFIG_APP_LOG_LEVEL);

/**
 * @brief Context for the state machine.
 *
 * This structure holds the context data for the state machine, including a
 * pointer to the data and a timestamp.
 */
state_machine_ctx_t *_state_machine_ctx_init(void *data) {
  state_machine_ctx_t *p_ctx =
      (state_machine_ctx_t *)k_malloc(sizeof(state_machine_ctx_t));
  if (p_ctx == NULL) {
    LOG_ERR("Failed to allocate memory for state_machine_ctx\n");
  }
  state_machine_ctx_t ctx = {
      .data = data,
      .time_stamp = k_uptime_get() / 1000,
  };
  *p_ctx = ctx;

  return p_ctx;
}

/**
 * @brief Destroy the context of the state machine.
 *
 * This function frees the memory allocated for the state machine context.
 *
 * @param sm Pointer to the state machine instance.
 */
void _state_machine_ctx_destroy(state_machine_t *sm) {
  if (sm->ctx != NULL) {
    k_free(sm->ctx);
  }
}

static state_machine_t sm_i;
static state_machine_t *sm = NULL;

/**
 * @brief Initialize the state machine.
 *
 * This function initializes the state machine by setting the initial state and
 * event.
 *
 * @return 0 on success, -1 on error.
 */
uint8_t state_machine_init() {
  if (sm != NULL) {
    LOG_ERR("State machine already initialized\n");
    return -1;
  }
  sm = &sm_i;
  memset(sm, 0, sizeof(state_machine_t));
  sm->last_state = STATE_OFF;
  sm->current_state = STATE_INIT;
  sm->event = EVENT_OK;
  sm->ctx = _state_machine_ctx_init(NULL);
  if (sm->ctx == NULL) {
    return -1;
  }

  return 0;
}

/**
 * @brief Write function for the state machine.
 *
 * This function is called to write the current state and data to the state
 * machine.
 *
 * @param state The current state of the state machine.
 * @param data The data associated with the current state.
 */
static void state_machine_write(uint8_t state, uint8_t data) {
  LOG_INF("STATE: %02x, DATA: %02x\n", state, data);

  if (sm == NULL) {
    LOG_ERR("State machine instance is NULL\n");
    return;
  }

  sm->last_state = sm->current_state;
  sm->current_state = (state_t)state;
  sm->ctx->data = (void *)&data;
}

#define LED1_NODE DT_ALIAS(led0)
#define LED2_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

/**
 * @brief State machine handle function.
 *
 * This function handles the state machine transitions and actions.
 *
 * @return 0 on success, -1 on error.
 */
uint8_t state_machine_handle() {
  int ret = 0;
  if (sm == NULL) {
    LOG_ERR("State machine is NULL\n");
    return -1;
  }

  if (sm->event != EVENT_OK) {
    return sm->event;
  }

  switch (sm->current_state) {
  case STATE_OFF:
    LOG_INF("SHUTTING DOWN...\n");
    _state_machine_shutdown();

    break;
  case STATE_INIT:
    if (!gpio_is_ready_dt(&led1) || !gpio_is_ready_dt(&led2)) {
      LOG_ERR("GPIO is not ready\n");
      return -1;
    }

    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
      LOG_ERR("Failed to configure LED1 pin (err %d)\n", ret);
      return -1;
    }

    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
      LOG_ERR("Failed to configure LED2 pin (err %d)\n", ret);
      return -1;
    }

    gpio_pin_set_dt(&led1, 1);
    gpio_pin_set_dt(&led2, 0);

    int8_t err;

    err = motor_init();
    if (err) {
      LOG_ERR("Motor init failed (err %d)\n", err);
      return -1;
    }

    callbacks_t *callbacks = callback_init(&state_machine_write);
    err = bluetooth_init(callbacks);
    if (err) {
      LOG_ERR("Bluetooth init failed1 (err %d)\n", err);
      return -1;
    }

    sm->last_state = sm->current_state;
    sm->current_state = STATE_CONNECTING;

    break;
  case STATE_CONNECTING:

    gpio_pin_toggle_dt(&led1);
    gpio_pin_toggle_dt(&led2);

    break;
  case STATE_IDLE:
    if (sm->last_state == STATE_SPIN_REVERSE ||
        sm->last_state == STATE_SPIN_FORWARD) {
        motor_reset();
    }

    gpio_pin_toggle_dt(&led1);
    gpio_pin_set_dt(&led2, 0);

    break;
  case STATE_HOLD:
    if (sm->last_state == STATE_SPIN_REVERSE ||
        sm->last_state == STATE_SPIN_FORWARD) {
        motor_reset();
    }
    // TODO: Implement the PID Controller...

    gpio_pin_set_dt(&led1, 0);
    gpio_pin_set_dt(&led2, 1);
    break;
  case STATE_SPIN_FORWARD:
    gpio_pin_set_dt(&led1, 1);
    gpio_pin_set_dt(&led2, 0);

    err = motor_spin(FORWARD, DEFAULT_MOTOR_SPIN_PERCENTAGE); // TODO: USE DATA INSTEAD OF DEFAULT_MOTOR_SPIN
    if (err) {
      LOG_ERR("Motor spin failed (err %d)\n", err);
      return -1;
    }

    break;
  case STATE_SPIN_REVERSE:
    gpio_pin_set_dt(&led1, 1);
    gpio_pin_set_dt(&led2, 1);

    err = motor_spin(REVERSE, DEFAULT_MOTOR_SPIN_PERCENTAGE); // TODO: USE DATA INSTEAD OF DEFAULT_MOTOR_SPIN
    if (err) {
      LOG_ERR("Motor spin failed (err %d)\n", err);       return -1;
    }

    break;
  }

  return 0;
}

void _state_machine_shutdown(void) {
  gpio_pin_set_dt(&led1, 0);
  gpio_pin_set_dt(&led2, 0);
  _state_machine_ctx_destroy(sm);
  int8_t err = bluetooth_deinit();
  if (err) {
    LOG_ERR("Bluetooth deinit failed (err %d)\n", err);
  }
  sys_poweroff();
}
