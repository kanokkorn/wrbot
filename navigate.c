#define _POSIX_C_SOURCE 200809L
#include "navigate.h"
#include <math.h>
#include <stdlib.h>

double haversine(const robot_t *bot, double dest_lat, double dest_lon) {
  double lat1 = deg_to_rad(bot->position.lat);
  double lat2 = deg_to_rad(dest_lat);
  double d_lat = deg_to_rad(dest_lat - bot->position.lat);
  double d_lon = deg_to_rad(dest_lon - bot->position.lon);

  double sin_lat = sin(d_lat / 2.0);
  double sin_lon = sin(d_lon / 2.0);
  double a = sin_lat * sin_lat + cos(lat1) * cos(lat2) * sin_lon * sin_lon;

  return EARTH_RADIUS * 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
}

double calculate_bearing(double lat1, double lon1, double lat2, double lon2) {
  double lat1_rad = deg_to_rad(lat1);
  double lat2_rad = deg_to_rad(lat2);
  double d_lon = deg_to_rad(lon2 - lon1);

  double y = sin(d_lon) * cos(lat2_rad);
  double x = cos(lat1_rad) * sin(lat2_rad) - sin(lat1_rad) * cos(lat2_rad) * cos(d_lon);
  return fmod(rad_to_deg(atan2(y, x)) + 360.0, 360.0);
}

void update_robot_mock_position(robot_t *bot, double dest_lat, double dest_lon) {
  if (bot->speed <= 0) bot->speed = 1.0;

  double bearing = calculate_bearing(bot->position.lat, bot->position.lon, dest_lat, dest_lon);
  bot->angle = bearing;

  const double METERS_PER_DEGREE = 111111.0;
  double d_lat = (bot->speed * cos(deg_to_rad(bearing))) / METERS_PER_DEGREE;
  double d_lon = (bot->speed * sin(deg_to_rad(bearing))) / (METERS_PER_DEGREE * cos(deg_to_rad(bot->position.lat)));

  bot->position.lat += d_lat + (((double)rand() / RAND_MAX * 0.00001) - 0.000005);
  bot->position.lon += d_lon + (((double)rand() / RAND_MAX * 0.00001) - 0.000005);
}
