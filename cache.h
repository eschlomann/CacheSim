#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdbool.h>
#include "configparse.h"


// Comment this line to suppress print statements
// #define PRINT

#define TRUE                    1
#define FALSE                   0


/******************************************************************************************************
 * Struct to represent each block of the cache
 ******************************************************************************************************/
struct cacheBlock {
    bool valid;
    bool dirty;
    unsigned long long tag;                 // Tag has to be able to handle up to 48 bits (long long is 64 bits)
    // Block: would put data here but the simulation doesn't put actual data into the cache, just makes calls to it
};


/******************************************************************************************************
 * Struct to represent each cache
 ******************************************************************************************************/
struct cache {
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


/******************************************************************************************************
 * Decompose the address into a tag and an index and update the reference struct
 ******************************************************************************************************/
void decomposeAddress( struct reference* ref, cache_TypeDef cache ); 


/******************************************************************************************************
 * Construct cache
 ******************************************************************************************************/
void constructCache( struct cache* cache, cache_TypeDef cacheType ); 


#endif // CACHE_H
