#include "pathd.h"
#include "navigate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int pathd_init(void) {
    printf("[pathd] Initializing Path Planning Daemon...\n");
    return 0;
}

int pathd_run_simulation(robot_t *bot) {
    const char *fname = "gps_list.txt";
    char *line = NULL;
    size_t len = 0;
    ssize_t read_bytes;
    int round = 0;

    FILE *gps_file = fopen(fname, "r");
    if (!gps_file) {
        perror("Error opening GPS file");
        return 1;
    }

    printf("[pathd] Reading waypoints from: %s\n", fname);
    while ((read_bytes = getline(&line, &len, gps_file)) != -1) {
        printf("[pathd] Processing waypoint #%d\n", ++round);

        char *lat_str = strtok(line, ",");
        char *lon_str = strtok(NULL, " \n\r");

        if (!lat_str || !lon_str) {
            fprintf(stderr, "[pathd] Invalid waypoint format\n");
            continue;
        }

        double dest_lat = atof(lat_str);
        double dest_lon = atof(lon_str);
        bot->distance_to_target = haversine(bot, dest_lat, dest_lon);

        while (bot->distance_to_target >= TOLERANCE) {
            double prev_distance = bot->distance_to_target;
            bot->distance_to_target = haversine(bot, dest_lat, dest_lon);

            // Avoid negative speed if we overshot a bit or due to noise
            double speed = calculate_speed(bot->distance_to_target, prev_distance, 1);
            bot->speed = (speed < 0) ? -speed : speed;
            if (bot->speed < 0.5) bot->speed = 1.0; // Keep moving

            print_robot_status(bot);
            update_robot_mock_position(bot, dest_lat, dest_lon);
            sleep(1);
        }

        printf("[pathd] --- Waypoint reached ---\n");
        sleep(2);
        printf("[pathd] Task finished. Moving to next waypoint...\n");
        sleep(1);
    }

    fclose(gps_file);
    if (line) {
        free(line);
    }
    return 0;
}
