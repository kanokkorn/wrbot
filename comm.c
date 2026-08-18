#define _POSIX_C_SOURCE 200809L
#include "comm.h"
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

static const char *server_addr = "127.0.0.1";
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
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("[comm] Socket creation failed");
    return;
  }

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(7000);

  if (inet_pton(AF_INET, server_addr, &address.sin_addr) <= 0) {
    fprintf(stderr, "[comm] Invalid address: %s\n", server_addr);
    close(client_socket);
    return;
  }

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

  close(client_socket);
}
