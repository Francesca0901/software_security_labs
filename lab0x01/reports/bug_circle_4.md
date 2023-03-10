# BUG-4
## Categary
Logic error

## Description
The radius can be value below 0, which is not a valid radius in math. 

## Affected Lines in the original program
`circle.c:25`

## Expected vs Observed
Expect a valid radius, but now accept invalid radius.

## Steps to Reproduce
### command
```shell
./circle solid.png solid_circle.png 10 10 -1 ff0000
```

## Suggested Fix Description
When the radius is below 0, set radius as 0;
```c
if (radius < 0) {
    radius = 0;
}
```