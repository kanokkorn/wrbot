/*
 * wrbot - haversine implementation for farming rover
 * refactor date : 05/03/2024
 * Goal :
 *   - read gps.csv file as input
 *   - file contain -> lat, lon, activity, duration
 *   - load and tokenize all value into data array
 *   - perform calculation between points
*/

#define _DEFAULT_SOURCE


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/*  ASCII color for terminal print  */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RST   "\x1B[0m"


/* pre-define value */
#define PLACEHOLDER 10
#define EARTH_RAD 6.3781e6

/* closest range for vehicle to stop before approaching target */
#define TOLERANCE_VALUE 2.00

/* header for interface (GPIO, USB) */
#ifdef TARGET_HW
#include "hw.h"
#endif


/* path planning */
#ifdef ADV_METHOD
#include "adv.h"
#endif

/* history, config and logging */
#ifdef DATABASE
#include "db.h"
#endif 

/* for developing on x86 */
#if defined(__linux__) && defined(__x86_64__)
#define PLATFORM "x86_64 Linux"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#endif

/* 
    TODO: support gpsfile & history with sqlite (if run completed)
      data template for loading GPS position
      position file design:
       - CSV format, below is example of file
       - (incremental_order, latitude, longitude, command_set, date_execute, date_create)
          ^                                       ^
          |  per file                             | nullable
       - can be load and off-load to SQL
 */

/* NMEA data struct */
typedef struct {
  double lat, lon;
}gpsdata;

typedef struct {
  double lat, lon;
}nmea_data;

typedef struct {
  gpsdata     current_pos;
  double      speed;
  double      angle;
  double      wr_distance;
}vehicle;


/* math function macros */
static inline double vspeed(double distance_a, double distance_b, double timeSpend) {
  return (distance_b - distance_a) / timeSpend;
}

static inline double degToRad(double angleInDegrees) {
  return angleInDegrees * M_PI / 180;
}

static inline double radToDeg(double angleInRadians) {
  return angleInRadians * 180 / M_PI;
}

static inline double sigmoid(double val) {
  return 1 / ( 1 + exp(-val) );
}

static inline int bmin(int a, int b) {
  return b + (( a - b) & (a - b) >> 31);
}

static inline int bmax(int a, int b) {
  return a - (( a - b) & (a - b) >> 31);
}
