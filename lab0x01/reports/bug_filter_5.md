# BUG-5
## Category
heap overflow

## Description
After calling malloc we must check if it was successful, but after the code allocate *neg, this is no check for malloc function.

## Affected Lines in the original program
`filter.c:107` and `filter.c:122`

## Expected vs Observed
We expected a check of success after malloc memory for neg, but there is no check procedure. 

## Suggested Fix Description
After allocate memory by using `get_pixel` function, we should check whether the allocation is successful or not.
```c
struct pixel *neg = get_pixel();
if(!neg) {
    free(img->px);
    free(img);
    return;
}
```