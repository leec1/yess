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

static fwdStruct fwd;


void initialize();
void initFwdStruct();

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
        stop = writebackStage(&fwd);
        memoryStage(&fwd);
        executeStage(&fwd);
        decodeStage(&fwd);
        fetchStage(&fwd);
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
    
    initFwdStruct();    
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

/* initFwdStuct
 *      Initializes the fowarding struct.
 * Params:  none
 * Returns: void
 * Modifies: fwd
 */
void initFwdStruct() {
    //fetch
    fwd.d_srcA = 0;
    fwd.d_srcB = 0;
    
    //decode
    fwd.D_icode = 0;
    
    //execute
    fwd.E_icode = 0;
    fwd.e_Cnd = 0;
    fwd.e_dstE = 0;
    fwd.e_valE = 0;
    fwd.E_dstM = 0;
    
    //memory
    fwd.M_icode = 0;
    fwd.M_Cnd = 0;
    fwd.m_stat = 0;
    fwd.M_dstM = 0;
    fwd.M_dstE = 0;
    fwd.m_valM = 0;
    fwd.M_valE = 0;
    fwd.M_valA = 0;
    
    //writeback
    fwd.W_icode = 0;
    fwd.W_stat = 0;
    fwd.W_dstM = 0;
    fwd.W_dstE = 0;
    fwd.W_valM = 0;
    fwd.W_valE = 0;
}
