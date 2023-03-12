# BUG-2
## Category
Iteration errors

## Description
Using i <= img->size_y will create an array overrun when reading image_data[i][j].

## Affected Lines in the original program
`filter.c:118` and `filter.c:119`

## Expected vs Observed
Expect reasonable access to arrays.

## Steps to Reproduce
### Command
```shell
./filter solid_epfl.png solid_epfl_filter.png negative
```

## Suggested Fix Description
Use `<` instead of `<=` in iteration
```c
 for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {
      ...
    }
  }
```