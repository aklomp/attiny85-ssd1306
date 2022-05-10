#!/usr/bin/env python

import argparse
import png

# Parse the filename argument from the command line.
parser = argparse.ArgumentParser(description='Convert a PNG image for the SSD1306.')
parser.add_argument('file', nargs=1, help='Path to the PNG image file.')
args = parser.parse_args()

# Read file.
r = png.Reader(args.file[0])
img = r.read()

# Write image.
print("const uint8_t PROGMEM img[] = {", end='')
rows = list(img[2]);

# Flip row order.
for y in range(len(rows) // 8 - 1, -1, -1):
    for col in range(len(rows[0])):
        pack = 0
        for row in range(8):
            pack |= (rows[y * 8 + row][col] << row);
        if not col % 8:
            print("\n\t", end='')
        print("%s0x%02x," % (' ' if (col % 8) else '', 255 - pack), end='')
print("\n};")
