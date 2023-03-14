# BUG-6

## Category
Temporal safety violation

## Description
The `free()` function expects a pointer to memory that was allocated on the heap with `malloc()` or a related function. Passing a pointer to a static array can result in undefined behavior.
So in filter, when we pass in an `img` there is no need to free the `img` and `img->px`, because it's on the stack, and will be free automatically after program finishes.

## Affected Lines in the original program
`filter.c:101`

## Reproduce
Like the tests.c, when we pass in an `img` with `img->px` being an array on the stack, and try to free img->px, program will encounter segfault.

## Suggested Fix Description
Just delete the free(img->px) function.