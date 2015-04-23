#!/bin/bash

# Define trace path
TRACE="traces/traces-short/tr1"
# TRACE="traces/traces-1M/bzip2.gz"

make
# Runs TR1 trace through simulation under default settings
zcat $TRACE | ./cacheSim.exe

# Print final contents of cache
cat cacheResults.results

# Print final results of cache
# cat resultsFile.results
