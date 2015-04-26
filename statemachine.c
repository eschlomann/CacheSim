#include "statemachine.h"


/******************************************************************************************************
 * Makes call to state machine
 ******************************************************************************************************/
void stateMachine( struct reference* ref ) {

    // Find which L1 cache to access
    struct cache L1_cache = cacheType( ref );

    // Define initial state
    struct state state;
    state.type = ref->type;
    state.iteration = 0;

    // Check instruction count
    if( setFlush == TRUE ) {
        state.next = FLUSH;
    } else {
        state.next = QUERY_L1;
    }

    // Define initial L1 reference
    decomposeAddress( ref, L1 );
    state.L1_Index = ref->index[0];
    state.L1_Tag = ref->tag[0];

    #ifdef PRINT
    printf( "Tag:   %13llx \n", ref->tag[0] );
    printf( "Index: %6llx \n", ref->index[0] );
    #endif
    
    // Start state machine
    while( TRUE ) {
        switch( state.next ) { 
            case IDLE:
                // Increment L1 reference
                #ifdef PRINT
                printf( "Increment Reference \n" );
                #endif
                incrementL1( &state, ref );
                break;

            case QUERY_L1:
                // Query L1 cache for tag
                #ifdef PRINT
                printf( "Query L1 \n" );
                #endif
                queryL1( &state, &L1_cache );
                break;

            case QUERY_L2:
                // Query L2 cache for tag
                #ifdef PRINT
                printf( "Query L2 \n" );
                #endif
                queryL2( &state );
                break;

            case ADD_L1:
                // Add tag to L1 cache
                #ifdef PRINT
                printf( "Add L1 \n" );
                #endif
                addL1( &state, &L1_cache );
                break;

            case ADD_L2:
                // Add tag to L2 cache
                #ifdef PRINT
                printf( "Add L2 \n" );
                #endif
                addL2( &state );
                break;

            case HANDLE_WRITE:
                // Set dirty bit in L1 cache
                #ifdef PRINT
                printf( "Handle write case \n" );
                #endif
                handleWrite( &state, &L1_cache );
                break;

            case FLUSH:
                // Flush and invalidate all caches
                #ifdef PRINT
                printf( "Flush \n" );
                #endif
                flushCaches( &state );
                break;

            case TERMINATE:
                // Break out of state machine
                #ifdef PRINT
                printf( "**********************************************************************************************\n" );
                #endif
                terminate( ref );
                return;
        }
    }
}


/******************************************************************************************************
 * Selects correct L1 cache based on reference type
 ******************************************************************************************************/
struct cache cacheType( struct reference* ref ) {
    
    // Make sure setFlush is initially false
    setFlush = FALSE;

    if( ref->type == 'I' ) {
        runResults.numInst++;

        // Check number of instructions
        if( runResults.numInst % 380000 == 0 ) {
            setFlush = TRUE;
        }

        #ifdef PRINT
        printf( "L1 Instruction Cache: %c \n", ref->type );
        #endif

        return L1_instruction;

    } else if( (ref->type == 'R') || (ref->type == 'W') ) {
        if ( ref-> type == 'R' ) {
            runResults.numReads++;
        } else {
            runResults.numWrites++;
        }

        #ifdef PRINT
        printf( "L1 Data Cache: %c \n", ref->type );
        #endif
        
        return L1_data;

    } else {
        printf( "Unrecognized reference type" );
        return L1_instruction;
    }
}


/******************************************************************************************************
 * Set L1 tag and index based on iteration
 ******************************************************************************************************/
void incrementL1( struct state* state, struct reference* ref ) {

    // Increment iteration
    state->iteration++;

    // Check if there is another reference
    if( state->iteration == ref->numReferences ) {
        state->next = TERMINATE;
        return;
    }

    // Otherwise set new references
    state->L1_Index = ref->index[ state->iteration ];
    state->L1_Tag = ref->tag[ state->iteration ];

    // Transition back start of statemachine
    state->next = QUERY_L1;
    return;
}


/******************************************************************************************************
 * Query L1 cache to get reference
 ******************************************************************************************************/
void queryL1( struct state* state, struct cache* cache ) {

    // Query L1 cache
    bool hit = queryCache( state->L1_Index, state->L1_Tag, cache );

    // Transition based on result
    if( (hit == TRUE) && (state->type == 'W') ) {
        runResults.l1d_hit++;
        runResults.numWriteCycles += config.L1_hit_time;
        state->next = HANDLE_WRITE;
        return;
    } 
    else if( hit == TRUE) {
        if (state-> type == 'R') {
            runResults.l1d_hit++;
            runResults.numReadCycles += config.L1_hit_time;
        } else {
            runResults.l1i_hit++;
            runResults.numInstCycles += config.L1_hit_time;
        }
        state->next = IDLE;
        return;
    } 
    else {
        if (state->type == 'I') {
            runResults.l1i_miss++;
            runResults.numInstCycles += config.L1_miss_time + config.L1_hit_time + config.L1_transfer_cycles;

        } else if (state->type == 'W') {
            runResults.l1d_miss++;
            runResults.numWriteCycles += config.L1_miss_time + config.L1_hit_time + config.L1_transfer_cycles;
        } else {
            runResults.l1d_miss++;
            runResults.numReadCycles += config.L1_miss_time + config.L1_hit_time + config.L1_transfer_cycles;
        }
        // state->next = QUERY_L2;
        state->next = ADD_L1;
        return;
    }
}


/******************************************************************************************************
 * Access L2 cache to get reference
 ******************************************************************************************************/
void queryL2( struct state* state ) {
    
    // Decompose the address for the L2 cache
    struct L2_Reference L2_Ref;
    constructL2Ref( &L2_Ref, state->L1_Index, state->L1_Tag );
    
    // Update index and tag for L2 cache
    state->L2_Tag = L2_Ref.L2_Tag;
    state->L2_Index = L2_Ref.L2_Index;

    // Query L2 cache
    bool hit = queryCache( L2_Ref.L2_Index, L2_Ref.L2_Tag, &L2_unified );

    // Transition based on result
    if( (hit == TRUE) && (state->type == 'W') ) {
        runResults.l2_hit++;
        runResults.numWriteCycles += config.L2_hit_time;
        state->next = HANDLE_WRITE;
        return;
    } else if( hit == TRUE ) {
        if ( state->type == 'R') {
            runResults.numReadCycles += config.L2_hit_time;
        } else {
            runResults.numInstCycles += config.L2_hit_time;
        }
        runResults.l2_hit++;
        state->next = IDLE;
        return;
    } else {
        if ( state->type == 'R') {
            runResults.numReadCycles += config.L2_miss_time + config.L2_hit_time + config.L2_transfer_cycles;
        } else if ( state->type == 'W') {
            runResults.numWriteCycles += config.L2_miss_time + config.L2_hit_time + config.L2_transfer_cycles;
        } else {
            runResults.numInstCycles += config.L2_miss_time + config.L2_hit_time + config.L2_transfer_cycles;
        }

        runResults.l2_miss++;
        state->next = ADD_L2;
        return;
    }
}


/******************************************************************************************************
 * Add tag to L1 cache
 ******************************************************************************************************/
void addL1( struct state* state, struct cache* cache ) {
    
    // Add reference 
    addCache( state->L1_Index, state->L1_Tag, cache ); 

    // Transition
    state->next = QUERY_L2;
    return;

}


/******************************************************************************************************
 * Add tag to L2 cache
 ******************************************************************************************************/
void addL2( struct state* state ) {
    
    // Add reference 
    struct cache* cache = &L2_unified;
    addCache( state->L2_Index, state->L2_Tag, cache ); 

    // Transition
    if( state->type == 'W' ) {
        state->next = HANDLE_WRITE;
        return;
    } else {
        state->next = IDLE;
        return;
    }
}


/******************************************************************************************************
 * Set the dirty bit of tag in L1 cache
 ******************************************************************************************************/
void handleWrite( struct state* state, struct cache* cache) {

    // Set the dirty bit of given cache (L1)
    setDirty( state->L1_Index, state->L1_Tag, cache );

    // Transition
    state->next = IDLE;
    return;
}


/******************************************************************************************************
 * Flush the cache by invalidating all of the data
 ******************************************************************************************************/
void flushCaches( struct state* state ) {;
    
    // Flush the L1_Instruction cache
    flush( &L1_instruction );

    // Flush the L1_Data cache
    flush( &L1_data );

    // Flush the L2_Unified cache
    flush( &L2_unified );

    // Increment results
    runResults.flushes++;
    runResults.invalidates++;

    // Transition
    state->next = QUERY_L1;
    return;
}


/******************************************************************************************************
 * Frees memory used to hold references
 ******************************************************************************************************/
void terminate( struct reference* ref ) {

    // Free reference and index
    free( ref->index );
    free( ref->tag );
}
