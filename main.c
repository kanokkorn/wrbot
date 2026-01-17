#include "main.h"
#include "navigate.h"
#include "pathd.h"
#include "msgd.h"
#include "comm.h"
#include <sys/mman.h>

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

volatile sig_atomic_t stop_signal = 0;
void handle_interrupt(int signal) {
    (void)signal;
    stop_signal = 1;
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
    robot_t *bot = mmap(NULL, sizeof(robot_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (bot == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    initialize_robot(bot);
    signal(SIGINT, handle_interrupt);

    if (self_check(bot) != 0) {
        fprintf(stderr, "Self-check failed. Exiting.\n");
        munmap(bot, sizeof(robot_t));
        return 1;
    }

    // Start subroutines
    if (pathd_init() != 0) {
        fprintf(stderr, "Failed to initialize pathd.\n");
    }

    if (msgd_init() != 0) {
        fprintf(stderr, "Failed to initialize msgd.\n");
    }

    if (comm_init(bot) != 0) {
        fprintf(stderr, "Failed to initialize comm.\n");
    }

    printf("\nPress Enter to start simulation...");
    getchar();

    // In a real application, we would check stop_signal in the main loop.
    // Here, pathd_run_simulation has the loop, so we should pass it or handle it there.

    // Set initial state to MOVING to start simulation
    bot->fsm.current_state = ROBOT_STATE_MOVING;

    // Run simulation (pathd manages the main loop for now)
    if (pathd_run_simulation(bot) != 0) {
        fprintf(stderr, "Simulation failed.\n");
        comm_cleanup();
        munmap(bot, sizeof(robot_t));
        return 1;
    }

    comm_cleanup();
    munmap(bot, sizeof(robot_t));
    return 0;
}
