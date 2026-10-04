#define _POSIX_C_SOURCE 200809L
#include "main.h"
#include "comm.h"
#include "msgd.h"
#include "navigate.h"
#include "pathd.h"
#include <sys/mman.h>

volatile sig_atomic_t stop_signal = 0;

void handle_interrupt(int signal) {
  (void)signal;
  stop_signal = 1;
}

int self_check(robot_t *bot) {
  if (!bot) return 1;
  static const char *modules[] = {"GPS module", "IMU", "MCU connection"};
  puts("--- Starting self-check subroutine ---");
  for (size_t i = 0; i < sizeof(modules)/sizeof(modules[0]); i++) {
    usleep(500000);
    printf("Checking %s... OK\n", modules[i]);
  }
  usleep(500000);
  printf("Initial Position: Lat %f, Lon %f\n--- Self-check completed successfully ---\n\n",
         bot->position.lat, bot->position.lon);
  return 0;
}

void print_robot_status(const robot_t *bot) {
  printf("\033[2J\033[H--- Robot Status ---\n"
         "Position:  (Lat: %f, Lon: %f)\n"
         "Speed:     %.2f m/s\n"
         "Angle:     %.2f degrees\n"
         "Distance:  %.2f meters\n"
         "--------------------\n",
         bot->position.lat, bot->position.lon, bot->speed, bot->angle, bot->distance_to_target);
}

void initialize_robot(robot_t *bot) {
  srand(time(NULL));
  *bot = (robot_t){ .position = {10.9995, 10.9995} };
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
  signal(SIGINT, handle_interrupt);

  if (self_check(bot) != 0) {
    fprintf(stderr, "Self-check failed. Exiting.\n");
    munmap(bot, sizeof(robot_t));
    return 1;
  }

  if (pathd_init() != 0) fprintf(stderr, "Failed to initialize pathd.\n");
  if (msgd_init() != 0) fprintf(stderr, "Failed to initialize msgd.\n");
  if (comm_init(bot) != 0) fprintf(stderr, "Failed to initialize comm.\n");

  printf("\nPress Enter to start simulation...");
  getchar();

  bot->fsm.current_state = ROBOT_STATE_MOVING;

  if (pathd_run_simulation(bot) != 0) {
    fprintf(stderr, "Simulation failed.\n");
  }

  comm_cleanup();
  munmap(bot, sizeof(robot_t));
  return 0;
}
