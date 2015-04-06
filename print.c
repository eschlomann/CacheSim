#include "print.h"

int printResults ( char* resultsFile ) {
	FILE * fp;
	fp = fopen( resultsFile , "w");	
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



	fclose(fp);
	return 0;
}