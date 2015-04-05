#include <stdio.h>
#include <stdbool.h>
#include <math.h>


// Comment this line to suppress print statements
#define PRINT


// Calculate tag size
int tagSize( int blockSize, int cacheSize, int assoc );

struct cacheBlock {
    bool valid;
    bool dirty;
    // tag
    // block
};

// Cache should consist of array of cacheBlocks where the calculated index corresponds to the array index
