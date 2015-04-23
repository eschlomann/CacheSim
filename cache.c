#include "cache.h"


/******************************************************************************************************
 * Decompose the address into a tag and an index and update the reference struct
 ******************************************************************************************************/
void decomposeAddress( struct reference* ref, cache_TypeDef cache ) {
    #define PRINT

    unsigned long long address = ref->address; 
    #ifdef PRINT
	printf( "   Full address: %llx \n", address );
    #endif
    
    // Shift address over so that it only contains index and tag
    int bitsTag = TAG_SIZE[cache];
    int bitsIndex = INDEX_SIZE[cache];
    int bitsBlock = 48 - bitsTag - bitsIndex;
    address = address >> bitsBlock;

    // Initial address shifted to byte boundary
    unsigned long long initialAddress = ref->address >> 3;

    // Mask the address to get the index
    unsigned long long initialIndex = (address & INDEX_MASK[cache]);

    // Shift address again to get tag
    unsigned long long initialTag = (address >> INDEX_SIZE[cache]);

    // Up to this point should be the base index and tag generated by the address
    // Depending on the number of bytes requested, it may be multiple references
    unsigned long long endAddress = ref->address;
    unsigned long long offset = (unsigned long long) ref->numBytes;
    // offset = offset << 2;
    endAddress = endAddress + offset;
    #ifdef PRINT
    printf( "   numBytes: %d \n", ref->numBytes );
	printf( "   End Address:  %llx \n", endAddress );
    #endif

    // Shift over by 3 (bytes)
    endAddress = endAddress >> 3;

    // Check if same as initial address (only one reference necessary)
    if( endAddress == initialAddress ) {
        // Only one reference is needed
        ref->numReferences = 1;
        #ifdef PRINT
        printf( "   One reference \n" );
        #endif

        // Initialize reference
        ref->index = calloc( 1, sizeof(unsigned long long) );
        ref->tag = calloc( 1, sizeof(unsigned long long) );

        // Set reference
        ref->index[0] = initialIndex;
        ref->tag[0] = initialTag;

        #ifdef PRINT
	    printf( "   Index 0: %lld \n", ref->index[0] );
	    printf( "   Tag 0:   %lld \n", ref->tag[0] );
        #endif
    }
    // Otherwise need to calculate additional references
    else {
        // Number of references needed
        int increment = (int)(endAddress - initialAddress);
        ref->numReferences = increment + 1;
        printf( "   Number of References: %d \n", increment+1 );

        // Initialize references
        ref->index = calloc( increment+1, sizeof(unsigned long long) );
        ref->tag = calloc( increment+1, sizeof(unsigned long long) );

        // Set initial reference
        ref->index[0] = initialIndex;
        ref->tag[0] = initialTag;
        #ifdef PRINT
	    printf( "   Index 0: %llx \n", ref->index[0] );
	    printf( "   Tag 0:   %llx \n", ref->tag[0] );
        #endif

        // For each increment, assign tag and index to the reference
        int i;
        unsigned long long currentAddress = initialAddress;
        unsigned long long addr;
        unsigned long long index = initialIndex;
        unsigned long long tag = initialTag;
        for( i = 1; i < increment+1; i++ ) {

            // Increment address by 2 bytes
            currentAddress = currentAddress + 1;

            // Make addr
            addr = currentAddress >> (bitsBlock-3);
            printf( "   Addr: %llx \n", addr );

            // Get associated index
            index = (addr & INDEX_MASK[cache]);

            // Get associated tag
            tag = (addr >> INDEX_SIZE[cache]);

            ref->index[i] = index;
            ref->tag[i] = tag;
            
            #ifdef PRINT
	        printf( "   Index %d: %llx \n", i, ref->index[i] );
	        printf( "   Tag %d:   %llx \n", i, ref->tag[i] );
            #endif

        }
    }
}


/******************************************************************************************************
 * Construct cache
 ******************************************************************************************************/
void constructCache( struct cache* cache, cache_TypeDef cacheType ) { 
    // Calculate number of blocks in the cache
    int numBlocks = CACHE_SIZE[cacheType] / BLOCK_SIZE[cacheType];

    // Create array of cacheBlocks
    struct cacheBlock* block = calloc( numBlocks, sizeof(struct cacheBlock) );

    // Initialize each cacheBlock
    int i;
    int associativity = ASSOC[cacheType];
    for( i=0; i<numBlocks; i++ ) {
        // Initialize valid to zero (all tags are invalid)
        bool* valid = (bool*) calloc( associativity, sizeof(bool) );

        // Initialize dirty to zero (all tags are NOT dirty)
        bool* dirty = (bool*) calloc( associativity, sizeof(bool) );

        // Initialize tags to zero (this is arbitrary)
        unsigned long long* tags = (unsigned long long*) calloc( associativity, sizeof(unsigned long long) );

        //Create the LRU instance and check the cache type
        LRU_inst* LRU = (LRU_inst*) calloc (1, sizeof(LRU_inst) );
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
 * Add given reference to the cache
 ******************************************************************************************************/
void addCache( unsigned long long index, unsigned long long tag, struct cache* cache ) {
    #define PRINT

    // Get associated block
    struct cacheBlock block = cache->block[index];

    // Check for any invalid blocks first
    int i;
    int associativity = ASSOC[cache->type];
    for( i=0; i<associativity; i++ ) {
        if( block.valid[i] == FALSE ) {
            #ifdef PRINT
            printf( "   Success, added %llx \n", tag );
            #endif

            // Add tag in this position
            block.valid[i] = TRUE;
            block.dirty[i] = FALSE;
            block.tags[i] = tag;

            // Indicate this index was recently used
            LRUpush ( block.LRU , i );

            // Don't ever need to writeback since data is invalid
            return;
        }
    }

    #ifdef PRINT
    printf( "   Replace last used block \n" );
    #endif

    // If there are no invalid blocks, replace the least recently used block
    if ( (int) block.LRU->count == associativity) {
        #ifdef PRINT
        printf( "   Success, added %llx \n", tag );
        #endif

        // Get most least recently used index
        int tagIndex = LRUpop( block.LRU );
        
        // If block is dirty, reset and indicate writeback
        if( block.dirty[tagIndex] == TRUE ) {
            block.dirty[tagIndex] = FALSE;
            
            // If L1 cache, need to writeback to L2 cache   
            if( cache->type == L1 ) {
                writeback( index, block.tags[tagIndex] );        
            }
            // Otherwise need to access main memory
            else { 

            }
        }
        
        // Overwrite associated tag
        block.tags[tagIndex] = tag;

        // Indicate index was recently used
        LRUpush ( block.LRU , tagIndex );
    } 
}


/******************************************************************************************************
 * Query given cache for reference
 ******************************************************************************************************/
bool queryCache( unsigned long long index, unsigned long long tag, struct cache* cache ) {
    #define PRINT
    
    // Get associated block
    struct cacheBlock block = cache->block[index];
    // Check tag(s)
    int i;
    int associativity = ASSOC[cache->type];
    bool hasTag = FALSE;
    for( i=0; i<associativity; i++ ) {
        // Check if tag is valid
        if( block.valid[i] == TRUE ) {
            if( block.tags[i] == tag ) {
                #ifdef PRINT
                printf( "   Tag %llx is contained in cache \n", tag );
                #endif
                hasTag = TRUE;
                LRUpush ( block.LRU , i );
            }
        }
    }
    
    return hasTag;
}


/******************************************************************************************************
 * Set dirty bit for given reference
 ******************************************************************************************************/
void setDirty( unsigned long long index, unsigned long long tag, struct cache* cache ) {
    // #define PRINT
    
    // Get associated block
    struct cacheBlock block = cache->block[index];
    // Check tag(s)
    int i;
    int associativity = ASSOC[cache->type];
    for( i=0; i<associativity; i++ ) {
        // Check if tag is valid
        if( block.valid[i] == TRUE ) {
            if( block.tags[i] == tag ) {
                #ifdef PRINT
                printf( "   Tag %llx is set to dirty \n", tag );
                #endif
                
                // Set dirty bit
                block.dirty[i] = TRUE;

                // Update LRU
                LRUpush ( block.LRU , i );
                return;
            }
        }
    }
}


/******************************************************************************************************
 * Construct L2_Tag and L2_Index based on L1_Tag and L1_Index
 ******************************************************************************************************/
void constructL2Ref( struct L2_Reference* ref, unsigned long long index, unsigned long long tag ) {
    #define PRINT

    // Reconstruct the original address from the index and tag
    unsigned long long address = ( (tag << INDEX_SIZE[L1]) | index );

    // Get difference in tag+index address size between L1 and L2 (how much larger is L1 "address" than L2)
    int difference = (TAG_SIZE[L1] + INDEX_SIZE[L1]) - (TAG_SIZE[L2] + INDEX_SIZE[L2]);

    // Shift address to fit size of L2
    address = address >> difference;

    // Mask the address to get the index
    ref->L2_Index = (address & INDEX_MASK[L2]);
    #ifdef PRINT
	printf( "   Constructed Index: %6llx \n", ref->L2_Index );
    #endif

    // Shift address again to get tag
    ref->L2_Tag = (address >> INDEX_SIZE[L2]);
    #ifdef PRINT
	printf( "   Constructed Tag: %13llx \n", ref->L2_Tag );
    #endif
}


/******************************************************************************************************
 * Writeback from L1 cache to L2 cache by setting corresponding data to dirty
 ******************************************************************************************************/
void writeback( unsigned long long index, unsigned long long tag ) { 
    #define PRINT

    // Make L2_Reference struct
    struct L2_Reference ref;

    // Get L2 reference
    constructL2Ref( &ref, index, tag );

    // Set constructed block to dirty
    setDirty( ref.L2_Index, ref.L2_Tag, &L2_unified );
}


/******************************************************************************************************
 * Flush the cache by setting all blocks to invalid
 ******************************************************************************************************/
void flush( struct cache* cache ) {
    
    // Calculate number of blocks in the cache
    int numBlocks = CACHE_SIZE[cache->type] / BLOCK_SIZE[cache->type];

    // Get associativity
    int associativity = ASSOC[cache->type];

    // Iterate through each cacheBlock
    int i, j;
    struct cacheBlock block;
    for( i = 0; i < numBlocks; i++ ) {
        block = cache->block[i];
        for( j = 0; j < associativity; j++ ) {
            if( block.valid[j] == TRUE ) {
                // Invalidate block
                block.valid[j] = FALSE;
            
                // If dirty, handle writeback
                if( block.dirty[j] == TRUE ) {
                    block.dirty[j] = FALSE;
                    
                    // If L1 cache, need to writeback to L2 cache   
                    if( cache->type == L1 ) {
                        writeback( i, block.tags[j] );        
                    }
                    // Otherwise need to access main memory
                    else { 
                        // Do something
                    }
                }

                // Handle LRU (for now, can just pop to remove nodes from LRU)
                LRUpop( block.LRU );
            }
        }
    }
}


/******************************************************************************************************
 * Clears the LRU and frees the nodes. Should call when done with the entire program
 ******************************************************************************************************/
void LRUclear (LRU_inst* LRU) {
    // Check the cases where it is single item or null
    if (LRU -> first == LRU -> last) {
        if (!( LRU -> first == NULL )) {
            free (LRU -> first);
        }
    // If its multiple items iterate through them and free memory
    } else {
        while( LRU -> first -> next != LRU -> last ) {
            LRU -> first -> next = LRU -> first -> next -> next;
            free(LRU -> first -> next);
        }
        free( LRU -> first );
        free( LRU -> last );
    } 
}


/******************************************************************************************************
 * Pushes a arrayIndex to the LRU should never push to something thats full (handled elsewhere).
 ******************************************************************************************************/
void LRUpush (LRU_inst* LRU, int arrayIndex) {
    // Check to make sure that the LRU isn't full
    if ( LRU->count == ASSOC[LRU->type] ) {
        printf ("\nTHE LRU IS FULL AND YOU'RE PUSHING\n");
        if ( !LRUpop (LRU) ) {
            //Code should never get to here, it should be handled with the count
        }
    }
    
    if (LRU -> first != NULL) {
        LRUnode * parser;
        parser = LRU->first;
        while ( parser->next != NULL ) {
            if ( parser -> arrayIndex == arrayIndex ) {
                if (LRU -> count == 1) {
    
                } else if ( parser == LRU->first ) {
                    free( parser->next->prev );
                    LRU -> first = parser -> next;
                    LRU->count--;
                } else {
                    free(parser -> prev -> next);
                    parser -> prev -> next = parser -> next;
                    if (parser -> next != NULL) {
                        parser -> next -> prev = parser -> prev;   
                    }
                    LRU->count--;
                }
            }
            parser = parser -> next;
        }
        if ( LRU -> last != NULL) {
            if ( LRU -> last -> arrayIndex == arrayIndex) {
                LRUpop(LRU);
            }
        }
    }

    // Make the node with the value of the arrayIndex
    LRUnode * node = calloc( 1, sizeof(LRUnode) );
    node->arrayIndex = arrayIndex;
    // Empty LRU case
    if (LRU->first == NULL) {
        LRU->first = node;
        LRU->last = node;
        node -> next = NULL;
        node -> prev = NULL;
    } else {
        LRU->first->prev = node;
        node->next = LRU->first;
        LRU->first = node;
    }
    LRU->count++;
    #ifdef PRINT
    // printf("   The size of the LRU is %lu \n",LRU->count);
    // printf("   The last item in the list's tag is %i\n",LRU->last->arrayIndex);
    #endif
}


/******************************************************************************************************
 * Pops an item off the LRU, the int of the last used arrayIndex that needs to be replaced
 ******************************************************************************************************/
int LRUpop (LRU_inst* LRU) {
    // Return value
    int result;
    
    // Empty LRU case, doesn't need to pop, returns 0 
    if( LRU -> count == 0 ) {
        result = 0;
    } else {
        if ( LRU -> count == 1 ) {
            // if there is only one item
            free( LRU -> first );
            LRU -> last = NULL;
            LRU -> first = NULL;
            result = 0;
        } else {
            // more than one item, really is only called when items = associativity
            result = LRU -> last -> arrayIndex;
            LRU -> last = LRU -> last -> prev;
            free ( LRU -> last -> next );
            LRU -> last -> next = NULL;
        }
        LRU->count--;
        printf("decrimented LRU counter");
    }
    return result;
}
