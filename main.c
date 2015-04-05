// Contains main function to run the simulation
#include "main.h"

int main( int argc, char** argv ) {
	configParse( "config" );	

    int tag = tagSize( config.L1_block_size, config.L1_cache_size, 8 );
    // printf( "tag size: %d \n", tag );
}
