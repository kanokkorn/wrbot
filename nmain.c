/* refactor, more organise */ 

#include "nmain.h"

// typedef struct {
//   double lat, lon;
// }gpsdata;

// typedef struct {
//   gpsdata     current_pos;
//   double      speed;
//   double      angle;
//   double      wr_distance;
// }vehicle;

enum manuver {
  /* set of manuvering robot can do */
  go_forward  ,
  turn_left   ,
  turn_right  ,
  go_bachward
};

enum state {
  offline ,
  ready   ,
  moving  ,
  working
};

enum task {
  taking_picture,
  watering      ,
  processing
};

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

void diff_graph(double val) {
  double val_diff = val - temp_val;
  for (int x = 0; x < val_diff * 100; x++) {
    printf(":");
  }
  printf("\n");
  temp_val = val;
}

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
  if (c == 0.00) {
    return EARTH_RAD * c;
  } else {
    return 0.00;
  }
}

int main(void) {
  return 0;
}
