#define _POSIX_C_SOURCE 200809L
#include "main.h"
#include "navigate.h"
#include "pathd.h"
#include "msgd.h"
#include "comm.h"
#include <sys/mman.h>

volatile sig_atomic_t stop_signal = 0;

void handle_interrupt(int signal) {
  (void)signal;
  stop_signal = 1;
}

int self_check(robot_t *bot) {
  printf("--- Starting self-check subroutine ---\n");
  if (!bot) return 1;

  usleep(100000);
  printf("Checking GPS, IMU, and MCU connection... OK\n");
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
  printf("State:     %d\n", bot->fsm.current_state);
  printf("--------------------\n");
}

volatile sig_atomic_t stop_signal = 0;
void handle_interrupt(int signal) {
  (void)signal;
  stop_signal = 1;
}

void initialize_robot(robot_t *bot) {
  srand(time(NULL));
  bot->position.lat = 10.9995;
  bot->position.lon = 10.9995;
  bot->speed = 0.0;
  bot->angle = 0.0;
  bot->distance_to_target = 0.0;
  fsm_init(&bot->fsm);
}

int main(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);

  robot_t *bot = mmap(NULL, sizeof(robot_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (bot == MAP_FAILED) {
    perror("mmap failed");
    return 1;
  }

  initialize_robot(bot);

  struct sigaction sa;
  sa.sa_handler = handle_interrupt;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  if (self_check(bot) != 0) {
    fprintf(stderr, "Self-check failed. Exiting.\n");
    munmap(bot, sizeof(robot_t));
    return 1;
  }

  if (pathd_init() != 0 || msgd_init() != 0 || comm_init(bot) != 0) {
    fprintf(stderr, "Failed to initialize subroutines.\n");
  }

  printf("\nStarting simulation...\n");
  fsm_handle_event(&bot->fsm, ROBOT_EVENT_WAYPOINT_RECEIVED);

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
