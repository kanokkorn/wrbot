#ifndef MAIN_H
#define MAIN_H

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "fsm.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Constants */
#define TOLERANCE    2.0
#define EARTH_RADIUS 6371000.0 // meters

/* GPS coordinate */
typedef struct {
  double lat;
  double lon;
} gps_t;

/* Robot state */
typedef struct {
  gps_t position;
  double speed;
  double angle;
  double distance_to_target;
  fsm_t fsm;
} robot_t;

/* Inline math functions */
static inline double calculate_speed(double dist_a, double dist_b, int time) {
  return time == 0 ? 0 : (dist_b - dist_a) / (double)time;
}

static inline double deg_to_rad(double deg) { return deg * M_PI / 180.0; }
static inline double rad_to_deg(double rad) { return rad * 180.0 / M_PI; }

extern volatile sig_atomic_t stop_signal;

/* Function Prototypes */
void print_robot_status(const robot_t *bot);
void initialize_robot(robot_t *bot);
void handle_interrupt(int signal);
int self_check(robot_t *bot);

#endif /* MAIN_H */
