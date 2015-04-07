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


/******************************************************************************************************
 * Struct to hold calculated results
******************************************************************************************************/
struct results {
	struct configuration config;
	unsigned int memReadyTime;
	unsigned int chunksize;
	unsigned int chunktime;
	unsigned int exTime;
	unsigned int totRefs;
	unsigned int flushTime;
	unsigned int instRefs;
	unsigned int dataRefs;
	unsigned int numReads;
	unsigned int numWrites;
	unsigned int numInst;
	unsigned int numReadCycles;
	unsigned int numWriteCycles;
	unsigned int numInstCycles;
	unsigned int l1i_hit;
	unsigned int l1i_miss;
	unsigned int l1i_total;
	float l1i_hitRate;
	float l1i_missRate;
	unsigned int l1i_kickouts;
	unsigned int l1i_dirtyKickouts;
	unsigned int l1i_transfers;
	unsigned int l1i_flushKickouts;
	unsigned int l1d_hit;
	unsigned int l1d_miss;
	unsigned int l1d_total;
	float l1d_hitRate;
	float l1d_missRate;
	unsigned int l1d_kickouts;
	unsigned int l1d_dirtyKickouts;
	unsigned int l1d_transfers;
	unsigned int l1d_flushKickouts;
	unsigned int l2_hit;
	unsigned int l2_miss;
	unsigned int l2_total;
	float l2_hitRate;
	float l2_missRate;
	unsigned int l2_kickouts;
	unsigned int l2_dirtyKickouts;
	unsigned int l2_transfers;
	unsigned int l2_flushKickouts;
	unsigned int l1_cost;
	unsigned int l2_cost;
	unsigned int mem_cost;
	unsigned int flushes;
	unsigned int invalidates;
};

struct results runResults;

#endif // CONFIGPARSE_H
