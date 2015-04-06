#include "cache.h"


/******************************************************************************************************
 * Decompose the address into a tag and an index
 ******************************************************************************************************/
void decomposeAddress( struct reference* ref, cache_TypeDef cache ) {
    unsigned long long address = ref->address; 
    
    // Shift address over so that it only contains index and tag
    int bitsTag = TAG_SIZE[cache];
    int bitsIndex = INDEX_SIZE[cache];
    int bitsBytes = 48 - bitsTag - bitsIndex;
    address = address >> bitsBytes;

    // Mask the tag to get the index
}
