#define _POSIX_C_SOURCE 200809L
#include "comm.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t comm_pid = -1;

typedef enum { RESET, READY, SAVE, WAIT, STOP, EXEC } Status;

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

  while (1) {
    sleep(10);
    Status check = (Status)(rand() % 6);

    switch (check) {
      case RESET:
        printf("[comm] RESET received: resetting position and task now\n");
        bot->position.lat = 10.9995;
        bot->position.lon = 10.9995;
        bot->fsm.current_state = ROBOT_STATE_IDLE;
        break;
      case STOP:
        printf("[comm] STOP received: stopping now\n");
        bot->speed = 0;
        bot->fsm.current_state = ROBOT_STATE_IDLE;
        break;
      case SAVE:
        printf("[comm] SAVE received: saving current state...\n");
        break;
      case WAIT:
        printf("[comm] WAIT received: pausing task...\n");
        bot->fsm.current_state = ROBOT_STATE_IDLE;
        break;
      case EXEC:
        printf("[comm] EXEC received: executing stored tasks\n");
        bot->fsm.current_state = ROBOT_STATE_MOVING;
        break;
      case READY:
      default:
        printf("[comm] READY: waiting for next command...\n");
        break;
    }
  }
}
