# BUG-4
## Category
String vulnerability or Wrong variables or Logic error

## Description
If we set width to 0, when allocate `struct pixel(*image_data)[img->size_x]`, variable length array bound evaluates to a non-positive value 0, which is not allowed in C, and will cause program to crash.
What's more, it makes no sense to generate a picture with either width or height with 0, so maybe just restrict input with 0.

## Affected Lines in the original program
`solid.c:89` and `solid.c:90`

## Expected vs Observed
We expected both height and width to be positive.

## Reproduce
### command
```shell
./solid solid_0.png 0 10 ffffff
```

## Suggested Fix Description
When determining the legality of width and height, add a judgement that they are not equal to 0.
```c
  unsigned long height = strtol(height_arg, &end_ptr, 10);

  /* If the user provides negative height or the height is 0 and the end_ptr
   * hasn't moved we issue an error and free palette
   */
  if (height >= USHRT_MAX || *end_ptr || height <= 0)
    goto error;

  unsigned long width = strtol(width_arg, &end_ptr, 10);

  if (width >= USHRT_MAX || *end_ptr || width <= 0) {
    goto error;
  }
```