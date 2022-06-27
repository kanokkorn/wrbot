#include "main.h"

int run(wrbot *bot) {
  const char *fname = "gps_list.txt";
  char *gps_line = NULL;
  size_t len = 0;
  ssize_t read;
  static int round = 0;
  FILE *gps_data = fopen(fname, "r");
  printf("file name is -> %s\n", fname);
  if (!gps_data) {
    printf("File opening failed\n");
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&gps_line, &len, gps_data)) != -1) {
    printf("round #%d\n", round);
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
  double wr_distance = PLACEHOLDER;
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
    printf("distance to destination -> %f meters\n", wr_distance);
    robot_loc_mock(bot);
    robot_status(bot);
    sleep(1);
  }
  printf("=== arrived ===\n");
  printf("doing task\n");
  /* task */
  sleep(20);
  printf("done...\n");
  sleep(5);
}

double haversine(wrbot *bot, double lat_des, double lon_des) {
  double c;
  double lat_int_rad = degToRad(bot->lat), lat_des_rad = degToRad(lat_des);
  double lat_delta = degToRad(lat_des - bot->lat),
         lon_delta = degToRad(lon_des - bot->lon);
  double a = pow(sin(lat_delta / 2), 2) +
             cos(lat_int_rad) * cos(lat_des_rad) * pow(lon_delta / 2, 2);
  return (c = (2 * atan2(sqrt(a), sqrt(1 - a)))) != 0 ? EARTH_RAD * c : 0;
}

void robot_status(wrbot *bot) {
  printf("\e[1;1H\e[2J");
  printf("<-- status -->\n");
  printf("current lat  : %f \n", bot->lat);
  printf("current lon  : %f \n", bot->lon);
  printf("current speed: %f \n", bot->speed);
  printf("current angle: %f \n", bot->angle);
  printf("distance to destination -> %f meters\n", bot->wr_distance);
}

void robot_sigint(int x) {
  printf("interrupted... stop!\n");
  /* stop all driver */
  exit(x);
}

void robot_value_init(wrbot *bot) {
  float seed = 1.0;
  bot->lat = (float)rand() / (float)(RAND_MAX / seed);
  bot->lon = (float)rand() / (float)(RAND_MAX / seed);
}

void robot_loc_mock(wrbot *bot) {
  float seed = 1.0;
  bot->lat = (float)rand() / (float)(RAND_MAX / seed);
  bot->lon = (float)rand() / (float)(RAND_MAX / seed);
}

int main(void) {
  wrbot *bot = malloc(sizeof(wrbot));
  robot_value_init(bot);
  signal(SIGINT, robot_sigint);
  run(bot) == 0 ? free(bot) : robot_sigint(1);
  return 0;
}
