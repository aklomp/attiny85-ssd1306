#!/usr/bin/env python

import sys
import png

# Get argument:
if len(sys.argv) == 1:
    print("Usage: %s [file]" % sys.argv[0])
    exit()

# Read file:
r = png.Reader(sys.argv[1])
img = r.read()

# Write image:
sys.stdout.write("const uint8_t PROGMEM img[] = {")
rows = list(img[2]);

# Flip row order:
for y in xrange(len(rows) / 8 - 1, -1, -1):
    for col in xrange(len(rows[0])):
        pack = 0
        for row in xrange(8):
            pack |= (rows[y * 8 + row][col] << row);
        if not col % 8:
            sys.stdout.write("\n\t")
        sys.stdout.write("%s0x%02x," % (' ' if (col % 8) else '', 255 - pack))
print
print("};")
