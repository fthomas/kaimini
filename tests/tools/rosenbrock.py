#!/usr/bin/env python

# http://en.wikipedia.org/wiki/Rosenbrock_function
# f(x, y) = (1 - x)^2 + 100 (y - x^2)^2

import sys

x = y = 0.

if len(sys.argv) > 2:
    x = float(sys.argv[1])
    y = float(sys.argv[2])

f = (1 - x)**2 + 100. * (y - x**2)**2
print f
