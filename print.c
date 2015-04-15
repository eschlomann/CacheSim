#include "print.h"

int printResults ( char* resultsFile ) {
	FILE * fp;
	fp = fopen( resultsFile , "w");
	fprintf(fp,"----------------------------------------------------------------------\n                          Simulation Results                          \n----------------------------------------------------------------------\n\n");
	fprintf(fp,"Memory system:\n  Dcache size = %i : ways = %i : block size = %i \n  Lcache size = %i : ways = %i : block size = %i \n  L2-cache size = %i : ways = %i : block size = %i \n  Memory ready time = %i : chunksize = %i : chunktime = %i",
		runResults.config.L1_cache_size,
		runResults.config.L1_assoc,
		runResults.config.L1_block_size,
		runResults.config.L1_cache_size,
		runResults.config.L1_assoc,
		runResults.config.L1_block_size,
		runResults.config.L2_cache_size,
		runResults.config.L2_assoc,
		runResults.config.L2_block_size,
		runResults.memReadyTime,
		runResults.chunksize,
		runResults.chunktime
	);
	fprintf(fp,"\n\nExecute time = %i; Total refs = %i\nFlush time = %i\nInst refs = %i; Data refs = %i",
		runResults.exTime,
		runResults.totRefs,
		runResults.flushTime,
		runResults.instRefs,
		runResults.dataRefs
	);
	int totalRef = runResults.numReads + runResults.numWrites + runResults.numInst;
	fprintf(fp,"\n\nNumber of Reference Types: [Percentage]\n  Reads  = %11i   [%4.1f%%]\n  Writes = %11i   [%4.1f%%]\n  Inst.  = %11i   [%4.1f%%]\n  Total  = %11i",
		runResults.numReads,
		((float)runResults.numReads / (float)totalRef)*100,
		runResults.numWrites,
		((float)runResults.numWrites / (float)totalRef)*100,
		runResults.numInst,
		((float)runResults.numInst / (float)totalRef)*100,
		totalRef
	);
	int totalCycle = runResults.numReadCycles + runResults.numWriteCycles + runResults.numInstCycles;
	fprintf(fp,"\n\nTotal cycles for activities: [Percentage]\n  Reads  = %11i   [%4.1f%%]\n  Writes = %11i   [%4.1f%%]\n  Inst.  = %11i   [%4.1f%%]\n  Total  = %11i",
		runResults.numReadCycles,
		((float)runResults.numReadCycles / (float)totalCycle)*100,
		runResults.numWriteCycles,
		((float)runResults.numWriteCycles / (float)totalCycle)*100,
		runResults.numInstCycles,
		((float)runResults.numInstCycles / (float)totalCycle)*100,
		totalCycle
	);
	fprintf(fp,"\n\nAverage cycles per activity\n  Read = %.1f; Write = %.1f; Inst. = %.1f",
		(float)runResults.numReadCycles / (float)runResults.numReads,
		(float)runResults.numWriteCycles / (float)runResults.numWrites,
		(float)runResults.numInstCycles / (float)runResults.numInst
	);
	//NO idea what these guys are
	fprintf(fp, "\nIdeal: Exec. Time = %i; CPI = %.1f\nIdeal mis-aligned: Exec. Time = %i; CPI = %.1f", 
		0,
		0.0,
		0,
		0.0
	);
	int l1i_total = runResults.l1i_hit + runResults.l1i_miss;
	fprintf(fp, "\n\nMemory Level:  L1i\n  Hit Count = %i  Miss Count = %i\n  Total Requests = %i\n  Hit Rate = %4.1f%%  Miss Rate = %4.1f%%\n  Kickouts = %i;  Dirty Kickouts = %i;  Transfers = %i\n  Flush Kickouts = %i", 
		runResults.l1i_hit,
		runResults.l1i_miss,
		l1i_total,
		((float)runResults.l1i_hit/(float)l1i_total)*100,
		((float)runResults.l1i_miss/(float)l1i_total)*100,
		runResults.l1i_kickouts,
		runResults.l1i_dirtyKickouts,
		runResults.l1i_transfers,
		runResults.l1i_flushKickouts
	);
	int l1d_total = runResults.l1d_hit + runResults.l1d_miss;
	fprintf(fp, "\n\nMemory Level:  L1d\n  Hit Count = %i  Miss Count = %i\n  Total Requests = %i\n  Hit Rate = %4.1f%%  Miss Rate = %4.1f%%\n  Kickouts = %i;  Dirty Kickouts = %i;  Transfers = %i\n  Flush Kickouts = %i", 
		runResults.l1d_hit,
		runResults.l1d_miss,
		l1d_total,
		((float)runResults.l1d_hit/(float)l1d_total)*100,
		((float)runResults.l1d_miss/(float)l1d_total)*100,
		runResults.l1d_kickouts,
		runResults.l1d_dirtyKickouts,
		runResults.l1d_transfers,
		runResults.l1d_flushKickouts
	);
	int l2_total = runResults.l2_hit + runResults.l2_miss;
	fprintf(fp, "\n\nMemory Level:  L2\n  Hit Count = %i  Miss Count = %i\n  Total Requests = %i\n  Hit Rate = %4.1f%%  Miss Rate = %4.1f%%\n  Kickouts = %i;  Dirty Kickouts = %i;  Transfers = %i\n  Flush Kickouts = %i", 
		runResults.l2_hit,
		runResults.l2_miss,
		l2_total,
		((float)runResults.l2_hit/(float)l2_total)*100,
		((float)runResults.l2_miss/(float)l2_total)*100,
		runResults.l2_kickouts,
		runResults.l2_dirtyKickouts,
		runResults.l2_transfers,
		runResults.l2_flushKickouts
	);
	//Need more here, cost calculations + memory cost
	fprintf(fp, "\n\nL1 cache cost (Icache $%i) + (Dcache $%i) = $%i\nL2 cache cost = $%i;  Memory cost = $%i; Total cost = $%i\nFlushes = %i  :  Invalidates = %i", 
		runResults.l1_cost / 2,
		runResults.l1_cost / 2,
		runResults.l1_cost,
		runResults.l2_cost,
		runResults.mem_cost,
		(runResults.l1_cost + runResults.l2_cost + runResults.mem_cost),
		runResults.flushes,
		runResults.invalidates
	);



	fclose(fp);
	return 0;
}

void printCaches ( char* cacheFile) {
	FILE * fp;
	fp = fopen( cacheFile , "w");
	int i, j;
	int numBlocksL1 = CACHE_SIZE[L1] / BLOCK_SIZE[L1];
	int numBlocksL2 = CACHE_SIZE[L2] / BLOCK_SIZE[L2];
	
	fprintf(fp,  "Memory Level:  L1i\n");
	for ( i = 0; i < numBlocksL1; i++) {
		if ( L1_instruction.block[i].LRU -> count > 0 ) {
			fprintf(fp,  "Index:%5x |", i );
			for( j = 0; j < ASSOC[L1]; j++) {
				if ( (int)L1_instruction.block[i].LRU -> count > j ) {
					fprintf(fp, " V:%d D:%d Tag:%13llx |", 
						L1_instruction.block[i].valid[j] , 
						L1_instruction.block[i].dirty[j] , 
						L1_instruction.block[i].tags[j]
					);
				}
			}
			fprintf(fp, "\n");
		}
	}
	fprintf(fp, "\n");

	fprintf(fp,  "Memory Level:  L1d\n");
	for ( i = 0; i < numBlocksL1; i++) {
		if ( L1_data.block[i].LRU -> count > 0 ) {
			fprintf(fp,  "Index:%5x |", i );
			for( j = 0; j < ASSOC[L1]; j++) {
				if ( (int)L1_data.block[i].LRU -> count > j ) {
					fprintf(fp, " V:%d D:%d Tag:%13llx |", 
						L1_data.block[i].valid[j] , 
						L1_data.block[i].dirty[j] , 
						L1_data.block[i].tags[j]
					);
				}
			}
			fprintf(fp, "\n");
		}
	}
	fprintf(fp, "\n");

	fprintf(fp,  "Memory Level:  L2\n");
	for ( i = 0; i < numBlocksL2; i++) {
		if ( L2_unified.block[i].LRU -> count > 0 ) {
			fprintf(fp,  "Index:%5x |", i );
			for( j = 0; j < ASSOC[L2]; j++) {
				if ( (int)L2_unified.block[i].LRU -> count > j ) {
					fprintf(fp, " V:%d D:%d Tag:%13llx |", 
						L2_unified.block[i].valid[j] , 
						L2_unified.block[i].dirty[j] , 
						L2_unified.block[i].tags[j]
					);
				}
			}
			fprintf(fp, "\n");
		}
	}
	fprintf(fp, "\n");
}






