/*
 * File: hw.h
 * Author: kanokkorn kornkankit <kanokorn@outlook.jp>
 */

#ifdef __ARM64__
#include <gpio.h>
#include <gps.h>
#include <math.h>
#include <unistd.h>

/* RASPI CHIPSET */
#define CONSUMER "bcm"
#define led_pin 23

#define MODE_STR_NUM 4
static char* mode_str[MODE_STR_NUM] = {"n/a", "None", "2D", "3D"};

int gps_open(char* server, char* port, struct gps_data_t* gpsdata);
int gps_read(struct gps_data_t* gpsdata, char* message, int message_size);

uint8_t check_gps_connection(void) {
  struct gps_data_t gps_data;
  if (0 != gps_open("localhost", "2947", &gps_data))
    return 0;
  else
    return 1;
}

void raspi_setup(void) {
  struct gps_data_t gps_data;
  const char* chipname = "gpiochip0";
  struct gpiod_chip* chip;
  struct gpiod_line* line;
  chip = gpiod_chip_open_by_name(chipname);
  (void)gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);
  if (!check_gps_connection) {
    perror("GPS failed to connect\n");
  }
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
}
void robot_signal_light(int) {}

#endif
