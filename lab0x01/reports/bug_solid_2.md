# BUG-2
## Category
memory leak

## Description
If the program encounter any problem related to `img` or `img->px`'s memory allocate, then it goto `error_px` or `error_img` or `error_mem`, but in these cases it won't free `palette`.

## Affected Lines in the original program
`solid.c:143` and `solid.c:144`

## Expected vs Observed
We expected palette to be free but it's not free.

## Suggested Fix Description
In the error handling code, we check if palette exist, if it hasn't been freed, we free it.
I do the check because in the previous bug report, I added the malloc check for palette, if it's not successfully malloced, program also `goto error_mem`, in this case we don't need to free palette.
```c
error_px:
  free(img->px);
error_img:
  free(img);
error_mem:
  if(palette) free(palette); //free palette
  printf("Couldn't allocate memory\n");
  return 1;
```