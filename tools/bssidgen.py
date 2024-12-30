#!/bin/env python3

"""

Generates a BSSID text file from a NetLog CSV file.

This tool is useful if you only have a NetLog CSV file,
but still want to load it onto your NetLog MK1 device.

"""

import csv
import argparse
import sys
import os

bssid_field = 'bssid'

# Argparser
parser = argparse.ArgumentParser(
           prog=sys.argv[0],
           description='Generates a BSSID text file from a NetLog CSV file.',
           formatter_class=argparse.RawDescriptionHelpFormatter,
           epilog='This tool is useful if you only have a NetLog CSV file,\n'
                  'but still want to load it onto your NetLog MK1 device.')

parser.add_argument('infile')
parser.add_argument('outfile')

args = parser.parse_args()

# Make sure infile exists
if not os.path.isfile(args.infile):
    print(f'{sys.argv[0]}: {args.infile}: not a file')
    sys.exit(1)

# Read and parse infile
infile_rows = []

with open(args.infile, 'r', newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=',',
                        quotechar='"', lineterminator='\n')

    for i, row in enumerate(reader):
        if i == 0:
            infile_header = row
        else:
            infile_rows.append(row)

# Get index of BSSID column
header_index = 0

if bssid_field not in infile_header:
    print(f"{sys.argv[0]}: {args.infile} does not contain '{bssid_field}' header.")
    sys.exit(1)
else:
    header_index = infile_header.index(bssid_field)

# Write BSSIDs to outfile
with open(args.outfile, 'w') as fw:
    for row in infile_rows:
        fw.write(row[header_index] + '\n')

