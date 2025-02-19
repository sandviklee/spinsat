#include "zephyr/sys/printk.h"
#include <example.h>

int main(int argc, char *argv[]) {
  char *name = "Zephyr";
  uint8_t result = example(1, 2);
  printk("Hello World! %s\n", name);
  printk("Result: %d\n", result);
  return 0;
}
