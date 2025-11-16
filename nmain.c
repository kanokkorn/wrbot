#include "nmain.h"

#include <stdio.h>

/*
typedef struct {
  gpsdata     current_pos;
  double      speed;
  double      angle;
  double      wr_distance;
}vehicle;
*/

/*
enum manuver {
  go_forward  ,
  turn_left   ,
  turn_right  ,
  go_bachward
};
*/

/*
enum state {
  offline ,
  ready   ,
  moving  ,
  working
};
*/

/*
enum task {
  taking_picture,
  watering      ,
  processing    ,
};
*/

double temp_val = 0.00;

static void file_check(char* gpsfile) {
  char *gps_line = NULL;
  size_t len = 0;
  ssize_t read;
  static int round = 0;
  FILE *gps_data = fopen(gpsfile, "r");
  fseek(gps_data, 0L, SEEK_END);
  long sz = ftell(gps_data);
  printf("loading GPS data from ["GRN"%s"RST"] total size ["GRN"%zu"RST" b]\n", gpsfile, sz);
  if (!gps_data) {
    perror("wrbot err::");
    exit(-1);
  }
}

static void s_curve(double val) {
  for (int x = 0; x < val * 25; x++) {
    printf(":");
  }
  printf("\n");
}

static void diff_graph(double val) {
  double val_diff = val - temp_val;
  for (int x = 0; x < val_diff * 100; x++) {
    printf(":");
  }
  printf("\n");
  temp_val = val;
}

/* haversine formula */
double haversine(gpsdata *a, gpsdata* b) {
  double c;
  double rad_lat_a = degToRad(a->lat);
  double rad_lat_b = degToRad(b->lat);
  double dt_lat = degToRad(b->lat - a->lat),
         dt_lon = degToRad(b->lon - a->lon);
  double x = pow(sin(dt_lat / 2), 2) +
             cos(rad_lat_a) * cos(rad_lat_b)
             * pow(dt_lon / 2, 2);
  c = (2 * atan2(sqrt(x), sqrt(1 - x)));
  if (c >= 0.00) {
    return EARTH_RAD * c;
  } else {
    return 0.00;
  }
}

double genposition(void) {
  return ((double)rand() / RAND_MAX) * 0.8;
}

// run simulation
int main(void) {
  srand(time(NULL));
  printf("testing haversine function\n");
  gpsdata *set_a = malloc(sizeof(gpsdata));
  gpsdata *set_b = malloc(sizeof(gpsdata));
  printf("[set a] lat:%f, lon: %f\n", set_a->lat, set_a->lon);
  for (int i = 0; i < 2000000; ++i) {
    set_a->lat = genposition(); set_a->lon = genposition();
    set_b->lat = genposition(); set_b->lon = genposition();
    printf("[round %3d] value a: [%f,%f] value b: [%f,%f] --> %.6f meters\n" , i+1,
           set_a->lat, set_a->lon, set_b->lat, set_b->lon, haversine(set_a, set_b)
           );
  }
  free(set_a); free(set_b);
  return 0;
}
