#include "statemachine.h"


/******************************************************************************************************
 * Makes call to state machine
 ******************************************************************************************************/
int stateMachine( struct reference* ref ) {
    // Find which L1 cache to access
    struct cache L1_cache;
    if( ref->type == 'I' ) {
        #ifdef PRINT
        printf( "L1 Instruction Cache: %c \n", ref->type );
        #endif
        L1_cache = L1_instruction;
    } else if( (ref->type == 'R') || (ref->type == 'W') ) {
        #ifdef PRINT
        printf( "L1 Data Cache: %c \n", ref->type );
        #endif
        L1_cache = L1_data;
    } else {
        // Unrecognized reference type, return an error
        return 1;
    }

    // Access L1 cache
    accessL1( ref, &L1_cache );
}


/******************************************************************************************************
 * Access L1 cache to get reference
 ******************************************************************************************************/
int accessL1( struct reference* ref, struct cache* cache ) {

}
