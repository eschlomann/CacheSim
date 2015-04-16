#ifndef CONFIGPARSE_H
#define CONFIGPARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

	
// Comment this line to suppress print statements
// #define PRINT

#define BUFFER_SIZE				100
#define DELIM					"="


// Defines L1 and L2 parameters
typedef enum {
    L1 = 0,
    L2 = 1
} cache_TypeDef;


/******************************************************************************************************
 * Parses the given configuration file and fills in the values of the configuration struct
******************************************************************************************************/
int configParse( char* ); 


/******************************************************************************************************
 * Calculates and sets TAG_SIZE and INDEX_SIZE arrays for given cache 
 ******************************************************************************************************/
void defineAddressParameters( cache_TypeDef );


void calculateCost();

/******************************************************************************************************
 * Struct used to access cache parameters
******************************************************************************************************/
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


/******************************************************************************************************
 * Arrays used to index the configuration parameters using a single cache parameter
******************************************************************************************************/
int BLOCK_SIZE[2];
int CACHE_SIZE[2];
int ASSOC[2];
int TAG_SIZE[2];
int INDEX_SIZE[2];
unsigned long long INDEX_MASK[2];
unsigned long long BYTE_MASK[2];



#endif // CONFIGPARSE_H
