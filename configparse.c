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
				printf( "%s", buffer );
			} else {
            	count += 1;
				// Convert string to an int
				value = (int) strtol(buffer, (char**)NULL, 10);
            	printf( "%d: %d \n",count, value );

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
				}
			}
        }
    }
	return 0;
}
