# BUG-2
## Categary
Unchecked return code from system call

## Description
When call store_png function, didn't check if it's successful stored.

## Affected Lines in the original program
`rect.c:85

## Expected vs Observed
Expect check for `store_png`.

## Suggested Fix Description
Check if `store_png` is successful or not.
```c
if (store_png(output, img, NULL, 0)) {
    printf("Usage: %s input_image output_image top_left_x top_left_y "
           "bottom_right_x bottom_right_y hex_color\n",
           argv[0]);
    free(img->px);
    free(img);
    return 1;
}
```
