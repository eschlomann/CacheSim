#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "configparse.h"


// Comment this line to suppress print statements
#define PRINT


#define TRUE                    1
#define FALSE                   0


/******************************************************************************************************
 * Calculate the tag size based on the given parameters
 ******************************************************************************************************/
int tagSize( int blockSize, int cacheSize, int assoc );


/******************************************************************************************************
 * Struct to represent each block of the cache
 ******************************************************************************************************/
struct cacheBlock {
    bool valid;
    bool dirty;
    unsigned long long tag;                 // tag has to be able to handle up to 48 bits (long long is 64 bits)
    // block: would put data here but the simulation doesn't put actual data into the cache, just makes calls to it
};


/******************************************************************************************************
 * Struct to represent the address as a tag and an index
 ******************************************************************************************************/
struct address {
    unsigned long long address;             // 48 bits
    unsigned long long tag;                 // up to 48 bits
    int index;                              // up to 10 bits
};


// Cache should consist of array of cacheBlocks where the calculated index corresponds to the array index

#endif // CACHE_H
