# BUG-1
## Categary
Logic error

## Description
Everytime the program runs, it will out put the number of input arguments (argc) in the terminal.

## Affected Lines in the original program
`resize.c:10`

## Expected vs Observed
Expect no ambigous output.

## Steps to Reproduce
### command
```shell
./resize solid_epfl.png solid_epfl_resize.png 10
```

## Suggested Fix Description
Just delete the printf line.
