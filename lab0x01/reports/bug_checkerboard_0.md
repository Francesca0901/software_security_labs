# BUG-0
## Categary
Iteration errors

## Description
When the length and width of the input are not divisible by square_width, the procedure of filling `image_data[square_top_left_y + y][square_top_left_x + x]` will iterate until `square_top_left_x + x` or `square_top_left_y + y` go out of boundary.

## Affected Lines in the original program
`checkerboard.c:113` and `checkerboard.c:114`

## Expected vs Observed
I think the program expect the square go out of bounds to be ignored, but observed segmental fault.

## Steps to Reproduce
### command
```shell
./checkerboard check.png 200 100 8 ffffff ff0000
```

## Suggested Fix Description
When iterating over a square and filling it with the correct color, check we don't reach out of bounds:
```c
    for (int x = 0; x < square_width && (square_top_left_x + x) < width; x++) {
        for (int y = 0; y < square_width && (square_top_left_y + y) < height; y++) {
```