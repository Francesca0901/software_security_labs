# BUG-8
## Category
Buffer overflow

## Description
The `printf()` function with %s format specifier expects a null-terminated string as input. If the input variable is longer than the buffer allocated for it, the `printf()` function could write beyond the end of the buffer, causing a buffer overflow. 

## Affected Lines in the original program
`filter.c:233`

## Suggested Fix Description
Just delete the printf() please.
```c
if (load_png(input, &img)) {
  //printf("%s", input);  
  printf(" PNG file cannot be loaded\n");
  exit(1);
}
```