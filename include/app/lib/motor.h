/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_LIB_MOTOR_H
#define APP_LIB_MOTOR_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Check the simulations folder in root for a description of these magic
 * numbers.
 */

#define REAL_PULSE_MIN 1070
#define REAL_PULSE_MAX 1300
#define PULSE_RESET 1000

#define DEFAULT_MOTOR_SPIN_PERCENTAGE 60

typedef enum { NOTHING, FORWARD, REVERSE } direction_t;

typedef enum { FALSE, TRUE } bool_t;

typedef struct {
  bool_t pwr;
  direction_t dir;
} motor_t;

int8_t motor_init(void);
int8_t motor_spin(direction_t dir, uint8_t pulse_percentage);
void motor_reset(void);
void _motor_turn_on(void);
void _motor_turn_off(void);
int32_t _motor_get_real_pulse(uint8_t pulse_percentage);

#endif /* APP_LIB_MOTOR_H */
