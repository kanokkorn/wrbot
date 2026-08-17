#define _POSIX_C_SOURCE 200809L
#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

static pid_t comm_pid = -1;

int comm_init(robot_t *bot) {
  printf("[comm] Initializing Remote Control Daemon (TCP/IP)...\n");

  comm_pid = fork();
  if (comm_pid < 0) {
    perror("[comm] Fork failed");
    return -1;
  }

  if (comm_pid == 0) {
    comm_handle_remote(bot);
    exit(0);
  }

  return 0;
}

void comm_cleanup(void) {
  if (comm_pid > 0) {
    printf("[comm] Stopping Remote Control Daemon (PID: %d)...\n", comm_pid);
    kill(comm_pid, SIGTERM);
    waitpid(comm_pid, NULL, 0);
    comm_pid = -1;
  }
}

void comm_handle_remote(robot_t *bot) {
  printf("[comm] Remote Control Daemon (PID: %d) is running...\n", getpid());
  srand(getpid() ^ (unsigned int)time(NULL));

  while (1) {
    sleep(2);

    int check = rand() % 6;
    switch (check) {
      case 0: // RESET
        printf("[comm] RESET received: resetting position\n");
        bot->position.lat = 10.9995;
        bot->position.lon = 10.9995;
        fsm_handle_event(&bot->fsm, ROBOT_EVENT_TASK_COMPLETED);
        break;
      case 1: // STOP
        printf("[comm] STOP received: stopping now\n");
        bot->speed = 0;
        fsm_handle_event(&bot->fsm, ROBOT_EVENT_TASK_COMPLETED);
        break;
      case 2: // WAIT / IDLE
        printf("[comm] WAIT received: pausing task...\n");
        fsm_handle_event(&bot->fsm, ROBOT_EVENT_TASK_COMPLETED);
        break;
      case 3: // EXEC / MOVE
        printf("[comm] EXEC received: executing stored tasks\n");
        fsm_handle_event(&bot->fsm, ROBOT_EVENT_WAYPOINT_RECEIVED);
        break;
      default:
        break;
    }
  }
}
