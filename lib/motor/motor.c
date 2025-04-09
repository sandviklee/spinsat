/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app/lib/motor.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>
#include <stdint.h>
#include <math.h>

LOG_MODULE_REGISTER(motor, CONFIG_APP_LOG_LEVEL);

static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));

static motor_t motor;
static motor_t *motor_ptr;

/**
 * @brief Initialize the motor.
 *
 * This function initializes the motor by setting the initial state and
 * configuring the PWM.
 *
 * @return 0 on success, -1 on failure.
 */
int8_t motor_init(void) {
  if (motor_ptr != NULL) {
    LOG_ERR("Motor already initialized");
    return -1;
  }
  motor_ptr = &motor;
  motor_ptr->pwr = FALSE;
  motor_ptr->dir = NOTHING;

  if (!pwm_is_ready_dt(&servo)) {
    LOG_ERR("Servo PWM is not ready");
    return -1;
  }

  LOG_INF("Servo PWM is ready");

  _motor_turn_on();
  pwm_set_pulse_dt(&servo, PULSE_RESET);

  LOG_INF("Stabilizing servo...");

  k_sleep(K_SECONDS(7));

  LOG_INF("Servo stabilized");

  return 0;
}

/**
 * @brief Spin the motor in a given direction with a specified pulse percentage.
 *
 * This function spins the motor in the specified direction with the given pulse
 * percentage. The pulse percentage is converted to a real pulse value.
 *
 * @param dir The direction to spin the motor (FORWARD or REVERSE).
 * @param pulse_percentage The percentage of the pulse width (0-100).
 * @return 0 on success, -1 on failure.
 */
int8_t motor_spin(direction_t dir, uint8_t pulse_percentage) {
  if (motor_ptr == NULL) {
    LOG_ERR("Motor not initialized");
    return -1;
  }
  motor_ptr->dir = dir;

  int32_t real_pulse = _motor_get_real_pulse(pulse_percentage);

  if (real_pulse < REAL_PULSE_MIN || real_pulse > REAL_PULSE_MAX) {
    LOG_ERR("Invalid pulse value");
    return -1;
  }

  if (dir == FORWARD) {
    pwm_set_pulse_dt(&servo, real_pulse);
  }

  k_sleep(K_MSEC(500));

  return 0;
}

/**
 * @brief Reset the motor.
 *
 * This function resets the motor by setting the direction to NOTHING and
 * configuring the PWM to the reset pulse.
 */
void motor_reset(void) {
  if (motor_ptr == NULL) {
    LOG_ERR("Motor not initialized");
    return;
  }
  motor_ptr->dir = NOTHING;

  pwm_set_pulse_dt(&servo, PULSE_RESET);
  k_sleep(K_SECONDS(1));

  _motor_turn_on();
}

/**
 * @brief Get the real pulse value based on the pulse percentage.
 *
 * This function calculates the real pulse value based on the given pulse
 * percentage. The pulse percentage should be between 0 and 100.
 *
 * @param pulse_percentage The percentage of the pulse width (0-100).
 * @return The real pulse value, or -1 on failure.
 */
int32_t _motor_get_real_pulse(uint8_t pulse_percentage) {
  if (pulse_percentage > 100) {
    LOG_ERR("Invalid pulse percentage");
    return -1;
  }
  int32_t real_pulse = floor(((REAL_PULSE_MAX - REAL_PULSE_MIN) / 100) * pulse_percentage) + REAL_PULSE_MIN;
  return real_pulse;
}

/**
 * @brief Turn on the motor.
 *
 * This function turns on the motor by setting the power state to TRUE.
 */
void _motor_turn_on(void) {
  if (motor_ptr == NULL) {
    LOG_ERR("Motor not initialized");
    return;
  }
  motor_ptr->pwr = TRUE;
  LOG_INF("Motor turned on");
}

/**
 * @brief Turn off the motor.
 *
 * This function turns off the motor by setting the power state to FALSE.
 */
void _motor_turn_off(void) {
  if (motor_ptr == NULL) {
    LOG_ERR("Motor not initialized");
    return;
  }
  motor_ptr->pwr = FALSE;
  LOG_INF("Motor turned off");
}
