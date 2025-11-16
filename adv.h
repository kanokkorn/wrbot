/*
 * File: adv.h
 * Author: kanokkorn kornkankit <kanokorn@outlook.jp>
 * FIFO implementation for averaging result
 */

#ifdef ADV_METHOD
#include <math.h>
#include <stddef.h>
#include <stdint.h>

#define Q_ELEM 100
#define Q_SIZE (Q_ELEM + 1)

uint8_t q[Q_SIZE];
uint8_t q_in, q_out;
uint8_t h;
static float avg;
static float predict;

typedef struct {
  double val[10];
}store;

extern void q_init(void) { q_in = q_out = 0; }

extern uint8_t q_put(float data) {
  if (q_in == ((q_out - 1 + Q_SIZE) % Q_SIZE)) {
    q_in = 0;
    for (h = 0; h < Q_ELEM; h++) {
      avg += q[h];
    }
    avg = avg / Q_ELEM;
  }
  q[q_in] = (uint8_t)data;
  q_in = (q_in + 1) % Q_SIZE;
  return 1;
}

/* accept 10 values, find average */
double avrg(store *input) {
  /* not implement */
}

#endif /* ADV_METHOD */
