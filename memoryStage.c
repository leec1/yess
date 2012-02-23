#include "memoryStage.h"

// M register holds the input for the memory stage. 
// It is only accessible from this file. (static)
static mregister M;


void memoryStage(){
    //updateWregister(stat, icode, ifun, rA, rB, valC, valP);
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


void updateMregister(int stat, int icode, int ifun, int rA, int rB,
        int valC, int valP){

}
