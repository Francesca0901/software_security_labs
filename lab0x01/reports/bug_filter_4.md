# BUG-4
## Category
String vulnerability

## Description
The `input` variable is passed as a format string argument to printf() without any format specifier to indicate the expected type of argument. 

## Affected Lines in the original program
`filter.c:233`

## Expected vs Observed
Expected a format specifier(namely `%s`) to indicate the expected type.

## Suggested Fix Description
Use `printf("%s", input);`