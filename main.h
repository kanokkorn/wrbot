#define _DEFAULT_SOURCE

#ifndef __has_builtin
  #define __has__builtin(x) 0
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TOLERANCE_VALUE 2.00
#define EARTH_RAD 6.3781e6
#define LEN 2
#define TOTAL LEN * 1000

#define BUF 10
#define BUFSIZE

#ifdef TARGET_HW
#include "hw.h"
#endif

#ifdef ADV_METHOD
#include "adv.h"
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>


typedef struct {
  double lat;
  double lon;
  double speed;
  double angle;
  double wr_distance;
}wrbot;

static inline double vspeed(double distance_a, double distance_b, int time) {
  return (distance_b - distance_a) / time;
}

static inline double degToRad(double angleInDegrees) {
  return angleInDegrees * M_PI / 180;
}

static inline double radToDeg(double angleInRadians) {
  return angleInRadians * 180 / M_PI;
}

/* robot */
void robot_status(wrbot *bot);
void robot_value_init(wrbot *bot);
void robot_loc_mock(wrbot *bot);
void robot_failsafe(double);
void robot_sigint(int);

/* prototypes */
int run(wrbot *bot);
void compute(wrbot *bot, char*);
double haversine(wrbot *bot, double, double);
