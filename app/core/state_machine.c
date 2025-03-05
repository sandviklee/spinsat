#include "state_machine.h"
#include "zephyr/drivers/sensor/st/stmemsc/stmemsc.h"
#include <stdint.h>

void state_machine_init(state_machine_t *state_machine) {

  while (state_machine->running) {
    switch (state_machine->state) {
    case IDLE:
      break;
    case HOLD:

      break;
    case SPIN_LEFT:

      break;
    case SPIN_RIGHT:

      break;
    }
  }
};
