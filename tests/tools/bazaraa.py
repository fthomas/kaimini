#!/usr/bin/env python

# http://imo.rz.tu-bs.de/~degbers/konvdisk08/akd03.pdf
# f(x, y) = (x - 2)^4 + (x - 2*y)^2

import sys

x = y = 0.

if len(sys.argv) > 2:
    x = float(sys.argv[1])
    y = float(sys.argv[2])

f = (x - 2)**4 + (x - 2*y)**2
print f
