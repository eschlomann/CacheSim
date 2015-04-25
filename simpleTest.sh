#!/bin/bash

# Define trace path
# TRACE="traces/traces-short/tr6"
TRACE="traces/traces-1M/sjeng.gz"

# Define config
CONFIG="configs/Default"
# CONFIG="configs/All-2way"

make
# Runs TR1 trace through simulation under default settings
zcat $TRACE | ./cacheSim.exe $CONFIG

# Print final contents of cache
# cat cacheResults.results

# Print final results of cache
# cat resultsFile.results
