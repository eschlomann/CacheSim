#include "statemachine.h"


/******************************************************************************************************
 * Makes call to state machine
 ******************************************************************************************************/
void stateMachine( struct reference* ref ) {

    // Find which L1 cache to access
    struct cache L1_cache = cacheType( ref );

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

            case HANDLE_WRITE:
                // Set dirty bit in L1 cache
                #ifdef PRINT
                printf( "Handle write case \n" );
                #endif
                state = handleWrite( ref, &L1_cache );
                break;
        }
    }
}


/******************************************************************************************************
 * Selects correct L1 cache based on reference type
 ******************************************************************************************************/
struct cache cacheType( struct reference* ref ) {

    if( ref->type == 'I' ) {
        runResults.numInst++;

        #ifdef PRINT
        printf( "L1 Instruction Cache: %c \n", ref->type );
        decomposeAddress( ref, L1 );
        printf( "Tag: %lld \n", ref->tag[L1][0] );
        printf( "Index %lld \n", ref->index[L1][0] );
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
        decomposeAddress( ref, L1 );
        printf( "Tag: %lld \n", ref->tag[L1][0] );
        printf( "Index %lld \n", ref->index[L1][0] );
        #endif
        
        return L1_data;

    } else {
        printf( "Unrecognized reference type" );
        return L1_instruction;
    }
}


/******************************************************************************************************
 * Query L1 cache to get reference
 ******************************************************************************************************/
int queryL1( struct reference* ref, struct cache* cache ) {

    // Decompose the address for the L1 cache
    decomposeAddress( ref, L1 );

    // Query L1 cache
    bool hit = queryCache( ref->index[cache->type][0], ref->tag[cache->type][0], cache );

    // Transition based on result
    if( (hit == TRUE) && (ref->type == 'W') ) {
        runResults.l1d_hit++;
        runResults.numWriteCycles += config.L1_hit_time;
        return HANDLE_WRITE;
    } 
    else if( hit == TRUE) {
        if (ref-> type == 'R') {
            runResults.l1d_hit++;
            runResults.numReadCycles += config.L1_hit_time;
        } else {
            runResults.l1i_hit++;
            runResults.numInstCycles += config.L1_hit_time;
        }
        return IDLE;
    } 
    else {
        if (ref->type == 'I') {
            runResults.l1i_miss++;
            runResults.numInstCycles += config.L1_miss_time;
        } else if (ref->type == 'W') {
            runResults.l1d_miss++;
            runResults.numWriteCycles += config.L1_miss_time;
        } else {
            runResults.l1d_miss++;
            runResults.numReadCycles += config.L1_miss_time;
        }
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
    struct cache* cache = &L2_unified;
    bool hit = queryCache( ref->index[cache->type][0], ref->tag[cache->type][0], cache );

    // Transition based on result
    int trasftertime;
    if( hit == TRUE ) {
        trasftertime = config.L2_transfer_time * ceil( (float)ref->numBytes / config.L2_bus_width );
        runResults.l2_hit++;
        if (ref->type == 'I') {
            runResults.numInstCycles += config.L2_hit_time + trasftertime;
        } else if (ref->type == 'W') {
            runResults.numWriteCycles += config.L2_hit_time + trasftertime;
        } else {
            runResults.numReadCycles += config.L2_hit_time + trasftertime;
        }
        return ADD_L1;
    } else {
        trasftertime = config.mem_sendaddr + config.mem_ready + (config.mem_chunktime * ceil( (float)ref->numBytes / config.mem_chunksize ));
        runResults.l2_miss++;
        if (ref->type == 'I') {
            runResults.numInstCycles += config.L2_miss_time + trasftertime;
        } else if (ref->type == 'W') {
            runResults.numWriteCycles += config.L2_miss_time + trasftertime;
        } else {
            runResults.numReadCycles += config.L2_miss_time + trasftertime;
        }
        return ADD_L2;
    }
}


/******************************************************************************************************
 * Add tag to L1 cache
 ******************************************************************************************************/
int addL1( struct reference* ref, struct cache* cache ) {
    
    // Add reference 
    addCache( ref->index[cache->type][0], ref->tag[cache->type][0], cache ); 

    // Transition
    if( ref->type == 'W' ) {
        return HANDLE_WRITE;
    } else {
        return IDLE;
    }
}


/******************************************************************************************************
 * Add tag to L2 cache
 ******************************************************************************************************/
int addL2( struct reference* ref ) {
    
    // Add reference 
    struct cache* cache = &L2_unified;
    addCache( ref->index[cache->type][0], ref->tag[cache->type][0], cache ); 

    // Transition
    return ADD_L1;
}


/******************************************************************************************************
 * Set the dirty bit of tag in L1 cache
 ******************************************************************************************************/
int handleWrite( struct reference* ref, struct cache* cache) {

    // Set the dirty bit of given cache
    setDirty( ref->index[cache->type][0], ref->tag[cache->type][0], cache );

    // Transition
    return IDLE;
}


/******************************************************************************************************
 * Flush the cache by invalidating all of the data
 ******************************************************************************************************/
int flushCache( struct cache* cache ) {;
    
    // Flush the given cache
    flush( cache );

    // Transition
    return IDLE;
}


