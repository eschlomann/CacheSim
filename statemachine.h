#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stdio.h>
#include "cache.h"
#include "print.h"


// Comment this line to disable print statements
// #define PRINT


/******************************************************************************************************
 * Define states used in the state machine
 ******************************************************************************************************/
#define IDLE                        0
#define QUERY_L1                    1
#define QUERY_L2                    2
#define ADD_L1                      3
#define ADD_L2                      4
#define HANDLE_WRITE                5
#define FLUSH                       6
#define TERMINATE                   7


/******************************************************************************************************
 * Struct to represent state and L1_Index and L1_Tag
 ******************************************************************************************************/
 struct state {
    int next;                               // Transition to this state 
    int iteration;                          // Index of the current reference
    char type;                              // Type of instruction
    bool queryL2;                           // Indicate if should transition to QUERY_L2 state
    bool addL2;                             // Indicate if should transition to ADD_L2 state
    unsigned long long L1_Tag;              // L1 tag to access
    unsigned long long L1_Index;            // L1 index to access
    unsigned long long L2_Tag;              // L2 tag of access
    unsigned long long L2_Index;            // L2 index to access
};


/******************************************************************************************************
 * Flag for if first state should be set to FLUSH
 ******************************************************************************************************/
bool setFlush;


/******************************************************************************************************
 * Makes call to state machine
 ******************************************************************************************************/
void stateMachine( struct reference* ref );


/******************************************************************************************************
 * Selects correct L1 cache based on reference type
 ******************************************************************************************************/
struct cache cacheType( struct reference* ref );


/******************************************************************************************************
 * Set L1 tag and index based on iteration
 ******************************************************************************************************/
void incrementL1( struct state* state, struct reference* ref );


/******************************************************************************************************
 * Access L1 cache to get reference
 ******************************************************************************************************/
void queryL1( struct state* state, struct cache* cache );


/******************************************************************************************************
 * Access L2 cache to get reference
 ******************************************************************************************************/
void queryL2( struct state* state );


/******************************************************************************************************
 * Add tag to L1 cache
 ******************************************************************************************************/
void addL1( struct state* state, struct cache* cache );


/******************************************************************************************************
 * Add tag to L2 cache
 ******************************************************************************************************/
void addL2( struct state* state );


/******************************************************************************************************
 * Set the dirty bit of tag in L1 cache
 ******************************************************************************************************/
void handleWrite( struct state* state, struct cache* cache );


/******************************************************************************************************
 * Flush the cache by invalidating all of the data
 ******************************************************************************************************/
void flushCaches( struct state* state );


#endif // STATEMACHINE_H
