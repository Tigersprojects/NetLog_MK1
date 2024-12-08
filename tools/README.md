# Tools
This directory contains some miscellaneous tools for processing files.

Before using the tools, you may need to run `chmod +x <toolname>`.

These are very quick and dirty, but get the job done.

## `netmerger.py`
Usage: `python3 netmerger.py [-h] [-o OUTPUT] [-v] files [files ...]`

Example: `python3 netmerger.py networks.csv morenetworks.csv foo/*.csv -o allnetworks.csv`

Description: Merges and deduplicates NetLog CSV files.

## `bssidgen.py`
Usage: `python3 bssidgen.py [-h] infile outfile`

Example: `python3 bssidgen.py mynetworks.csv NetLog_bssids.txt`

Description: Generates a BSSID text file from a NetLog CSV file.