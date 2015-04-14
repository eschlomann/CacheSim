#include "statemachine.h"


/******************************************************************************************************
 * Makes call to state machine
 ******************************************************************************************************/
void stateMachine( struct reference* ref ) {

    // Find which L1 cache to access
    struct cache L1_cache;
    if( ref->type == 'I' ) {
        #ifdef PRINT
        printf( "L1 Instruction Cache: %c \n", ref->type );
        decomposeAddress( ref, L1 );
        printf( "Tag: %lld \n", ref->tag[L1] );
        printf( "Index %lld \n", ref->index[L1] );
        #endif
        L1_cache = L1_instruction;
    } else if( (ref->type == 'R') || (ref->type == 'W') ) {
        #ifdef PRINT
        printf( "L1 Data Cache: %c \n", ref->type );
        decomposeAddress( ref, L1 );
        printf( "Tag: %lld \n", ref->tag[L1] );
        printf( "Index %lld \n", ref->index[L1] );
        #endif
        L1_cache = L1_data;
    } else {
        printf( "Unrecognized reference type" );
        return;
    }

    // Define initial state
    int state = QUERY_L1;
    
    // Start state machine
    while( TRUE ) {
        switch( state ) { 
            case IDLE:
                // Break out of state machine
                #ifdef PRINT
                printf( "**********************************************************************************************\n" );
                #endif
                return;

            case QUERY_L1:
                // Query L1 cache for tag
                #ifdef PRINT
                printf( "Query L1 \n" );
                #endif
                state = queryL1( ref, &L1_cache );
                break;

            case QUERY_L2:
                // Query L2 cache for tag
                #ifdef PRINT
                printf( "Query L2 \n" );
                #endif
                state = queryL2( ref );
                break;

            case ADD_L1:
                // Add tag to L1 cache
                #ifdef PRINT
                printf( "Add L1 \n" );
                #endif
                state = addL1( ref, &L1_cache );
                break;

            case ADD_L2:
                // Add tag to L2 cache
                #ifdef PRINT
                printf( "Add L2 \n" );
                #endif
                state = addL2( ref );
                break;
        }
    }
}


/******************************************************************************************************
 * Query L1 cache to get reference
 ******************************************************************************************************/
int queryL1( struct reference* ref, struct cache* cache ) {

    // Decompose the address for the L1 cache
    decomposeAddress( ref, L1 );

    // Query L1 cache
    bool hit = queryCache( ref, cache );

    // Transition based on result
    if( hit == TRUE ) {
        return IDLE;
    } else {
        return QUERY_L2;
    }
}


/******************************************************************************************************
 * Access L2 cache to get reference
 ******************************************************************************************************/
int queryL2( struct reference* ref ) {
    
    // Decompose the address for the L2 cache
    decomposeAddress( ref, L2 );

    // Query L2 cache
    bool hit = queryCache( ref, &L2_unified );

    // Transition based on result
    if( hit == TRUE ) {
        return ADD_L1;
    } else {
        return ADD_L2;
    }
}


/******************************************************************************************************
 * Add tag to L1 cache
 ******************************************************************************************************/
int addL1( struct reference* ref, struct cache* cache ) {
    
    // Add reference 
    addCache( ref, cache );

    // Transition
    return IDLE;
}


/******************************************************************************************************
 * Add tag to L2 cache
 ******************************************************************************************************/
int addL2( struct reference* ref ) {
    
    // Add reference 
    addCache( ref, &L2_unified );

    // Transition
    return ADD_L1;
}
