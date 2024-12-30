#!/bin/env python3

"""

Merges and deduplicates NetLog CSV files.

This tool is slow and basic, but gets the job done.

"""

import argparse
import csv
import sys
import os

fieldnames = ['ssid', 'bssid', 'rssi', 'auth', 'coord', 'speed']

# Argparser
parser = argparse.ArgumentParser(
           prog=sys.argv[0],
           description='Merges and deduplicates NetLog CSV files.',
           epilog='This tool is slow and basic, but gets the job done.')

parser.add_argument('files', nargs='+')
parser.add_argument('-o', '--output')
parser.add_argument('-v', '--verbose', action='count')

args = parser.parse_args()

if not args.output:
    print(f'{sys.argv[0]}: output file not specified')
    sys.exit(1)

# Read and parse input files
print(f'Parsing {len(args.files)} files...')

all_rows = []

for filename in args.files:
    if not os.path.isfile(filename):
        print(f'{sys.argv[0]}: {filename}: not a file')
        sys.exit(1)

    with open(filename, 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',',
                                quotechar='"', lineterminator='\n')
        
        for row in reader:
            all_rows.append(row)

# Get BSSIDs from rows
all_row_bssids = []

for row in all_rows:
    all_row_bssids.append(row['bssid'])

# Search for duplicates
deduped_rows = []
duplicates = 0

print(f'Merging {len(all_rows)} networks...')

for i, row in enumerate(all_rows):
    if row['bssid'] in all_row_bssids[i + 1:]:

        # Print info if --verbose specified
        if args.verbose:
            print(f'\033[2K\r{row["bssid"]} ({row["ssid"]}) is a duplicate')
        
        duplicates += 1
    else:
        deduped_rows.append(row)

    if args.verbose:
        print(end=f'\r\033[1mMerging networks... {i}/{len(all_rows)}, {duplicates} '
              'duplicate(s) found\033[22m',
              flush=True)

if args.verbose:
    print('\n\nWriting networks to output...')

# Write lines to output CSV file
with open(args.output, 'w', newline='') as csvfile:
    writer = csv.DictWriter(csvfile, delimiter=',',
                        quotechar='"', quoting=csv.QUOTE_ALL,
                        lineterminator='\n', fieldnames=fieldnames)
    
    writer.writeheader()
    writer.writerows(deduped_rows)

# Print info
print(f'\nMerged {len(all_rows)} networks, of which {duplicates} are duplicate(s).')
print(f'\033[1mTotal: {len(all_rows) - duplicates} networks.\033[22m')
