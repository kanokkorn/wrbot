#define _POSIX_C_SOURCE 200809L
#include "pathd.h"
#include "navigate.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int pathd_init(void) {
  printf("[pathd] Initializing Path Planning Daemon...\n");
  return 0;
}

int pathd_run_simulation(robot_t *bot) {
  const char *fname = "gps_list.txt";
  int fd = open(fname, O_RDONLY);
  if (fd < 0) {
    perror("Error opening GPS file");
    return 1;
  }

  struct stat st;
  if (fstat(fd, &st) < 0) {
    perror("Error getting GPS file size");
    close(fd);
    return 1;
  }

  if (st.st_size == 0) {
    close(fd);
    return 0;
  }

  char *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  if (data == MAP_FAILED) {
    perror("Error mapping GPS file");
    return 1;
  }

  int round = 0;
  const char *ptr = data;
  const char *end = data + st.st_size;

  while (ptr < end && !stop_signal) {
    const char *line_end = memchr(ptr, '\n', end - ptr);
    if (!line_end) {
      line_end = end;
    }

    size_t line_len = line_end - ptr;
    if (line_len > 0) {
      char line[128];
      if (line_len >= sizeof(line)) {
        line_len = sizeof(line) - 1;
      }
      memcpy(line, ptr, line_len);
      line[line_len] = '\0';

      char *lat_str = strtok(line, ",");
      char *lon_str = strtok(NULL, " \n\r");

      if (lat_str && lon_str) {
        printf("[pathd] Processing waypoint #%d\n", ++round);
        double dest_lat = atof(lat_str);
        double dest_lon = atof(lon_str);
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
    }

    ptr = (line_end < end) ? line_end + 1 : end;
  }

  munmap(data, st.st_size);
  return 0;
}
