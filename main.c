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

unsigned int e_dstE, e_valE;
unsigned int M_dstM, M_dstE;
unsigned int m_valM, M_valE;
unsigned int W_dstM, W_dstE;
unsigned int W_valM, W_valE;
unsigned int M_Cnd;
unsigned int M_icode;
unsigned int M_valA;
unsigned int W_icode;

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
    
    if (!load()) {
        dumpMemory();
        return 0;
    }
    
    while(!stop) {
        stop = writebackStage(&W_dstE, &W_valE, &W_icode);
        memoryStage(&W_valE, &W_valM, &W_dstE, &W_dstM, &m_valM,
                    &M_dstE, &M_dstM, &M_valE, &M_Cnd, &M_icode,
                    &M_valA);
        executeStage(&e_dstE, &e_valE);
        decodeStage(&W_dstE, &W_valE, &e_dstE, &e_valE, &M_dstM,
                    &m_valM, &M_dstE, &M_valE, &W_dstM, &W_valM);
        fetchStage(&M_Cnd, &M_icode, &M_valA, &W_icode);
        clockCount++;
    }

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
    clockCount = 0;
    stop = FALSE;
    
    W_valE = 0;
    W_dstE = 0;
    
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
