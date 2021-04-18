#define _GNU_SOURCE

#define tolerance_value 2.00
#define earth_rad 6.3781e6

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

#define vspeed(distance_a, distance_b, time) ((distance_b - distance_a) / time )

#ifdef TARGET_HW
#include "interface.h"
#endif

#ifdef ADV_METHOD
#include "kalman.h"
#endif

#include <stdio.h>
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
  double distance;
}wrbot;

void run(wrbot *bot);
void compute(wrbot *bot, char*);
void robot_status(wrbot *bot);
void robot_value_init(wrbot *bot);
void robot_loc_mock(wrbot *bot);
void robot_failsafe(double);
void robot_sigint(int);
double haversine(wrbot *bot, double, double);
