# BUG-6
## Category
memory leak

## Description
in filter_blur, if malloc of new_data is not successful, the program return without free `img` and `img->px`.

## Affected Lines in the original program
`filter.c:60`

## Expected vs Observed
We expect every memory allocated to be freed.

## Suggested Fix Description
If new_data is not successful allocated, first free `img`, then return.
```c
  if (!new_data) {
    free(img->px);
    free(img);
    return;
  }
```