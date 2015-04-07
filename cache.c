#include "cache.h"


/******************************************************************************************************
 * Decompose the address into a tag and an index and update the reference struct
 ******************************************************************************************************/
void decomposeAddress( struct reference* ref, cache_TypeDef cache ) {
    unsigned long long address = ref->address; 
    #ifdef PRINT
	printf( "Full address: %lld \n", address );
    #endif
    
    // Shift address over so that it only contains index and tag
    int bitsTag = TAG_SIZE[cache];
    int bitsIndex = INDEX_SIZE[cache];
    int bitsBytes = 48 - bitsTag - bitsIndex;
    #ifdef PRINT
	printf( "Shift amount: %d \n", bitsBytes );
    #endif
    address = address >> bitsBytes;
    #ifdef PRINT
	printf( "Tag | Index: %lld \n", address );
    #endif

    // Mask the address to get the index
    ref->index[cache] = (address & INDEX_MASK[cache]);
    #ifdef PRINT
	printf( "Index: %lld \n", ref->index[cache] );
    #endif

    // Shift address again to get tag
    ref->tag[cache] = (address >> INDEX_SIZE[cache]);
    #ifdef PRINT
	printf( "Tag: %lld \n\n", ref->tag[cache] );
    #endif
}


/******************************************************************************************************
 * Construct cache
 ******************************************************************************************************/
void constructCache( struct cache* cache, cache_TypeDef cacheType ) { 
    // Calculate number of blocks in the cache
    int numBlocks = CACHE_SIZE[cacheType] / BLOCK_SIZE[cacheType];

    // Create array of cacheBlocks
    struct cacheBlock block[ numBlocks ];

    // Point cache struct to array
    cache->block = block;
}
