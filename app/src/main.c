#include "state_machine.h"
#include "zephyr/sys/printk.h"
#include <stdint.h>

int main() {
  char *os = "Zephyr";
  printk("%s: Starting State Machine... \n", os);
  state_machine_t state_machine = {
      .running = true,
      .state = IDLE,
  };
  state_machine_init(&state_machine);
  return 0;
}
