#!/usr/bin/env python

# http://en.wikipedia.org/wiki/Himmelblau%27s_function
# f(x, y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2

import sys

x = y = 0.

if len(sys.argv) > 2:
    x = float(sys.argv[1])
    y = float(sys.argv[2])

f = (x**2 + y - 11)**2 + (x + y**2 - 7)**2
print f
