#ifndef KALMAN_H
#define KALMAN_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#define guess(upper, lower) (( rand() % (upper - lower + 1)) + lower)

typedef struct {
  float current_estimate;
  float previous_estimate;
  float kalman_gain;
  float measured_val;
  float signal_val;
  float error_covariance;
}kalman;

void initial_state(kalman *kx);
float time_update(kalman *kx, float, float);
float measure_update(kalman *kx, float, float);

#endif
