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

unsigned int W_valE;
unsigned int W_dstE;

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
    
    clockCount = 0;
    stop = FALSE;
    while(!stop) {
        stop = writebackStage(&W_dstE, &W_valE);
        memoryStage();
        executeStage();
        decodeStage(W_dstE, W_valE);
        fetchStage();
        clockCount++;
    }

    //dumpProgramRegisters();
    //dumpProcessorRegisters();
    //dumpMemory(); 
    
    printf("\nTotal clock cycles = %d\n", clockCount);

    return 0;
}

/* initialize
 *      Initializes a bunch of stuff.
 * Params:   none
 * Returns:  void
 * Modifies: W_valE, W_dstE
 */
void initialize() {
    W_valE = 0; W_dstE = 0;
    initializeFuncPtrArray();
    initializeCC();
    clearMemory();
    clearRegisters();
    clearFregister();
    clearDregister();
    clearEregister();
    clearMregister();
    clearWregister();
}
