#define _POSIX_C_SOURCE 200809L
#include "navigate.h"
#include <math.h>
#include <stdlib.h>

#define METERS_PER_DEGREE 111111.0

double haversine(const robot_t *bot, double dest_lat, double dest_lon) {
  double lat1 = deg_to_rad(bot->position.lat);
  double lat2 = deg_to_rad(dest_lat);
  double d_lat = deg_to_rad(dest_lat - bot->position.lat);
  double d_lon = deg_to_rad(dest_lon - bot->position.lon);

  double sin_half_lat = sin(d_lat / 2.0);
  double sin_half_lon = sin(d_lon / 2.0);
  double a = sin_half_lat * sin_half_lat + cos(lat1) * cos(lat2) * sin_half_lon * sin_half_lon;

  return 2.0 * EARTH_RADIUS * asin(sqrt(a));
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

  double rad = deg_to_rad(bearing);
  double noise_lat = ((double)rand() / RAND_MAX * 0.00001) - 0.000005;
  double noise_lon = ((double)rand() / RAND_MAX * 0.00001) - 0.000005;

  bot->position.lat += (bot->speed * cos(rad)) / METERS_PER_DEGREE + noise_lat;
  bot->position.lon += (bot->speed * sin(rad)) / (METERS_PER_DEGREE * cos(deg_to_rad(bot->position.lat))) + noise_lon;
}
