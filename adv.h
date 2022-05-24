/*
 * File: adv.h
 * Author: kanokkorn kornkankit <kanokorn@outlook.jp>
 */

#ifdef ADV_METHOD
#include <stddef.h>
#include <math.h>

#define Q_ELEM 100
#define Q_SIZE (Q_ELEM + 1)

uint8_t q[Q_SIZE];
float q_in, q_out;
int x, h;
static float avg;
static float predict;

void q_init (void) {
  q_in = q_out = 0;
}

int q_put (float data) {
  if (q_in == (( q_out - 1 + Q_SIZE ) % Q_SIZE)) {
    q_in = 0;
    for (x = 0; x < Q_ELEM; x++) {
      avg += q[x];
    }
    avg / Q_ELEM;
  }
  q[q_in] = data;
  q_in = (q_in + 1) % Q_SIZE;
  return 1;
}

#endif /* ADV_METHOD */
