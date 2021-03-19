#include "main.h"

int main(void) {
  run();
  return 0;
}

void run(void) {
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
    compute(gps_line);
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

void compute(char* pos) {
  int x = 0;
  double mock_lat = 0.000, mock_lon = 0.000, distance = 10.0000;
  char* gps_array[2];
  char* pos_tok = strtok(pos, ",");
  while (pos_tok) {
    gps_array[x++] = pos_tok;
    pos_tok = strtok(NULL, " ");
  }
  while (distance >= tolerance_value) {
    distance = haversine(mock_lat, mock_lon, atof(gps_array[0]), atof(gps_array[1]));
    mock_lat++; mock_lon++;
    printf("distance to destination -> %f meters\n", distance);
  }
  /**
   * task
   */
  printf("arrived\n");
}

double haversine(double lat_int, double lon_int, double lat_des, double lon_des) {
  double c;
  double lat_int_rad = degToRad(lat_int), lat_des_rad = degToRad(lat_des);
  double lat_delta = degToRad(lat_des - lat_int), lon_delta = degToRad(lon_des - lon_int);
  double a = pow(sin(lat_delta / 2), 2) + cos(lat_int_rad) * cos(lat_des_rad) * pow(lon_delta / 2, 2);
  return (c = ( 2 * atan2(sqrt(a), sqrt(1-a)))) != 0 ? earth_rad * c : 0;
}
