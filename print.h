#ifndef PRINT_H
#define PRINT_H

#include "configparse.h"
#include "cache.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************************
 * Print cache to verify entries
 ******************************************************************************************************/
void printCaches( char* cacheFile);


/******************************************************************************************************
 * Function to print results to a file of choosing, uses the results structure
 ******************************************************************************************************/
int printResults( char* );


/******************************************************************************************************
 * Struct to hold calculated results
******************************************************************************************************/
struct results {
	struct configuration m_config;
	unsigned long long exTime;
	unsigned long long flushTime;
	unsigned long long numReads;
	unsigned long long numWrites;
	unsigned long long numInst;
	unsigned long long numReadCycles;
	unsigned long long numWriteCycles;
	unsigned long long numInstCycles;
	unsigned long long l1i_hit;
	unsigned long long l1i_miss;
	unsigned long long l1i_total;
	float l1i_hitRate;
	float l1i_missRate;
	unsigned long long l1i_kickouts;
	unsigned long long l1i_dirtyKickouts;
	unsigned long long l1i_transfers;
	unsigned long long l1i_flushKickouts;
	unsigned long long l1d_hit;
	unsigned long long l1d_miss;
	unsigned long long l1d_total;
	float l1d_hitRate;
	float l1d_missRate;
	unsigned long long l1d_kickouts;
	unsigned long long l1d_dirtyKickouts;
	unsigned long long l1d_transfers;
	unsigned long long l1d_flushKickouts;
	unsigned long long l2_hit;
	unsigned long long l2_miss;
	unsigned long long l2_total;
	float l2_hitRate;
	float l2_missRate;
	unsigned long long l2_kickouts;
	unsigned long long l2_dirtyKickouts;
	unsigned long long l2_transfers;
	unsigned long long l2_flushKickouts;
	unsigned int l1_cost;
	unsigned int l2_cost;
	unsigned int mem_cost;
	unsigned long long flushes;
	unsigned long long invalidates;
};

struct results runResults;


#endif // PRINT_H
