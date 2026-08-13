#include "msgd.h"
#include <stdio.h>

int msgd_init(void) {
  printf("[msgd] Initializing MCU Communication Daemon (UART/I2C)...\n");
  return 0;
}

void msgd_send_command(const char *command) {
  printf("[msgd] Sending command to MCU: %s\n", command);
}
