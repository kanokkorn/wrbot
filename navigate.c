#include "navigate.h"
#include <math.h>
#include <stdlib.h>

double haversine(const robot_t *bot, double dest_lat, double dest_lon) {
  double lat_rad = deg_to_rad(bot->position.lat);
  double dest_lat_rad = deg_to_rad(dest_lat);
  double d_lat = deg_to_rad(dest_lat - bot->position.lat);
  double d_lon = deg_to_rad(dest_lon - bot->position.lon);

  double a = pow(sin(d_lat / 2.0), 2) + cos(lat_rad) * cos(dest_lat_rad) * pow(sin(d_lon / 2.0), 2);
  return EARTH_RADIUS * (2.0 * atan2(sqrt(a), sqrt(1.0 - a)));
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
  double bearing_rad = deg_to_rad(bearing);

  bot->position.lat += (bot->speed * cos(bearing_rad)) / METERS_PER_DEGREE;
  bot->position.lon += (bot->speed * sin(bearing_rad)) / (METERS_PER_DEGREE * cos(deg_to_rad(bot->position.lat)));

  // Add positional noise
  bot->position.lat += ((double)rand() / RAND_MAX * 0.00001) - 0.000005;
  bot->position.lon += ((double)rand() / RAND_MAX * 0.00001) - 0.000005;
}
