#define bool int
#define false 0
#define true 1

/**
 * @file staet_machine.h
 * @brief  State machine structure
 *
 * This file contains the state machine structure and the initialization
 * function
 */

enum states_ { IDLE = 0, HOLD, SPIN_LEFT, SPIN_RIGHT };

typedef struct {
  enum states_ state;
  bool running;
} state_machine_t;

void state_machine_init(state_machine_t *state_machine);
