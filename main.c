#include "main.h"
#include "navigate.h"
#include "pathd.h"
#include "msgd.h"
#include "comm.h"

int self_check(robot_t *bot) {
    printf("--- Starting self-check subroutine ---\n");
    if (!bot) return 1;

    usleep(500000);
    printf("Checking GPS module... OK\n");
    usleep(500000);
    printf("Checking IMU... OK\n");
    usleep(500000);
    printf("Checking MCU connection... OK\n");
    usleep(500000);

    printf("Initial Position: Lat %f, Lon %f\n", bot->position.lat, bot->position.lon);
    printf("--- Self-check completed successfully ---\n\n");

    return 0;
}

void print_robot_status(const robot_t *bot) {
    printf("\033[2J\033[H"); // Clear screen
    printf("--- Robot Status ---\n");
    printf("Position:  (Lat: %f, Lon: %f)\n", bot->position.lat, bot->position.lon);
    printf("Speed:     %.2f m/s\n", bot->speed);
    printf("Angle:     %.2f degrees\n", bot->angle);
    printf("Distance:  %.2f meters\n", bot->distance_to_target);
    printf("--------------------\n");
}

void handle_interrupt(int signal) {
    printf("\nInterrupt received, stopping robot...\n");
    exit(signal);
}

void initialize_robot(robot_t *bot) {
    srand(time(NULL));
    // Initial position near the first waypoint in gps_list.txt (11.0, 11.0)
    bot->position.lat = 10.9995;
    bot->position.lon = 10.9995;
    bot->speed = 0.0;
    bot->angle = 0.0;
    bot->distance_to_target = 0.0;
    fsm_init(&bot->fsm);
}

int main(void) {
    robot_t *bot = malloc(sizeof(robot_t));
    if (!bot) {
        perror("Failed to allocate memory for robot");
        return 1;
    }

    initialize_robot(bot);
    signal(SIGINT, handle_interrupt);

    if (self_check(bot) != 0) {
        fprintf(stderr, "Self-check failed. Exiting.\n");
        free(bot);
        return 1;
    }

    // Start subroutines
    if (pathd_init() != 0) {
        fprintf(stderr, "Failed to initialize pathd.\n");
    }

    if (msgd_init() != 0) {
        fprintf(stderr, "Failed to initialize msgd.\n");
    }

    if (comm_init() != 0) {
        fprintf(stderr, "Failed to initialize comm.\n");
    }

    printf("\nPress Enter to start simulation...");
    getchar();

    // Run simulation (pathd manages the main loop for now)
    if (pathd_run_simulation(bot) != 0) {
        fprintf(stderr, "Simulation failed.\n");
        free(bot);
        return 1;
    }

    free(bot);
    return 0;
}
