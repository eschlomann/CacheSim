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
        LRU_inst* LRU = makeLRU();
        LRU->type = cacheType;

        // Link initialized arrays to associated cacheBlock
        block[i].valid = valid;
        block[i].dirty = dirty;
        block[i].tags = tags;
        block[i].LRU = LRU;

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

LRU_inst* makeLRU(cache_TypeDef cacheType) {
    return calloc (1, sizeof(LRU_inst));
}

bool LRUcheckDestroyPush(LRU_inst* LRU, unsigned long long tag) {
    printf("should get in here");
    if ( LRU -> first == NULL) {
        LRUpush ( LRU, tag );
        return false;
    } else {
        if ( LRU -> first -> tag == tag ) {
            return true;
        } else {
            LRUnode * parser = LRU -> first;
            while ( parser != LRU -> last) {
                if (parser -> next -> tag == tag) {
                    parser -> next -> prev = parser -> prev -> next;
                    parser -> prev -> next = parser -> next -> next;
                    free( parser -> next );
                    LRUpush( LRU , tag );
                    free(parser);
                    return true;
                }
                parser = parser -> next;
            }
            LRUpush( LRU, tag );
            free(parser);
            return false;
        }
    }
}

void LRUclear (LRU_inst* LRU) {
    if (LRU -> first == LRU -> last) {
        if (!( LRU -> first == NULL )) {
            free (LRU -> first);
        }
    } else {
        while( LRU -> first -> next != LRU -> last ) {
            LRU -> first -> next = LRU -> first -> next -> next;
            free(LRU -> first -> next);
        }
        free( LRU -> first );
        free( LRU -> last );
    } 
}

void LRUpush (LRU_inst* LRU, unsigned long long tag) {
    if ( LRU->count == ASSOC[LRU->type] ) {
        if ( !LRUpop (LRU) ) {
            printf("THIS SHOULD NEVER HAPPEN");
        }
    }
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

bool LRUpop (LRU_inst* LRU) {
    bool result;
    LRUnode* lastNode = LRU->last;
    if( lastNode == NULL) {
        result = false;
    } else {
        if ( LRU->first == lastNode) {
            LRU->first = NULL;
            LRU->last = NULL;
        } else {
            lastNode->prev->next = NULL;
            LRU->last = lastNode->prev;
        }
        result = true;
        LRU->count--;
    }
    free(lastNode);
    return true;
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
