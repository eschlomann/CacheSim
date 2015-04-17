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
	unsigned int exTime;
	unsigned int flushTime;
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


#endif // PRINT_H
