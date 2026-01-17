/*
 * wrbot - haversine implementation for a farming rover
 */

#ifndef MAIN_H
#define MAIN_H

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include "fsm.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Constants */
#define TOLERANCE         2.0
#define EARTH_RADIUS      6371000.0 // meters

/* GPS coordinate */
typedef struct {
    double lat;
    double lon;
} gps_t;

/* Robot state */
typedef struct {
    gps_t  position;
    double speed;
    double angle;
    double distance_to_target;
    fsm_t  fsm;
} robot_t;

/* Inline math functions */
static inline double calculate_speed(double dist_a, double dist_b, int time) {
    if (time == 0) return 0;
    return (dist_b - dist_a) / (double)time;
}

static inline double deg_to_rad(double degrees) {
    return degrees * M_PI / 180.0;
}

static inline double rad_to_deg(double radians) {
    return radians * 180.0 / M_PI;
}

/* Function Prototypes */
void print_robot_status(const robot_t *bot);
void initialize_robot(robot_t *bot);
void handle_interrupt(int signal);
int self_check(robot_t *bot);

#endif /* MAIN_H */
