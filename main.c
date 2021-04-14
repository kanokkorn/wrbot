#include "main.h"

int main(void) {
  wrbot *bot = malloc(sizeof(wrbot));
  robot_value_init(bot);
  run(bot);
  return 0;
}

void run(wrbot *bot) {
  const char* fname = "gps_list.txt";
  char* gps_line = NULL;
  size_t len = 0;
  ssize_t read;
  static int round = 0;
  FILE* gps_data = fopen(fname, "r");
  printf("file name is -> %s\n", fname);
  if (!gps_data) {
    perror("File opening failed");
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&gps_line, &len, gps_data)) != -1) {
    printf("round #%d\n", round);
    compute(bot, gps_line);
    round++;
  }
  if (ferror(gps_data)) {
    puts("I/O error while reading");
  } else if (feof(gps_data)) {
    puts("End of file");
  }
  fclose(gps_data);
  if (gps_line) {
    free(gps_line);
  }
}

void compute(wrbot *bot, char* pos) {
  int x = 0;
  double distance = 10.0000;
  char* gps_array[2];
  char* pos_tok = strtok(pos, ",");
  while (pos_tok) {
    gps_array[x++] = pos_tok;
    pos_tok = strtok(NULL, " ");
  }
  while (distance >= tolerance_value) {
    distance = haversine(bot, atof(gps_array[0]), atof(gps_array[1]));
    bot->distance = distance;
    printf("distance to destination -> %f meters\n", distance);
    robot_loc_mock(bot);
    robot_status(bot);
    sleep(1);
  }
  printf("=== arrived ===\n");
  puts("doing task");
  /**
   * TODO: add task
   */
  sleep(20);
  puts("done...");
  sleep(5);
}

double haversine(wrbot *bot, double lat_des, double lon_des) {
  double c;
  double lat_int_rad = degToRad(bot->lat), lat_des_rad = degToRad(lat_des);
  double lat_delta = degToRad(lat_des - bot->lat), lon_delta = degToRad(lon_des - bot->lon);
  double a = pow(sin(lat_delta / 2), 2) + cos(lat_int_rad) * cos(lat_des_rad) * pow(lon_delta / 2, 2);
  return (c = ( 2 * atan2(sqrt(a), sqrt(1-a)))) != 0 ? earth_rad * c : 0;
}

void robot_status(wrbot *bot) {
  printf("<-- status -->\n");
  printf("current lat  : %f \n", bot->lat);
  printf("current lon  : %f \n", bot->lon);
  printf("current speed: %f \n", bot->speed);
  printf("current angle: %f \n", bot->angle);
}

void robot_value_init(wrbot *bot) {
  bot->lat = 10.728520;
  bot->lon = 99.383200;
}

void robot_loc_mock(wrbot *bot) {
  bot->lat += 0.000001;
  bot->lon += 0.000001;
}

void robot_failsafe(double distance) {
}
