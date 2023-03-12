# BUG-3
## Category
Memory leak

## Description
Use `struct pixel *neg = get_pixel();` to allocate a pointer neg but didn't free it in the end.

## Affected Lines in the original program
`filter.c:122`

## Expected vs Observed
Expected all pointer to be freed but `*neg` is never freed.

## Suggested Fix Description
free *neg
```c
  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      struct pixel current = image_data[i][j];
      struct pixel *neg = get_pixel();

      ...

      free(neg); //bug 3
    }
  }
```