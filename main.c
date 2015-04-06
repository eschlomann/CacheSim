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
	runResults.config = config;
	printResults( "resultsFile" );	
}
