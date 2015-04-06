// Contains main function to run the simulation
#include "main.h"

int main( int argc, char** argv ) {
	configParse( "config" );	

    int tag = tagSize( config.L2_block_size, config.L2_cache_size * 2, 8 );

    struct cacheBlock block;
    block.valid = FALSE;
    block.dirty = FALSE;
    block.tag = 0;
}
