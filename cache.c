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

    // Initialize each cacheBlock
    int i, j;
    int associativity = ASSOC[cacheType];
    for( i=0; i<numBlocks; i++ ) {
        // Initialize valid to zero (all tags are invalid)
        bool* valid = (bool*) malloc( associativity*sizeof(bool) );
        for( j=0; j<associativity; j++ ) {
            valid[j] = FALSE;
        }

        // Initialize dirty to zero (all tags are NOT dirty)
        bool* dirty = (bool*) malloc( associativity*sizeof(bool) );

        // Initialize tags to zero (this is arbitrary)
        unsigned long long* tags = (unsigned long long*) malloc( associativity*sizeof(unsigned long long) );

        // Link initialized arrays to associated cacheBlock
        block[i].valid = valid;
        block[i].dirty = dirty;
        block[i].tags = tags;
    }

    // Point cache struct to array
    cache->block = block;

    // Set cache type
    cache->type = cacheType;
}


/******************************************************************************************************
 * Query given cache for reference
 ******************************************************************************************************/
bool queryCache( struct reference* ref, struct cache* cache ) {
    // Get index 
    unsigned long long index = ref->index[cache->type];

    // Get tag
    unsigned long long tag = ref->tag[cache->type];
    
    // Get associated block
    struct cacheBlock block = cache->block[index];

    // Check tag(s)
    int i;
    int associativity = ASSOC[cache->type];
    bool hasTag = FALSE;
    for( i=0; i<associativity; i++ ) {
        // Check if tag is valid
        if( block.valid[i] ) {
            if( block.tags[i] == tag ) {
                hasTag = TRUE;
            }
        }
    }
    
    return hasTag;
}
