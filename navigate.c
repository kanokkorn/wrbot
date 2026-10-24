#define _POSIX_C_SOURCE 200809L
#include "navigate.h"
#include <math.h>
#include <stdlib.h>

double haversine(const robot_t *bot, double dest_lat, double dest_lon) {
  double lat1 = deg_to_rad(bot->position.lat), lat2 = deg_to_rad(dest_lat);
  double d_lat = deg_to_rad(dest_lat - bot->position.lat) / 2.0;
  double d_lon = deg_to_rad(dest_lon - bot->position.lon) / 2.0;
  double a = sin(d_lat) * sin(d_lat) + cos(lat1) * cos(lat2) * sin(d_lon) * sin(d_lon);
  return 2.0 * EARTH_RADIUS * asin(sqrt(a));
}

double calculate_bearing(double lat1, double lon1, double lat2, double lon2) {
  double rlat1 = deg_to_rad(lat1), rlat2 = deg_to_rad(lat2), d_lon = deg_to_rad(lon2 - lon1);
  double y = sin(d_lon) * cos(rlat2);
  double x = cos(rlat1) * sin(rlat2) - sin(rlat1) * cos(rlat2) * cos(d_lon);
  return fmod(rad_to_deg(atan2(y, x)) + 360.0, 360.0);
}

void update_robot_mock_position(robot_t *bot, double dest_lat, double dest_lon) {
  if (bot->speed <= 0) bot->speed = 1.0;
  bot->angle = calculate_bearing(bot->position.lat, bot->position.lon, dest_lat, dest_lon);

  const double METERS_PER_DEG = 111111.0;
  double rad_b = deg_to_rad(bot->angle);
  double noise = ((double)rand() / RAND_MAX * 0.00001) - 0.000005;

  bot->position.lat += (bot->speed * cos(rad_b)) / METERS_PER_DEG + noise;
  bot->position.lon += (bot->speed * sin(rad_b)) / (METERS_PER_DEG * cos(deg_to_rad(bot->position.lat))) + noise;
}
