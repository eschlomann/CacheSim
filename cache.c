#include "cache.h"


/******************************************************************************************************
 * Calculate the tag size based on the given parameters
 ******************************************************************************************************/
int tagSize( int blockSize, int cacheSize, int assoc ) {
    // Calculate number of blocks in the cache
    int numBlocks = cacheSize / blockSize;
    #ifdef PRINT
    printf( "Blocks in cache: %d \n", numBlocks );
    #endif

    // Calculate number of bits used for the index (ignoring associativity)
    int bitsIndex = log(numBlocks) / log(2);
    #ifdef PRINT
    printf( "Number of index bits: %d \n", bitsIndex );
    #endif

    // Adjust for associativity
    int bitsAssociative = log(assoc) / log(2);
    bitsIndex = bitsIndex - bitsAssociative;
    #ifdef PRINT
    printf( "Number of associative bits: %d \n", bitsAssociative );
    printf( "Adjusted number of index bits: %d \n", bitsIndex );
    #endif

    // Calculate number of bits used to address each byte in the block
    int bitsBytes = log(blockSize) / log(2);
    #ifdef PRINT
    printf( "Number of byte bits: %d \n", bitsBytes );
    #endif
    
    // Size of the address string
    int numAddress = 48;

    // Calculate how many bits are left over from the address
    int bitsTag = numAddress - bitsIndex - bitsBytes;
    #ifdef PRINT
    printf( "Number of tag bits: %d \n", bitsTag );
    #endif

    return bitsTag;
}
