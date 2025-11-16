/*
* TODO: -  implement mini database for location history
 *       -  decode NMEA directly from `gpsd sock`
 */

#include "main.h"
#include <stdio.h>

int run(wrbot *bot, char* filename) {
  char *gps_line = NULL;
  size_t len = 0;
  ssize_t read;
  static int round = 0;
  FILE *gps_data = fopen(filename, "r");
  fseek(gps_data, 0L, SEEK_END);
  long sz = ftell(gps_data);
  printf("loading file ["GRN"%s"RST"] total size ["GRN"%zu"RST" b]\n", filename, sz);
  if (!gps_data) {
    perror("err:");
    exit(1);
  }
  while ((read = getline(&gps_line, &len, gps_data)) != -1) {
    printf("round #%d\n", round);
    puts(gps_line);
    compute(bot, gps_line);
    round++;
  }
  if (ferror(gps_data)) {
    printf("I/O error while reading\n");
  } else if (feof(gps_data)) {
    printf("End of file\n");
  }
  fclose(gps_data);
  if (gps_line) {
    free(gps_line);
  }
  return 0;
}

void compute(wrbot *bot, char *pos) {
  unsigned int x = 0;
  double wr_distance = 0.0000;
  char *gps_array[2];
  char *pos_tok = strtok(pos, ",");
  while (pos_tok) {
    gps_array[x++] = pos_tok;
    pos_tok = strtok(NULL, " ");
  }
   while (wr_distance >= TOLERANCE_VALUE) { 
    wr_distance = haversine(bot, atof(gps_array[0]), atof(gps_array[1]));
    bot->speed = vspeed(wr_distance, bot->wr_distance, 1);
    bot->wr_distance = wr_distance;
    // printf("distance to destination -> %f meters\n", wr_distance);
    robot_loc_mock_inc(bot);
    robot_status(bot);
    //sleep(1);
  }
  exit(0);
  printf("=== arrived ===\n");
  printf("doing task\n");
  /* task */
  sleep(20);
  printf("done...\n");
  sleep(5);
}

/* haversine with readble format */
double haversine(wrbot *bot, double lat_des, double lon_des) {
  double c;
  double lat_int_rad = degToRad(bot->lat), lat_des_rad = degToRad(lat_des);
  double lat_delta = degToRad(lat_des - bot->lat),
         lon_delta = degToRad(lon_des - bot->lon);
  double a = pow(sin(lat_delta / 2), 2) +
             cos(lat_int_rad) * cos(lat_des_rad)
             * pow(lon_delta / 2, 2);
  c = (2 * atan2(sqrt(a), sqrt(1 - a)));
  if (c == 0.00) {
    return EARTH_RAD * c;
  } else {
    return 0;
  }
}
/* more sane printing */
void robot_status(wrbot *bot) {
  printf(
    "current pos: %6.4f, %6.4f | speed: %9.2f m/s | head: %4.2f deg | distn: %9.f m\n",
    bot->lat, bot->lon, bot->speed, bot->angle, bot->wr_distance
  );
}

/* disconnect everything */
void robot_sigint(int x) {
  printf("interrupted... stop!\n");
  /* stop all driver */
  exit(x);
}

/* TODO: no type casting */
void robot_value_init_rand(wrbot *bot) {
  double seed = 1.0;
  bot->lat = rand() / (RAND_MAX / seed);
  bot->lon = rand() / (RAND_MAX / seed);
}

void robot_value_init_nearby(wrbot *bot) {
  bot->lat = bot->lat + 0.0000001;
  bot->lon = bot->lon + 0.0000001;
}

/* in case of not using hardware, use mock-up */
void robot_loc_mock_rand(wrbot *bot) {
  double seed = 1.0;
  bot->lat = rand() / (RAND_MAX / seed);
  bot->lon = rand() / (RAND_MAX / seed);
}


int gps_probe(wrbot *bot) {
  int gps_stat = 0;
  printf("probing GPS daemon.. \n");
  if (gps_stat) {
    printf("probe successful. gpsd is ready");
    printf("loading gps value to vehicle...");
    bot->lat = 0.000; /* value from gpsd */
    return 1;
  } else {
    printf("probe failed: gpsd not working, using fallback\n");
    return 0;
  }
}

/* 
   wrbot-cli accept gpsfile, wrconfig and command line args 
   TODO: 
    - add `getopt` for cli usage
    - new data structure & clean up
    - 
 */
int main(void) {
  printf(CYN"wrbot"GRN" v%s "RST "is starting up on" YEL " %s \n"RST, VERSION, PLATFORM);
  wrbot *bot = malloc(sizeof(wrbot));
  // robot_value_init_nearby(bot);
  if (gps_probe(bot)) {
    return EXIT_FAILURE;
  }
  signal(SIGINT, robot_sigint);
  if (!run(bot, "./gps_list.txt")) {
    free(bot);
  } else {
    robot_sigint(1);
  } return EXIT_SUCCESS; 
}
