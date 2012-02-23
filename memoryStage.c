#include "memoryStage.h"

// M register holds the input for the memory stage. 
// It is only accessible from this file. (static)
static mregister M;


void memoryStage(){
    updateWregister(M.stat, M.icode, M.valE, M.valA, M.dstE, M.dstM);
}

/* getMregister
 *      Returns a copy of the M register.
 * Params:   none
 * Returns:  eregister
 * Modfiies: none
 */
mregister getMregister() {
    return M;
}

/* clearMregsiter
 *      Clears the contents of the M register.
 * Params:   none
 * Returns:  void
 * Modifies: M
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
