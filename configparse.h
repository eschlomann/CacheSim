#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	
// Comment this line to suppress print statements
#define PRINT


#define BUFFER_SIZE				100
#define DELIM					"="


// Struct used to access cache parameters
struct configuration {
	// L1 parameters
	int L1_block_size;
	int L1_cache_size;
	int L1_assoc;
	int L1_hit_time;
	int L1_miss_time;
	// L2 parameters
	int L2_block_size;
	int L2_cache_size;
	int L2_assoc;
	int L2_hit_time;
	int L2_miss_time;
	int L2_transfer_time;
	int L2_bus_width;
};

struct configuration config;
