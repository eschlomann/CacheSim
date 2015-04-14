// Contains main function to run the simulation
#include "main.h"

int main( int argc, char** argv ) {
	//TESTING PRINT FUNCTION PURPOSES ONLY (it really doesnt do much)
	const struct results defaultResults = { config , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
		100 , 20 , 300 , 1000
		, 200 , 3000 , 0 , 0 , 0 , 0.0 , 0.0 , 0 , 0 , 0 , 0 , 0
		, 0 , 0 , 0.0 , 0.0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0.0
		, 0.0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
	runResults = defaultResults;
	//END TESTING PRINT FUNCTION PURPOSES
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
		printf("\n%c %llx %d\n\n",ref.type,ref.address,ref.numBytes);
        decomposeAddress( &ref, L1 );
        if( strncmp(&ref.type,"I",1) == 0 ) {
        	//printf("\nThe tag is %llx\n",ref.tag[L1]);
        	//printf("The index is %llu\n",ref.index[L1]);

        	if ( queryCache( &ref, &L1_instruction ) ) {
        		printf ("HIT\n\n\n");
        	} else {
        		printf ("MISS\n\n\n");
        	}

        	/*if ( LRUcheckDestroyPush( L1_instruction.block[ref.index[L1]].LRU , ref.tag[L1] ) ) {
        		printf ("HIT\n\n\n");
        	} else {
        		printf ("MISS\n\n\n");
        	}*/
        }
        
	}

	runResults.config = config;
	if ( argc < 3) {
		printResults( "resultsFile.results" );
	} else {
		printResults( argv[2] );
	}
}
