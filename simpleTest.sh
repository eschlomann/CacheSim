#!/bin/bash

# Define trace path
TRACE="traces/traces-short/tr1"

make
# Runs TR1 trace through simulation under default settings
zcat $TRACE | ./cacheSim.exe
