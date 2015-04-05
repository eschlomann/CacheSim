// Contains main function to run the simulation
#include "main.h"

int main( int argc, char** argv ) {
	configParse( "config" );	
	printf( "%d \n", config.L1_block_size );
	printf( "%d \n", config.L2_cache_size );
}
