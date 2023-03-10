# BUG-3
## Categary
Arithmetic overflow

## Description
When the radius\center_x\center_y are too large, the calculation `radius * radius` or `center_x + radius` or `center_y + radius` will overflow the maximun value which int can contain, and make the program crash.

## Affected Lines in the original program
`circle.c:52` and `circle.c:54` and `circle.c:62` and `circle.c:75` and `circle.c:77` and `circle.c:85`

## Expected vs Observed
Expect program run smoothly but observed segment fault when the arguments are too large.

## Steps to Reproduce
### command
```shell
./circle solid.png solid_circle.png 2147483647 2147483647 2147483647 ff0000
```

## Suggested Fix Description
Forced conversion of variable types, from int to long long type, to allow larger calculation (temporary) outcome.
```c
for (int x = center_x - radius; x <= 1LL * center_x + radius; x++) {
    ...
    int y = round(center_y + 
                  sqrt(1LL * radius * radius - 1LL * (x - center_x) * (x - center_x)));
    ...
```