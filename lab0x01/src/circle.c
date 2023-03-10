#include "pngparser.h"
#include <math.h>
#include <string.h>

int main(int argc, char *argv[]) {
  struct image *img;

  /* Check if the number of arguments is correct */
  if (argc != 7) {
    printf("Usage: %s input_image output_image center_x center_y radius "
           "hex_color\n",
           argv[0]);
    return 1;
  }

  /* Rename arguments for easier reference */
  char *input = argv[1];
  char *output = argv[2];

  /* Decode the center of the circle. Invalid values are decoded as 0 */
  int center_x = atoi(argv[3]);
  int center_y = atoi(argv[4]);

  /* Invalid radius will just be interpreted as 0 */
  int radius = atoi(argv[5]);

  // if (radius < 0 || center_x < 0 || center_y < 0) {
  //   return 1;
  // }
  if (radius < 0) {
    radius = 0;
  }

  /* Invalid color will be interpreted as black */
  char *end_ptr;
  long hex_color = strtol(argv[6], &end_ptr, 16); //BUG-0
  if (*end_ptr || strlen(argv[6]) != 6 || hex_color < 0) {
    hex_color = 0;
  }

  if (load_png(input, &img)) {
    return 1;
  }

  unsigned height = img->size_y;
  unsigned width = img->size_x;

  struct pixel(*image_data)[width] = (struct pixel(*)[width])img->px;

  /* We will iterate through all the x coordinate values in the pixel and
   * calculate the y values for the pixels. Every circle has two points
   * corresponding to every x coordinate.
   *
   * The coordinates were obtained by solving the equation:
   * (x - center_x)^2 + (y - center_y)^2 = radius^2
   *
   * A radius of 0 means a single pixel in the center
   */
  for (int x = center_x - radius; x <= 1LL * center_x + radius; x++) {
    if (x < 0) {
      x = 0;
      continue;
    }
    if (x >= width){
      break;
    }
    int y = round(center_y + 
                  sqrt(1LL * radius * radius - 1LL * (x - center_x) * (x - center_x)));
    if(y < 0 || y >= height) continue;

    image_data[y][x].red = (hex_color & 0xff0000) >> 16;
    image_data[y][x].green = (hex_color & 0x00ff00) >> 8;
    image_data[y][x].blue = (hex_color & 0x0000ff);
    image_data[y][x].alpha = 0xff;

    y = round(center_y -
               sqrt(1LL * radius * radius - 1LL * (x - center_x) * (x - center_x)));
    if(y < 0 || y >= height) continue;

    image_data[y][x].red = (hex_color & 0xff0000) >> 16;
    image_data[y][x].green = (hex_color & 0x00ff00) >> 8;
    image_data[y][x].blue = (hex_color & 0x0000ff);
    image_data[y][x].alpha = 0xff;
  }

  /* There are going to be some ugly gaps in the image, so we will repeat the
   * procedure for the y axis.
   *
   * In practice a more efficient rasterization algorithm is used.
   */
  for (int y = center_y - radius; y <= 1LL * center_y + radius; y++) {
    if (y < 0) {
      y = 0;
      continue;
    }
    if (y >= height){
      break;
    }
    if (y < 0 || y >= width) continue;
    int x = round(center_x +
                  sqrt(1LL * radius * radius -  1LL * (y - center_y) * (y - center_y)));
    if (x < 0 || x >= width) continue;

    image_data[y][x].red = (hex_color & 0xff0000) >> 16;
    image_data[y][x].green = (hex_color & 0x00ff00) >> 8;
    image_data[y][x].blue = (hex_color & 0x0000ff);
    image_data[y][x].alpha = 0xff;

    x = round(center_x -
               sqrt(1LL * radius * radius - 1LL * (y - center_y) * (y - center_y)));
    if (x < 0 || x >= width) continue;

    image_data[y][x].red = (hex_color & 0xff0000) >> 16;
    image_data[y][x].green = (hex_color & 0x00ff00) >> 8;
    image_data[y][x].blue = (hex_color & 0x0000ff);
    image_data[y][x].alpha = 0xff;
  }

  store_png(output, img, NULL, 0);
  free(img->px);
  free(img);
  return 0;
}
