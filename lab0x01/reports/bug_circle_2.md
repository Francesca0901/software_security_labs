# BUG-2
## Categary
Iteration errors

## Description
When painting, both x and y can go out of bound. But we didn't do the boundary check.

## Affected Lines in the original program
`circle.c:53` and `circle.c:61` and `circle.c:76` and `circle.c:84`

## Expected vs Observed
Expected to see seccessful circle, but actually see segment fault. Because `image_data[y][x]` can reach out of bound.

## Steps to Reproduce
### command
```shell
./circle solid.png solid_circle.png 50 50 50 ff0000
```

## Suggested Fix Description
Whenever finish calculation for y and x, check if it's still within boundary.



