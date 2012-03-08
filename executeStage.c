#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "memoryStage.h"
#include "executeStage.h"

static eregister E;
int (*funcArr[16])();

void executeStage(){
    int Cnd = 0x0;
    int valE = (*funcArr[E.icode])();
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

void printEregister() {
    printf("\n=== Execute Stage ===\n");
    printf("stat = %d\ticode = %x\tifun = %x\n", E.stat, E.icode, E.ifun);
    printf("valC = %x\tvalA = %x\tvalB = %x\n", E.valC, E.valA, E.valB);
    printf("dstE = %x\tdstM = %x\tsrcA = %x\tsrcB = %x\n", E.dstE, E.dstM, E.srcA, E.srcB);
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
