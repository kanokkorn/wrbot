#include "main.h"

int run_simulation(robot_t *bot) {
    const char *fname = "gps_list.txt";
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int round = 0;

    FILE *gps_file = fopen(fname, "r");
    if (!gps_file) {
        perror("Error opening GPS file");
        return 1;
    }

    printf("Reading waypoints from: %s\n", fname);
    while ((read = getline(&line, &len, gps_file)) != -1) {
        printf("Processing waypoint #%d\n", ++round);
        process_waypoint(bot, line);
    }

    if (ferror(gps_file)) {
        perror("I/O error while reading");
    }

    fclose(gps_file);
    if (line) {
        free(line);
    }
    return 0;
}

void process_waypoint(robot_t *bot, char *waypoint_str) {
    char *lat_str = strtok(waypoint_str, ",");
    char *lon_str = strtok(NULL, " ");

    if (!lat_str || !lon_str) {
        fprintf(stderr, "Invalid waypoint format: %s\n", waypoint_str);
        return;
    }

    double dest_lat = atof(lat_str);
    double dest_lon = atof(lon_str);
    bot->distance_to_target = haversine(bot, dest_lat, dest_lon);

    while (bot->distance_to_target >= TOLERANCE) {
        double prev_distance = bot->distance_to_target;
        bot->distance_to_target = haversine(bot, dest_lat, dest_lon);
        bot->speed = calculate_speed(bot->distance_to_target, prev_distance, 1);

        print_robot_status(bot);
        update_robot_mock_position(bot);
        sleep(1);
    }

    printf("--- Waypoint reached ---\n");
    sleep(2); // Simulate task execution
    printf("Task finished. Moving to next waypoint...\n");
    sleep(1);
}

double haversine(const robot_t *bot, double dest_lat, double dest_lon) {
    double lat_rad = deg_to_rad(bot->position.lat);
    double dest_lat_rad = deg_to_rad(dest_lat);
    double d_lat = deg_to_rad(dest_lat - bot->position.lat);
    double d_lon = deg_to_rad(dest_lon - bot->position.lon);

    double a = pow(sin(d_lat / 2), 2) + cos(lat_rad) * cos(dest_lat_rad) * pow(sin(d_lon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}

void print_robot_status(const robot_t *bot) {
    printf("\033[2J\033[H"); // Clear screen
    printf("--- Robot Status ---\n");
    printf("Position:  (Lat: %f, Lon: %f)\n", bot->position.lat, bot->position.lon);
    printf("Speed:     %f m/s\n", bot->speed);
    printf("Angle:     %f degrees\n", bot->angle);
    printf("Distance:  %f meters\n", bot->distance_to_target);
    printf("--------------------\n");
}

void handle_interrupt(int signal) {
    printf("\nInterrupt received, stopping robot...\n");
    exit(signal);
}

void initialize_robot(robot_t *bot) {
    srand(time(NULL));
    bot->position.lat = (double)rand() / RAND_MAX * 180.0 - 90.0;
    bot->position.lon = (double)rand() / RAND_MAX * 360.0 - 180.0;
    bot->speed = 0.0;
    bot->angle = 0.0;
    bot->distance_to_target = 0.0;
    fsm_init(&bot->fsm);
}

void update_robot_mock_position(robot_t *bot) {
    // Simulate movement
    bot->position.lat += (double)rand() / RAND_MAX * 0.001 - 0.0005;
    bot->position.lon += (double)rand() / RAND_MAX * 0.001 - 0.0005;
}

int main(void) {
    robot_t *bot = malloc(sizeof(robot_t));
    if (!bot) {
        perror("Failed to allocate memory for robot");
        return 1;
    }

    initialize_robot(bot);
    signal(SIGINT, handle_interrupt);

    if (run_simulation(bot) != 0) {
        fprintf(stderr, "Simulation failed.\n");
        free(bot);
        return 1;
    }

    free(bot);
    return 0;
}
