# BUG-0
## Category
Logic error

## Description
The original code forgot to do `return 1` when user doesn't input 7 arguments. So after `printf "Usage: ..."`, the program keeps running, and finally gives segmentation fault.

## Affected Lines in the original program
`epfl.c:17`

## Expected vs Observed
### Expected
no segmentation fault
### observed
exist segmentation fault
![[pictures/segmentfault.png]]

## Steps to Reproduce
### command
```
./epfl aha.png aha_epfl.png 10 10
```

## Suggested Fix Description
To fix this bug, put a `return statement` when the number of argument doesn't match `7`.
```c
if (argc != 7) {
    printf("Usage: %s input_image output_image top_left_x top_left_y size "
           "hex_color\n",
           argv[0]);
    return 1; //bug_0
  }
```

### Current Output
![[pictures/no_segfault.png]]