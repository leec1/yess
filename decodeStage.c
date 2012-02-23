#include "decodeStage.h"

// D register holds the input for the decode stage.
// It is only accessible from this file. (static)
static dregister D;

void decodeStage(){

}

/* getDregister
 *     Returns a copy of the D regsiter.
 * Params:   none
 * Returns:  dregsiter
 * Modifies: none
 */
dregister getDregister() {
    return D;
}

/* chearDregister
 *      Clears the contents of the D register.
 * Params:   none 
 * Returns:  void
 * Modifies: D
 */
void clearDregister() {
    clearBuffer((char *) &D, sizeof(D));
}

void updateDregister(int stat, int icode, int ifun, int rA, int rB, 
        int valC, int valP){
    D.stat = stat;
    D.icode = icode;
    D.ifun = ifun;
    D.rA = rA;
    D.rB = rB;
    D.valC = valC;
    D.valP = valP;
}
