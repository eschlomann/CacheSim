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

    LRUlist* L1i_LRU = makeLRU();
    LRUlist* L1d_LRU = makeLRU();
    LRUlist* L2_LRU  = makeLRU();

	while( scanf ("%c %llx %d\n",&ref.type,&ref.address,&ref.numBytes) == 3) {
		printf("%c %llx %d",ref.type,ref.address,ref.numBytes);
        decomposeAddress( &ref, L1 );
        if( strncmp(&ref.type,"I",1) == 0 ) {
        	printf("\nThe tag is %llx",ref.tag[L1]);
        	LRUpush (L1i_LRU,ref.tag[L1]);
        }
        
	}
	unsigned long long testTAG = 1;
	while (testTAG != 0) {
		testTAG = LRUpop(L1i_LRU);
    	printf("\nThe tag popped off the array is: %llx\n",testTAG);
    }

	runResults.config = config;
	if ( argc < 3) {
		printResults( "resultsFile.results" );
	} else {
		printResults( argv[2] );
	}
}
