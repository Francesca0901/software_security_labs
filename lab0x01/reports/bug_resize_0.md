# BUG-0
## Categary
Buffer overflow

## Description
As n_pixels is the total number of pixels required for the new image, so we should allocate `n_pixels * sizeof(struct pixel)` space to image->px, but the original program only allocates `n_pixels + sizeof(struct pixel)`

## Affected Lines in the original program
`resize.c:48`

## Expected vs Observed
Expected n_pixels * sizeof(struct pixel) size to be allocated to image->px, observed buffer overflow and the program crashed.

## Steps to Reproduce
### command
```shell
./resize solid.png solid_resize.png 1
```

## Suggested Fix Description
change the malloc size to n_pixels * sizeof(struct pixel)
```c
 new_img->px = malloc(n_pixels * sizeof(struct pixel));
```