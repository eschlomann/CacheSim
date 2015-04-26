#include "configparse.h"

/******************************************************************************************************
 * Parses the given configuration file and fills in the values of the configuration struct
******************************************************************************************************/
int configParse( char* configFile ) {
    int count = 0;
	int value = 0;
	FILE* fp;
	char buffer[BUFFER_SIZE];
    // Open configuration file
    if( (fp = fopen(configFile, "r")) != NULL ) {
        // Iterate over each line
        while( fgets(buffer, BUFFER_SIZE, fp) != NULL ) {
			if( *buffer == '#' ) {
                #ifdef PRINT
				printf( "%s", buffer );
                #endif
			} else {
            	count += 1;
				// Convert string to an int
				value = (int) strtol(buffer, (char**)NULL, 10);
                #ifdef PRINT
            	printf( "%d: %d \n",count, value );
                #endif

				// Basic implementation expects certain lines to contain the correct variables and doesn't make any check
				if( count == 1 ) {
					config.L1_block_size = value;	
				} else if( count ==2 ) {
					config.L1_cache_size = value;		
				} else if( count ==3 ) {
					config.L1_assoc = value;
				} else if( count ==4 ) {
					config.L1_hit_time = value;
				} else if( count ==5 ) {
					config.L1_miss_time = value;
				} else if( count ==6 ) {
					config.L2_block_size = value;
				} else if( count ==7 ) {
					config.L2_cache_size = value;		
				} else if( count ==8 ) {
					config.L2_assoc = value;	
				} else if( count ==9 ) {
					config.L2_hit_time = value;
				} else if( count ==10 ) {
					config.L2_miss_time = value;
				} else if( count ==11 ) {
					config.L2_transfer_time = value;
				} else if( count ==12 ) {
					config.L2_bus_width = value;
				} else if( count ==13 ) {
                    config.mem_sendaddr = value;
                } else if( count ==14 ) {
                    config.mem_ready = value;
                } else if( count ==15 ) {
                    config.mem_chunktime = value;
                } else if( count ==16 ) {
                    config.mem_chunksize = value;
                }
			}
        }
    }
    fclose(fp);

    // Define the values of the arrays
    BLOCK_SIZE[0] = config.L1_block_size;
    BLOCK_SIZE[1] = config.L2_block_size;

    CACHE_SIZE[0] = config.L1_cache_size;
    CACHE_SIZE[1] = config.L2_cache_size;

    ASSOC[0] = config.L1_assoc;
    ASSOC[1] = config.L2_assoc;

    defineAddressParameters( 0 );
    defineAddressParameters( 1 );

    config.L1_transfer_cycles = config.L2_transfer_time * (config.L1_block_size/L2_bus_width);
    config.L2_transfer_cycles = mem_sendaddr + mem_ready + mem_chunktime * (L2_block_size/mem_chunksize);

	return 0;
}


/******************************************************************************************************
 * Calculates and sets TAG_SIZE and INDEX_SIZE arrays for given cache 
 ******************************************************************************************************/
void defineAddressParameters( cache_TypeDef cache ) {
    // Calculate number of blocks in the cache
    int numBlocks = CACHE_SIZE[cache] / BLOCK_SIZE[cache];
    #ifdef PRINT
    printf( "Blocks in cache %d: %d \n", cache, numBlocks );
    #endif

    // Calculate number of bits used for the index (ignoring associativity)
    int bitsIndex = log(numBlocks) / log(2);
    #ifdef PRINT
    printf( "Number of index bits (%d): %d \n", cache, bitsIndex );
    #endif

    // Adjust for associativity
    int bitsAssociative = log(ASSOC[cache]) / log(2);
    bitsIndex = bitsIndex - bitsAssociative;
    #ifdef PRINT
    printf( "Number of associative bits (%d): %d \n", cache, bitsAssociative );
    printf( "Adjusted number of index bits (%d): %d \n", cache, bitsIndex );
    #endif

    // Calculate number of bits used to address each byte in the block
    int bitsBlock = log(BLOCK_SIZE[cache]) / log(2);
    #ifdef PRINT
    printf( "Number of block bits (%d): %d \n", cache, bitsBlock );
    #endif
    
    // Size of the address string
    int numAddress = 48;

    // Calculate how many bits are left over from the address
    int bitsTag = numAddress - bitsIndex - bitsBlock;
    #ifdef PRINT
    printf( "Number of tag bits (%d): %d \n", cache, bitsTag );
    #endif

    TAG_SIZE[cache] = bitsTag;
    INDEX_SIZE[cache] = bitsIndex;

    // Create bit mask to get index
    int i;
    unsigned long long mask = 0;
    unsigned long long tempMask = 0;
    for( i = 0; i < bitsIndex; i++ ) {
        tempMask = 1 << i;
        mask = (mask | tempMask);
    }
    #ifdef PRINT
    printf( "Index mask: %lld \n",mask );
    #endif
    INDEX_MASK[cache] = mask;

    // Create bit mask to get bytes
    mask = 0;
    tempMask = 0;
    for( i = 0; i < bitsBlock; i++ ) {
        tempMask = 1 << i;
        mask = (mask | tempMask);
    }
    #ifdef PRINT
    printf( "Byte mask: %lld \n",mask );
    #endif
    BYTE_MASK[cache] = mask;


}

