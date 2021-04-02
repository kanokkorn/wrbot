#include "kalman.h"

float time_update(kalman *kx) {
   return kx->current_estimate = (const_a  * kx->current_estimate) + (const_b * no_signal); 
}

float measure_update(kalman *kx) {
  return kx->kalman_gain = kx->error_covariance * pow(const_h, time); 
}

void initial_state(kalman *kx) {
  kx->previous_estimate = 0;
  kx->error_covariance  = 0;
}
