#include "zephyr/sys/printk.h"

int main(int argc, char *argv[]) {
  char *name = "Zephyr";
  printk("Hello World! %s\n", name);
  return 0;
}
