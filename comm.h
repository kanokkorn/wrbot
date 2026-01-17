#ifndef COMM_H
#define COMM_H

#include "main.h"

int comm_init(robot_t *bot);
void comm_handle_remote(robot_t *bot);
void comm_cleanup(void);

#endif /* COMM_H */
