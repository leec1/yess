#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "memory.h"

#include "writebackStage.h"
#include "memoryStage.h"

static mregister M;
static bool canUpdateMem;

bool W_stall(fwdStruct *fwd);

/* memoryStage
 *      Controls the main combinational logic of the memory stage
 * Params:   //TODO: fix
 * Returns:  void
 * Modifies: Writeback Register
 */
/*void memoryStage(unsigned int *m_valM, unsigned int *M_dstE,
                 unsigned int *M_dstM, unsigned int *M_valE,
                 unsigned int *M_Cnd, unsigned int *M_icode,
                 unsigned int *M_valA) {*/
void memoryStage(fwdStruct *fwd) {

    if (M.stat == SINS || M.stat == SADR || M.stat == SHLT)
        canUpdateMem = FALSE;
    int valM = M.valA, stat = M.stat;
    bool readC = FALSE, writeC = FALSE, memError = FALSE;
    
    memoryControl(&readC, &writeC);
    
    int memAddr = memoryAddr();
    
    if (readC)
        valM = getWord(memAddr, &memError);

    if(memError) {
        stat = SADR;
        canUpdateMem = FALSE;
    }
    
    if (writeC && canUpdateMem) putWord(memAddr, M.valA, &memError);

    if (memError){
        stat = SADR;
        canUpdateMem = FALSE;
    }
    
    fwd->M_valE = M.valE;
    fwd->m_valM = valM;
    fwd->M_dstE = M.dstE;
    fwd->M_dstM = M.dstM;
    fwd->M_Cnd = M.Cnd;
    fwd->M_icode = M.icode;
    fwd->M_valA = M.valA;
    fwd->m_stat = stat;

    //printf("setting fwd->M_valA to %d\n", fwd->M_valA);
    if(W_stall(fwd)){
    }else{
        updateWregister(stat, M.icode, M.valE, valM, M.dstE, M.dstM);
    }
}

/* memoryControl
 *      Determines if the current instruction reads and/or writes to memory
 * Params:   *readC - True if Instruction will read from memory
 *           *writeC - True if Instruction will write to memory
 * Returns:  void
 * Modifies: *readC, *writeC
 */
void memoryControl(bool *readC, bool *writeC) {
    if (M.icode == MRMOVL || M.icode == POPL || M.icode == RET)
        *readC = TRUE;
    if (M.icode == RMMOVL || M.icode == PUSHL || M.icode == CALL)
        *writeC = TRUE;
}

/* memoryAddr
 *      Returns the location of the memory to be overwritten
 * Params:   none
 * Returns:  int 
 * Modifies: none
 */
int memoryAddr() {
    if (M.icode == RMMOVL || M.icode == PUSHL ||
        M.icode == CALL   || M.icode == MRMOVL)
        return M.valE;
    else if (M.icode == POPL || M.icode == RET)
        return M.valA;
    return 0;
}

bool W_stall(fwdStruct *fwd){
    return fwd->W_stat == SADR || fwd->W_stat == SINS || fwd->W_stat == SHLT;
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
    canUpdateMem = TRUE;
    M.stat = SAOK;
    M.icode = NOP;
}

void updateMregister(int stat, int icode, int Cnd, int valE, int valA,
                     int dstE, int dstM) {
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
