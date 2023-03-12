# BUG-1
## Category
Local persisting pointers

## Description
`get_pixel` creates a `pixel` on the **stack** and returns a pointer to it. However, when the function returns, the struct pixel is deallocated, and the memory it used is freed. Therefore, the returned pointer will points to invalid memory.

## Affected Lines in the original program
`filter.c:108` and `filter.c:109`

## Expected vs Observed
Expected a pointer point to memory which can be access outside the function `get_pixel`, so the memory should be dynamic allocated.

## Steps to Reproduce
### Command
```shell
./filter solid_epfl.png solid_epfl_filter.png negative
```

## Suggested Fix Description
