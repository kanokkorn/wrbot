/*
 * wrbot - simple rover navigate with gps
 */

#define VERSION "0.1.0"
#define _DEFAULT_SOURCE

#ifndef __has_builtin
  #define __has__builtin(x) 0
#endif

/*  math pi  */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*  some color print  */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RST "\x1B[0m"

/*  pre-define value  */
#define PLACEHOLDER 10
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

#if defined(__linux__) && defined(__x86_64__)
#define PLATFORM "x86_64 Linux"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#endif /* linux */

/* 
    TODO: support gpsfile & history with sqlite (if run completed)
      data template for loading gps position
      position file design:
       - csv format, below is example of file
       - (incremental_order, latitude, lontitude, command_set, date_execute, date_create)
          ^                                       ^
          |  per file                             | nullable
       - can be load and off-load to sql
       - 
 */

typedef struct {
  double      lat;
  double      lon;
  double      speed;
  double      angle;
  double      wr_distance;
}vehicle_stat;

/* GPS data */
typedef struct{
  double lat;
  double lon;
  double speed;
  double angle;
  double wr_distance;
}wrbot;

/* 
  TODO: reimplement gpsdata 
    - gpsdata have 4 uses : gpssock, gpsfile, gpscurrent, gpsmock
    - float for lat, lon should be precise enough
 */
typedef struct {
  double lat;
  double lon;
}gpsdata;

struct gps_data *gps_head = NULL;
struct gps_data *gps_current = NULL;

static inline double vspeed(double distance_a, double distance_b, double timeSpend) {
  return (distance_b - distance_a) / timeSpend;
}

static inline double degToRad(double angleInDegrees) {
  return angleInDegrees * M_PI / 180;
}

static inline double radToDeg(double angleInRadians) {
  return angleInRadians * 180 / M_PI;
}

/* robot */
void robot_status                     (wrbot *bot);
void robot_value_init_rand            (wrbot *bot);
void robot_value_init_nearby          (wrbot *bot);
void robot_loc_mock_rand              (wrbot *bot);
void robot_loc_mock_inc               (wrbot *bot);
void robot_failsafe                       (double);
void robot_sigint                            (int);

/* prototypes */
int     run                    (wrbot *bot, char*);
void    compute                (wrbot *bot, char*);
double  haversine     (wrbot *bot, double, double);
int     gps_probe                     (wrbot *bot);
