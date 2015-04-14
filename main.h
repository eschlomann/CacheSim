#include <stdio.h>
#include "configparse.h"
#include "cache.h"
#include "print.h"
#include "statemachine.h"

/*struct results {
	struct configuration config;
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

	unsigned int flushes;
	unsigned int invalidates;
};

struct results runResults;
*/
//	/* data */

// Comment this line to suppress print statements
#define PRINT

