#include <stdio.h>
#include <limits.h>
#include "types.h"
#include "tools.h"
#include "memory.h"
#include "registers.h"
#include "loader.h"
#include "dump.h"

#include "fetchStage.h"
#include "decodeStage.h"
#include "executeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"

int clockCount;
bool stop;

void initialize();

int main(int argc, char * argv[]) {
    if (argc < 2) {
        printf("Please provide a filename.\n");
        return 0;
    }

    if (!openFile(argv[1])) {
        printf("Failed to open file.\nUsage: yess <filename>.yo\n");
        return 0;
    }
    
    initialize();
    
    if (!load()) {//printf("\n");
        dumpMemory();
        return 0;
    }
    
    //initializeFuncPtrArray();
     
    clockCount = 0;
    stop = FALSE;
    while(!stop) {
        /*printFregister();
        printDregister();
        printEregister();
        printMregister();
        printWregister();*/
        stop = writebackStage();
        //printf("clock: %d\tstop?: %d\n", clockCount, stop);
        memoryStage();
        executeStage();
        decodeStage();
        fetchStage();
        clockCount++;
    }

    //dumpProgramRegisters();
    //dumpProcessorRegisters();
    //dumpMemory(); 
    
    printf("\nTotal clock cycles = %d\n", clockCount);

    return 0;
}

void initialize() {
    initializeFuncPtrArray();
    clearMemory();
    clearRegisters();
    clearFregister();
    clearDregister();
    clearEregister();
    clearMregister();
    clearWregister();
}
