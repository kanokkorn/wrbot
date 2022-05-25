/*
 * File: adv.h
 * Author: kanokkorn kornkankit <kanokorn@outlook.jp>
 */

#ifdef ADV_METHOD
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#define Q_ELEM 100
#define Q_SIZE (Q_ELEM + 1)

uint8_t q[Q_SIZE];
uint8_t q_in, q_out;
uint8_t h;
static float avg;
static float predict;

void q_init (void) {
  q_in = q_out = 0;
}

uint8_t q_put (float data) {
  if (q_in == (( q_out - 1 + Q_SIZE ) % Q_SIZE)) {
    q_in = 0;
    for (h = 0; h < Q_ELEM; h++) {
      avg += q[h];
    }
    avg = avg / Q_ELEM;
  }
  q[q_in] = (uint8_t) data;
  q_in = (q_in + 1) % Q_SIZE;
  return 1;
}

#endif /* ADV_METHOD */
