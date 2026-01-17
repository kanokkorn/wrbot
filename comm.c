#include "comm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

// mock-up ip address
const char* server_addr = "192.168.1.120";

typedef enum {
  RESET,
  READY,
  SAVE,
  WAIT,
  STOP,
  EXEC
} Status;

// spawn a subprocess to handle external communication like TCP/IP .. etc 
int comm_init(void) {
    printf("[comm] Initializing Remote Control Daemon (TCP/IP)...\n");
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(7000);
    inet_aton(server_addr, (struct in_addr*) &(address.sin_addr.s_addr));
    return 0;
}

void comm_handle_remote(void) {
  int enable = 1; 
  while(enable) {
    Status check = READY;
    switch(check) {
      case RESET: printf("reset receive. resetting position and task now");
      case STOP:  printf("stop receive. stopping now");
      case SAVE:  printf("save receive. readding new position and task now");
      case WAIT:  printf("wait receive. waiting for x minites");
      default: printf("READY."); break;
    }
  }
}
