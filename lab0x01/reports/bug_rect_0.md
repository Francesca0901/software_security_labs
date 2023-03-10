# BUG-0
## Categary
Wrong variables

## Description
The `hex_color` is the 8 argv so we should find it in `argv[7]`, argv[8] will reach out of bound.

## Affected Lines in the original program
`rect.c:34` and `rect:35`

## Expected vs Observed
Expected obtain hex_color by searching argv[7].

## Steps to Reproduce
### command
```shell
./rect solid.png solid_rect.png 10 10 50 50 ff0000
```

## Suggested Fix Description
Change argv[8] to argv[7] and get the right value of hex_color.
```c
  long hex_color = strtol(argv[7], &end_ptr, 16);
  if (*end_ptr || strlen(argv[7]) != 6 || hex_color < 0) {
    hex_color = 0;
  }
```