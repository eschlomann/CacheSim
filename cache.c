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

LRUlist* makeLRU() {
    return calloc (1, sizeof(LRUlist));
}

void LRUclear (LRUlist* LRU) {

}

void LRUdestroy (LRUlist* LRU) {

}

void LRUpush (LRUlist* LRU, unsigned long long tag) {
    LRUnode * node = calloc( 1, sizeof(LRUnode) );
    node->tag = tag;
    if (LRU->first == NULL) {
        LRU->first = node;
        LRU->last = node;
    } else {
        LRU->last->next = node;
        node->prev = LRU->last;
        LRU->last = node;
    }
    LRU->count++;
    printf("\n The size of the LRU is %lu \n",LRU->count);
    printf("\n The last item in the list's tag is %llx\n",LRU->last->tag);
}

unsigned long long LRUpop (LRUlist* LRU) {
    unsigned long long result;
    LRUnode* lastNode = LRU->last;
    if( lastNode == NULL) {
        result = 0;
    } else {
        if ( LRU->first == lastNode) {
            LRU->first = NULL;
            LRU->last = NULL;
        } else {
            lastNode->prev->next = NULL;
            LRU->last = lastNode->prev;
        }
        result = lastNode->tag;
        LRU->count--;
    }
    free(lastNode);
    return result;
}

/******************************************************************************************************
 * Check L1i cache for an instruction
 ******************************************************************************************************/
/*int checkCache( struct reference* ref, cache_TypeDef cache ) {

    if (L1_instruction.block[ref->index[cache]].tag == ref->tag[cache]) {
        printf("HOLY SHIT THAT ACUTALLY WORKED???");
        return 1;
    } else {
        return 0;
    }
}*/
