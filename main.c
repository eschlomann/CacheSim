// Contains main function to run the simulation
#include "main.h"

int main( int argc, char** argv ) {

	if( argc < 2 ) {
		configParse( "config" );
	} else {
		configParse( argv[1] );
	}

    // Initialize cache structures
    constructCache( &L1_instruction , L1 );
    constructCache( &L1_data , L1 );
    constructCache( &L2_unified , L2 );


	while( scanf ("%c %llx %d\n",&ref.type,&ref.address,&ref.numBytes) == 3) {
		// printf("\n%c %llx %d\n\n",ref.type,ref.address,ref.numBytes);
       
        // Begin the statemachine
        stateMachine( &ref );
	}

	printCaches( "cacheResults.results" );
	
    printf ("calculating cost");
    runResults.l1_cost = 2 * (100 * (config.L1_cache_size/4096) + 100 * (log(config.L1_assoc) / log(2)) * (config.L1_cache_size/4096));
    runResults.l2_cost = (int)(50 * ((float) config.L2_cache_size/65536) + 50 * (log(config.L2_assoc) / log(2)) * ((float)config.L2_block_size/65536));
    runResults.mem_cost = 50 + 200 * (log ( 50 / config.mem_ready )) / log(2) + 25 + 100 * (log (config.mem_chunksize/16)) / log(2);

	runResults.m_config = config;
	if ( argc < 3) {
		printResults( "resultsFile.results" );
	} else {
		printResults( argv[2] );
	}

    return 1;
}
