#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdbool.h>
#include "configparse.h"
#include "print.h"


// Comment this line to suppress print statements
//#define PRINT

#define TRUE                    1
#define FALSE                   0


/******************************************************************************************************
 * Struct to represent each item in the LRU
 ******************************************************************************************************/
typedef struct LRUnode {
    struct LRUnode* prev;
    struct LRUnode* next;
    int arrayIndex;
    bool valid;
    bool dirty;
} LRUnode;

/******************************************************************************************************
 * Struct to represent LRU instance
 ******************************************************************************************************/
typedef struct LRU_inst {
    cache_TypeDef type;
    int count;
    LRUnode* first;
    LRUnode* last;
} LRU_inst;

/******************************************************************************************************
 * Struct to represent each block of the cache
 *      NOTE: Everything is represented as an array to support associative caches
 ******************************************************************************************************/
struct cacheBlock {
    bool* valid;                            // Pointer to valid array
    bool* dirty;                            // Pointer to dirty array
    unsigned long long* tags;               // Pointer to tag array
    LRU_inst* LRU;                          // Pointer to LRU
    // Block: would put data here but the simulation doesn't put actual data into the cache, just makes calls to it
};


/******************************************************************************************************
 * Struct to represent each cache
 ******************************************************************************************************/
struct cache {
    cache_TypeDef type;                     // Type of cache (L1 or L2)
    char L1_Type;                           // Data or Instruction cache (for L1 only)
    struct cacheBlock* block;               // Pointer to an array of cacheBlocks
};


/******************************************************************************************************
 * Define the three caches
 ******************************************************************************************************/
struct cache L1_instruction;
struct cache L1_data;
struct cache L2_unified;


/******************************************************************************************************
 * Struct to represent the information provided by the program trace
 ******************************************************************************************************/
struct reference {
    char type;                              // I, R, W (Instruction, Read Data, Write Data)
    unsigned long long address;             // 48 bits
    unsigned long long* tag;                // Array of L1 tags to access
    unsigned long long* index;              // Array of L1 indexes to access
    int numReferences;                      // Number of aligned references requested
    int numBytes;                           // Number of bytes requested
};


/******************************************************************************************************
 * Struct to represent index and tag of L2 cache
 ******************************************************************************************************/
struct L2_Reference {
    unsigned long long L2_Tag;              // L2 tag to access
    unsigned long long L2_Index;            // L2 index to access
};


/******************************************************************************************************
 * Clear an LRU
 ******************************************************************************************************/
void LRUclear (LRU_inst* LRU);


/******************************************************************************************************
 * Push an arrayindex onto the LRU
 ******************************************************************************************************/
void LRUpush (LRU_inst* LRU, int arrayIndex);


/******************************************************************************************************
 * Pop an array index off the LRU, used to add new tag with same index when tag[] is full
 ******************************************************************************************************/
int LRUpop (LRU_inst* LRU);


/******************************************************************************************************
 * Define reference used for all traces 
 ******************************************************************************************************/
struct reference ref;


/******************************************************************************************************
 * Decompose the address into a tag and an index and update the reference struct
 ******************************************************************************************************/
void decomposeAddress( struct reference* ref, cache_TypeDef cache ); 


/******************************************************************************************************
 * Construct cache
 ******************************************************************************************************/
void constructCache( struct cache* cache, cache_TypeDef cacheType, char c );


/******************************************************************************************************
 * Add given reference to the cache
 ******************************************************************************************************/
void addCache( unsigned long long index, unsigned long long tag, struct cache* cache, char instructionType ); 


/******************************************************************************************************
 * Query given cache for reference
 ******************************************************************************************************/
bool queryCache( unsigned long long index, unsigned long long tag, struct cache* cache ); 


/******************************************************************************************************
 * Set dirty bit for given reference
 ******************************************************************************************************/
bool setDirty( unsigned long long index, unsigned long long tag, struct cache* cache ); 


/******************************************************************************************************
 * Construct L2_Tag and L2_Index based on L1_Tag and L1_Index
 ******************************************************************************************************/
void constructL2Ref( struct L2_Reference* ref, unsigned long long index, unsigned long long tag ); 


/******************************************************************************************************
 * Writeback from L1 cache to L2 cache by setting corresponding data to dirty
 ******************************************************************************************************/
void writeback( unsigned long long index, unsigned long long tag, char instructionType ); 


/******************************************************************************************************
 * Flush the cache by setting all blocks to invalid
 ******************************************************************************************************/
void flush( struct cache* cache ); 


#endif // CACHE_H
