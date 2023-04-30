extern "C" {
#include "pngparser.h"
}
#include <stdio.h>

#include "png_mutator.h"
#include "yolo_png_mutator.h"

// LibFuzzer stub
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {

  struct image *test_img = NULL;

  FILE *input = fopen("testfile.png","w");
  fwrite(data, size, 1, input);
  fclose(input);

  struct pixel pixels[256];
  for(int i = 0; i < 256; i++){
      pixels[i].red = 0xff;
      pixels[i].blue = 0xff;
      pixels[i].green = 0xff;
  }
  // struct pixel pixel;
  // pixel.red = 0xff;
  // pixel.blue = 0xff;
  // pixel.green = 0xff;

  // What would happen if we run multiple fuzzing processes at the same time?
  // Take a look at the name of the file.
  if (load_png("testfile.png", &test_img) == 0){
    // printf("in test\n");
    store_png("storedfile.png", test_img, pixels, 256);

    if (test_img){
      if (test_img->px)
        free(test_img->px);
      free(test_img);
    }
  }


  // Always return 0
  return 0;
}