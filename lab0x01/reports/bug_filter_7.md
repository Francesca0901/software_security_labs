# BUG-7
## Category
Lojic error

## Description
When calculate the average, the method used to calculate the number of pixels didn't take into account that edge pixels is not influenced by `(2 * radius + 1) * (2 * radius + 1)` pixels. We should calculate number of pixels everytime to avoid this situation.

## Affected Lines in the original program
`filter.c:86`

## Expected vs Observed
We expect when handle the pixels on the edge, don't use (2 * radius + 1) * (2 * radius + 1) as the pixels number.

## Reproduce
The example is given in tests.c file.

## Suggested Fix Description
When calculate the sum of red, blue, green of the pixels around, increase number of pixels by 1.
```c
    int num_pixels = 0;
      /* We iterate over all pixels in the square */
      for (long y_offset = -radius; y_offset <= radius; y_offset++) {
        for (long x_offset = -radius; x_offset <= radius; x_offset++) {

          /* BUG!
           * This bug isn't graded.
           *
           * FIX: Limit reads only to valid memory
           */
          if((i + y_offset) < 0 || (i + y_offset) >= img->size_y || (j + x_offset) < 0 || (j + x_offset) >= img->size_x) {
            continue;
          }

          struct pixel current = image_data[i + y_offset][j + x_offset];

          red += current.red;
          blue += current.blue;
          green += current.green;
          alpha += current.alpha;
          num_pixels += 1;
        }
      }
```