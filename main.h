#define _GNU_SOURCE

#define tolerance_value 2.00
#define earth_rad 6.3781e6

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

#ifdef TARGET_HW
#include "interface.h"
#endif

#ifdef ADV_METHOD
#include "kalman.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void run(void);
void compute(char*);
double haversine(double, double, double, double);
