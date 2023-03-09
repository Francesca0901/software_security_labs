# BUG-3
## Categary
Logic error

## Description
When the logo crosses the lower session of the image, the EPFL becomes ETHZ printed at the top of the image. Because we do `epfl[j_logo] += logo_adj[j_logo];` to calculate a new logo.

## Affected Lines in the original program
`epfl.c:82` and `epfl.c:83`

## Expected vs Observed
Expected logo not to be changed, but the output is no longer EPFL but ETHZ.

## Steps to Reproduce
### command
```c
./epfl solid.png solid_epfl.png 10 150 5 fe0002
```

## Suggested Fix Description
We can just delete the change to the structure `epfl[]`.
```c
if (i + size * logo_height >= height) {
        // If we do, move the logo up to the top.
        //epfl[j_logo] += logo_adj[j_logo];
        //i = logo_adj[j_logo] = 0;
        i = 0;
      }
```

