#ifndef INTERFACE_H
#define INTERFACE_H

#if defined(__aarch64__) && defined(__unix__)

#define led_pin 23

#include <gpiod.h>

static unsigned int config = 0;

void interface_setup(void);
void robot_signal_light(int);

#endif /* aarch64 & unix */

#endif
