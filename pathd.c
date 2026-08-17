#define _POSIX_C_SOURCE 200809L
#include "pathd.h"
#include "navigate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

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
    perror("[pathd] Error getting file status or file empty");
    close(fd);
    return 1;
  }

  char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);

  if (map == MAP_FAILED) {
    perror("[pathd] Error memory mapping GPS file");
    return 1;
  }

  printf("[pathd] Reading waypoints from: %s\n", fname);

  const char *p = map;
  const char *end = map + st.st_size;
  int round = 0;

  while (p < end && !stop_signal) {
    const char *line_end = p;
    while (line_end < end && *line_end != '\n' && *line_end != '\r') {
      line_end++;
    }

    if (line_end == p) {
      p++;
      continue;
    }

    double dest_lat, dest_lon;
    if (sscanf(p, "%lf,%lf", &dest_lat, &dest_lon) == 2) {
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

    p = line_end;
    while (p < end && (*p == '\n' || *p == '\r')) {
      p++;
    }
  }

  munmap(map, st.st_size);
  return 0;
}
