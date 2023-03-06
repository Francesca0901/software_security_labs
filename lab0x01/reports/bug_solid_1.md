# BUG-1
## Category
memory leak

## Description
If the program excuted without error, it only free `img` and `img->px`, and doesn't free `palette` .

## Affected Lines in the original program
`solid.c:107` and `solid.c:108`

## Expected vs Observed
We expected palette to be freed but it's not freed.

## Suggested Fix Description
When the program free `img->px` and `img`, we free `palette` together:
```c
  free(palette); //free palette
  free(img->px);
  free(img);
```