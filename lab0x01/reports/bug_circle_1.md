# BUG-0
## Categary
Wrong operators/variables

## Description
We want to abtain the coordinates of y by solving the equation `(x - center_x)^2 + (y - center_y)^2 = radius^2`, to successfully draw a color on the picture, we need to set y to the value we obtained, but in this code we are using `==`, which compares two expression instead of setting value. Thus, part of the picture can't be coloured.

## Affected Lines in the original program
`circle.c:61` and `circle.c:84`

## Expected vs Observed
We expect to get a complete circle, but only got an incomplete one.

### Expected outcome
/pictures/solid_circle_before.png

### Observed outcome
/pictures/solid_circle_after.png

## Steps to Reproduce
### command
```c
./circle solid.png solid_circle.png 10 10 10 ff0000
```

### Proof-of-Concept Input (if needed)
/pictures/solid.png

## Suggested Fix Description
By changing the operator from `==` to `=`, we can successfully assign values to y and x, and then fix the problem.
```c
y = round(center_y - sqrt(radius * radius - (x - center_x) * (x - center_x)));
x = round(center_x - sqrt(radius * radius - (y - center_y) * (y - center_y)));
```