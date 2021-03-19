#include "interface.h"

void interface_setup(void) {
  const char* chipname = "gpiochip0";
  struct gpiod_chip *chip;
  struct gpiod_line *line;
  chip = gpiod_chip_open_by_name(chipname);
  if (!chip) {
    perror("Open chip failed\n");
  }
  line = gpiod_chip_get_line(chip, line_num);
  if (!line) {
    perror("Get line failed\n");
  }
  ret = gpiod_line_request_output(line, CONSUMER, 0);
  if (ret < 0) {
    perror("Request line as output failed\n");
  }
  config = 1;
}

void robot_signal_light(int status) {
  enum {stop, run}
  if (!config) {
    setup();
  } else {
    switch (status) {
      case stop:
        /* static */
        break;
      case run:
        /* blink */
        break;
      default:
        /* another LED */
    }
  }
}
