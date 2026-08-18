#include "pathd.h"
#include "navigate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int pathd_init(void) {
  printf("[pathd] Initializing Path Planning Daemon...\n");
  return 0;
}

int pathd_run_simulation(robot_t *bot) {
  const char *fname = "gps_list.txt";
  int fd = open(fname, O_RDONLY);
  if (fd < 0) {
    perror("[pathd] Error opening GPS file");
    return 1;
  }

  struct stat st;
  if (fstat(fd, &st) < 0 || st.st_size == 0) {
    perror("[pathd] Error stating GPS file");
    close(fd);
    return 1;
  }

  char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  if (map == MAP_FAILED) {
    perror("[pathd] mmap failed");
    return 1;
  }

  printf("[pathd] Reading waypoints from: %s\n", fname);
  int round = 0;
  const char *ptr = map;
  const char *end = map + st.st_size;

  while (ptr < end && !stop_signal) {
    const char *line_start = ptr;
    while (ptr < end && *ptr != '\n') ptr++;
    size_t line_len = ptr - line_start;
    if (ptr < end && *ptr == '\n') ptr++;

    if (line_len == 0 || line_len >= 128) continue;

    char line_buf[128];
    memcpy(line_buf, line_start, line_len);
    line_buf[line_len] = '\0';

    char *comma = strchr(line_buf, ',');
    if (!comma) continue;
    *comma = '\0';

    double dest_lat = atof(line_buf);
    double dest_lon = atof(comma + 1);

    printf("[pathd] Processing waypoint #%d\n", ++round);
    bot->distance_to_target = haversine(bot, dest_lat, dest_lon);

    while (bot->distance_to_target >= TOLERANCE && !stop_signal) {
      if (bot->fsm.current_state != ROBOT_STATE_MOVING) {
        print_robot_status(bot);
        printf("[pathd] Robot is %s. Waiting for EXEC command...\n",
               (bot->fsm.current_state == ROBOT_STATE_IDLE) ? "IDLE" : "WORKING");
        sleep(2);
        continue;
      }

      double prev_distance = bot->distance_to_target;
      bot->distance_to_target = haversine(bot, dest_lat, dest_lon);

      double speed = calculate_speed(bot->distance_to_target, prev_distance, 1);
      bot->speed = (speed < 0) ? -speed : speed;
      if (bot->speed < 0.5) bot->speed = 1.0;

      print_robot_status(bot);
      update_robot_mock_position(bot, dest_lat, dest_lon);
      sleep(1);
    }

    if (stop_signal) break;

    printf("[pathd] --- Waypoint reached ---\n");
    sleep(2);
    printf("[pathd] Task finished. Moving to next waypoint...\n");
    sleep(1);
  }

  munmap(map, st.st_size);
  return 0;
}
