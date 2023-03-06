# BUG-0
## Category
heap overflow

## Description
As the annotation says 'After calling malloc we must check if it was successful', but after the code allocate palette, this is no check for malloc function.

## Affected Lines in the original program
`solid.c:16`

## Expected vs Observed
We expected a check of success after malloc space for palette, but there is no check procedure. 

## Suggested Fix Description
After the code 
```c
struct pixel *palette = allocate_palette();
```
we should add the check for `palette`
```c
if (!palette) {
    goto error_mem;
  }
```
If the palette is not successfully malloc, the program go to error_mem and print Couldn't allocate memory"