# BUG-1
## Categary
Logic error

## Description
When traversing to find the pixels inside the rectangle, after each pixel is coloured, raise both i and j.

## Affected Lines in the original program
`rect.c:63` to `rect.c:81

## Expected vs Observed
Expected a rectangle, but actually only draw a line.

## Steps to Reproduce
### command
any command
```shell
./rect solid.png solid_rect.png 10 10 50 50 ff0000
```

## Suggested Fix Description
Modifying loop conditions and loop layers
```c
  while (i < height) {
    if (i >= top_left_y && i <= bottom_right_y) {
      while (j < width) {
        if (j >= top_left_x && j <= bottom_right_x){
          image_data[i][j].red = (hex_color & 0xff0000) >> 16;
          image_data[i][j].green = (hex_color & 0x00ff00) >> 8;
          image_data[i][j].blue = (hex_color & 0x0000ff);
          image_data[i][j].alpha = 0xff;
        }
        j++;
      }
    }
    j = 0;
    i++;
  }
  ```
