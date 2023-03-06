# BUG-2
## Category
Iteration errors

## Description
The program doesn't exert boundary check when inserting data into array `image_data[][]`, when user input a very large size number (when `size*5 > height`), the sum of `i` and `i_logo` will exceed `height`, which means the program wanna write below the bottom of the picture.

## Affected Lines in the original program
`epfl.c:87`

## Expected vs Observed
When `size*5 > height` we observed program crash with `malloc(): corrupted top size`

## Steps to Reproduce
### command
Assume we have a solid.png with size 100 * 100. We want to draw a EPFL logo with size 25.
```shell
./epfl solid.png solid_epfl.png 0 0 25 fe0002
```

### Proof-of-Concept Input (if needed)
/pictures/solid.png

## Suggested Fix Description
Modify the condition check in `for` loop:
```c
for (int i_logo = 0; i_logo < size * logo_height && (i + i_logo) < height; i_logo++)
```
so that `i + i_logo` won't go beyond the boundary of `image_data[][]`.