#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "executeStage.h"
#include "decodeStage.h"

static dregister D;

void decodeStage(unsigned int W_dstE, unsigned int W_valE){
    int valA = 0;
    int valB = 0;
    int dstE = 0xf;
    int dstM = 0xf;
    int srcA = 0xf;
    int srcB = 0xf;
    updateEregister(D.stat, D.icode, D.ifun, D.valC, valA, valB, dstE,
                    dstM, srcA, srcB);
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

void printDregister() {
    printf("\n=== Decode Stage ===\n");
    printf("stat = %d\ticode = %x\tifun = %x\n", D.stat, D.icode, D.ifun);
    printf("rA = %x\trB = %x\n", D.rA, D.rB);
    printf("valC = %x\tvalP = %x\n", D.valC, D.valP);
}
