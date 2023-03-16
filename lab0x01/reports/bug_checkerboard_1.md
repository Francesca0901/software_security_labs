# BUG-1
## Categary
Temporal safety violation

## Description
if `img->px = malloc(sizeof(struct pixel) * n_pixels);` fails, img will be double freed, because the goto command also lead to a `free(img)` function.

## Affected Lines in the original program
`checkerboard.c:91`

## Expected vs Observed
Expect only one time free.

## Suggested Fix Description
delete the first free function 
```c
if (!img->px) {
    //free(img);  ////bug-1 don't free!!!!!!
    goto error_img;
  }
```