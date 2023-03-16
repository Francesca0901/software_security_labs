# BUG-2
## Categary
Arithmetic overflow/underflow

## Description
for the arithmetic
```c
for (int i = 0; i < (height + square_width - 1) / square_width; i++) {
      for (int j = 0; j < (width + square_width - 1) / square_width; j++) {
```
If `square_width` is the max number of long, `height + square_width - 1` and `width + square_width - 1` might overflow long type.
And `int square_top_left_x = j * square_width;` might also give number overlow `int` type.

## Affected Lines in the original program
`checkerboard.c:`

## Expected vs Observed
Expect no arithmetic overflow, the result of `j * square_width` and `width + square_width - 1` should be within type restriction.

## Steps to Reproduce
### command
```shell
./checkerboard out.png 65535 65535 9223372036854775807 ffffff 000000
```

## Suggested Fix Description
Just restrict square_width to USHRT_MAX as same as width and height.
```c
if (square_width <= 0 || square_width >= USHRT_MAX || *end_ptr) {
    goto error;
}
```
