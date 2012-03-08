#include "types.h"
#include "executeStage.h"
#include <stdio.h>


// E register holds the input for the execute stage.
// It is only accessible from this file. (static)
static eregister E;
int (*funcArr[16])();

void executeStage(){
    int Cnd = 0x0;
    int valE = (*funcArr[E.icode])();
    //printf("%d", valE);
    updateMregister(E.stat, E.icode, Cnd, valE, E.valA, E.dstE, E.dstM);
}

/* getEregister
 *      Returns a copy of the eregister.
 * Params:   none
 * Returns:  eregister
 * Modifies: none
 */
eregister getEregister() {
    return E;
}

/* clearEregister
 *      Clears the contents of the E register.
 * Params:   none
 * Returns:  void
 * Modifies: E
 */
void clearEregister() {
    clearBuffer((char *) &E, sizeof(E));
}


void updateEregister(int stat, int icode, int ifun, int valC, int valA,
                     int valB, int dstE, int dstM, int srcA, int srcB){
    E.stat = stat;
    E.icode = icode;
    E.ifun = ifun;
    E.valC = valC;
    E.valA = valA;
    E.valB = valB;
    E.dstE = dstE;
    E.dstM = dstM;
    E.srcA = srcA;
    E.srcB = srcB;
}

void initializeFuncPtrArray(){
    funcArr[HALT] = &doNothing;
    funcArr[NOP] = &doNothing;
    funcArr[CMOV] = &doNothing;
    funcArr[IRMOVL] = &doNothing;
    funcArr[RMMOVL] = &doNothing;
    funcArr[MRMOVL] = &doNothing;
    funcArr[OPL] = &doNothing;
    funcArr[JXX] = &doNothing;
    funcArr[CALL] = &doNothing;
    funcArr[RET] = &doNothing;
    funcArr[PUSHL] = &doNothing;
    funcArr[POPL] = &doNothing;
    funcArr[DUMP] = &dump;
}

int doNothing(){
    return 0;
}

int dump(){
    return E.valC;
}
