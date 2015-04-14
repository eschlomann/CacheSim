#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stdio.h>
#include "cache.h"


// Comment this line to disable print statements
#define PRINT

/******************************************************************************************************
 * Makes call to state machine
 ******************************************************************************************************/
int stateMachine( struct reference* ref );


/******************************************************************************************************
 * Access L1 cache to get reference
 ******************************************************************************************************/
int accessL1( struct reference* ref, struct cache* cache );


#endif // STATEMACHINE_H
