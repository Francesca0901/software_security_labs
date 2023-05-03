extern "C" {
#include "pngparser.h"
}
#include "png_mutator.h"
#include "yolo_png_mutator.h"
#include <stdio.h>
#include <string.h>

// LibFuzzer stub
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {

  struct image *test_img = NULL;

  char *filename = (char *)malloc(size + 1);
  if (!filename) {
    return 0;
  }

  memcpy(filename, data, size);
  filename[size] = '\0';

  // What would happen if we run multiple fuzzing processes at the same time?
  // Take a look at the name of the file.
  if (load_png(filename, &test_img) == 0) {
    if (test_img) {
      if (test_img->px)
        free(test_img->px);
      free(test_img);
    }
  }

  free(filename);

  // Always return 0
  return 0;
}