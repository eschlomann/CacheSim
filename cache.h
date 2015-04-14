#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdbool.h>
#include "configparse.h"


// Comment this line to suppress print statements
//#define PRINT

#define TRUE                    1
#define FALSE                   0

typedef struct LRUnode {
    struct LRUnode* prev;
    struct LRUnode* next;
    int arrayIndex;
    bool valid;
    bool dirty;
} LRUnode;

typedef struct LRU_inst {
    cache_TypeDef type;
    unsigned long count;
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
    unsigned long long tag[2];              // up to 48 bits
    unsigned long long index[2];            // up to 10 bits
    int numBytes;                           // Number of bytes requested
};


//LRU_inst* makeLRU();
void LRUclear (LRU_inst* LRU);
void LRUpush (LRU_inst* LRU, int arrayIndex);
int LRUpop (LRU_inst* LRU);
//bool LRUcheckDestroyPush(LRU_inst* LRU, unsigned long long tag);


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
void constructCache( struct cache* cache, cache_TypeDef cacheType );


/******************************************************************************************************
 * Add given reference to the cache
 ******************************************************************************************************/
void addCache( struct reference* ref, struct cache* cache ); 


/******************************************************************************************************
 * Query given cache for reference
 ******************************************************************************************************/
bool queryCache( struct reference* ref, struct cache* cache ); 


/*int checkCache( struct reference* ref, cache_TypeDef cache ); */

#endif // CACHE_H
