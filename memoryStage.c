#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "memory.h"

#include "writebackStage.h"
#include "memoryStage.h"

static mregister M;

void memoryStage() {
    int valM = M.valA, stat = M.stat;
    bool readC = FALSE, writeC = FALSE, memError;
    memoryControl(&readC, &writeC);
    
    int memAddr = memoryAddr();
    
    if (readC) valM = getWord(memAddr, &memError);
    if (writeC) putWord(memAddr, M.valA, &memError);

    if (memError) stat = SADR;

    updateWregister(M.stat, M.icode, M.valE, valM, M.dstE, M.dstM);
}

void memoryControl(bool *readC, bool *writeC) {
    if (M.icode == MRMOVL || M.icode == POPL || M.icode == RET)
        *readC = TRUE;
    if (M.icode == RMMOVL || M.icode == PUSHL || M.icode == CALL)
        *writeC = TRUE;
}

int memoryAddr() {
    if (M.icode == RMMOVL || M.icode == PUSHL ||
        M.icode == CALL   || M.icode == MRMOVL)
        return M.valE;
    else if (M.icode == POPL || M.icode == RET)
        return M.valA;
    return 0;
}

/* getMregister
 *      Returns a copy of the M register.
 * Params:   none
 * Returns:  mregister
 * Modfiies: none
 */
mregister getMregister() {
    return M;
}

/* clearMregsiter
 *      Clears the contents of the M register.
 * Params:   none
 * Returns:  void
 * Modifies: mregister M
 */
void clearMregister() {
    clearBuffer((char *) &M, sizeof(M));
}

void updateMregister(int stat, int icode, int Cnd, int valE, int valA,
                     int dstE, int dstM){
    M.stat = stat;
    M.icode = icode;
    M.Cnd = Cnd;
    M.valE = valE;
    M.valA = valA;
    M.dstE = dstE;
    M.dstM = dstM;
}

void printMregister() {
    printf("\n=== Memory Stage ===\n");
    printf("stat = %d\ticode = %x\tCnd = %x\n", M.stat, M.icode, M.Cnd);
    printf("valE = %x\tvalA = %x\n", M.valE, M.valA);
    printf("dstE = %x\tdstM = %x\n", M.dstE, M.dstM);
}
