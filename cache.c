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
