#ifndef NAVIGATE_H
#define NAVIGATE_H

#include "main.h"

double haversine(const robot_t *bot, double dest_lat, double dest_lon);
void update_robot_mock_position(robot_t *bot, double dest_lat, double dest_lon);
double calculate_bearing(double lat1, double lon1, double lat2, double lon2);

#endif /* NAVIGATE_H */
