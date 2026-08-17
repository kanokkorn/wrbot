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

  struct stat sb;
  if (fstat(fd, &sb) < 0) {
    perror("[pathd] fstat failed");
    close(fd);
    return 1;
  }

  char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);

  if (addr == MAP_FAILED) {
    perror("[pathd] mmap failed");
    return 1;
  }

  printf("[pathd] Reading waypoints using memory map from: %s\n", fname);
  int round = 0;
  const char *curr = addr;
  const char *limit = addr + sb.st_size;

  while (curr < limit && !stop_signal) {
    while (curr < limit && (*curr == ' ' || *curr == '\n' || *curr == '\r' || *curr == '\t')) {
      curr++;
    }
    if (curr >= limit) break;

    char *next;
    double dest_lat = strtod(curr, &next);
    if (curr == next || *next != ',') {
      while (curr < limit && *curr != '\n') curr++;
      continue;
    }
    curr = next + 1;
    double dest_lon = strtod(curr, &next);
    if (curr == next) {
      while (curr < limit && *curr != '\n') curr++;
      continue;
    }
    curr = next;

    printf("[pathd] Processing waypoint #%d\n", ++round);
    fsm_handle_event(&bot->fsm, ROBOT_EVENT_WAYPOINT_RECEIVED);
    bot->distance_to_target = haversine(bot, dest_lat, dest_lon);

        print_robot_status(bot);
        printf("[pathd] Robot is not MOVING. Waiting for state update...\n");
        sleep(2);
        continue;
      }

      double prev_distance = bot->distance_to_target;
      bot->distance_to_target = haversine(bot, dest_lat, dest_lon);

      double speed = calculate_speed(bot->distance_to_target, prev_distance, 1);
      bot->speed = (speed < 0) ? -speed : speed;
      if (bot->speed < 0.5) bot->speed = 1.0;

      printf("[pathd] --- Waypoint reached ---\n");
      sleep(2);
      printf("[pathd] Task finished. Moving to next waypoint...\n");
      sleep(1);
    }

    if (stop_signal) break;

    printf("[pathd] --- Waypoint reached ---\n");
    fsm_handle_event(&bot->fsm, ROBOT_EVENT_WAYPOINT_REACHED);
    sleep(1);
    printf("[pathd] Task finished. Transitioning state...\n");
    fsm_handle_event(&bot->fsm, ROBOT_EVENT_TASK_COMPLETED);
    sleep(1);
  }

  munmap(addr, sb.st_size);
  return 0;
}
