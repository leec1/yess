#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "registers.h"
#include "memoryStage.h"
#include "executeStage.h"

static eregister E;
int (*funcArr[16])();
bool Cnd;

/* executeStage
 *      Handles the main combination logic of the execute stage.
 * Params:   none
 * Returns:  void
 * Modifies: Cnd
 */
void executeStage() {
    Cnd = FALSE;
    int valE = (*funcArr[E.icode])();
    int dstE = E.dstE;
    if (E.icode == CMOV && !Cnd)
        dstE = RNONE;
    updateMregister(E.stat, E.icode, Cnd, valE, E.valA, dstE, E.dstM);
}

//placeholder function for the function pointer array
int doNothing() {
    return 0;
}

/* performOpl
 *      Simulates the OPL instruction. Checks E.ifun and performs the correct
 *      operation based on the icode given. Also updates condition codes. If
 *      an invald function code is provided, sets E.stat to SINS.
 * Params:   none
 * Returns:  int - the result of the desired operation
 * Modifies: E.stat
 */
int performOpl() {
    int val = 0;
    switch (E.ifun) {
        case ADD:
            val = E.valB + E.valA;
            if ((isNeg(E.valB) && isNeg(E.valA) && !isNeg(val))  ||
                (!isNeg(E.valB) && !isNeg(E.valA) && isNeg(val)))
                setCC(OF, 1);
            else setCC(OF, 0);
            break;
        case SUB:
            val = E.valB - E.valA;
            if ((isNeg(E.valB) && !isNeg(E.valA) && !isNeg(val)) ||
                (!isNeg(E.valB) && isNeg(E.valA) && isNeg(val)))
                setCC(OF, 1);
            else setCC(OF, 0);
            break;
        case AND:
            setCC(OF, 0);
            val = E.valB & E.valA;
            break;
        case XOR:
            setCC(OF, 0);
            val =  E.valB ^ E.valA;
            break;
        default:
            E.stat = SINS;
            break;
    }
    updateCC(val);
    return val;
}

/* performRrmovl
 *      Simulates the RRMOVL/CMOVXX instruction. Checks E.ifun and performs the
 *      correct type of move based on the icode given. Updates Cnd to reflect
 *      whether or not a conditional instruction should actually occur.
 * Params:   none
 * Returns:  int - E.valA
 * Modifies: Cnd
 */
int performRrmovl() {
    switch (E.ifun) {
        case RRMOVL:
            Cnd = TRUE;
            break;
        case CMOVLE:
            Cnd = (getCC(SF) && !getCC(OF)) || (!getCC(SF) && getCC(OF)) || getCC(ZF);
            break;
        case CMOVL:
            Cnd = (getCC(SF) && !getCC(OF)) || (!getCC(SF) && getCC(OF));
            break;
        case CMOVE:
            Cnd = getCC(ZF);
            break;
        case CMOVNE:
            Cnd = !getCC(ZF);
            break;
        case CMOVGE:
            Cnd = (getCC(SF) && getCC(OF)) || (!getCC(SF) && !getCC(OF));
            break;
        case CMOVG:
            Cnd = (getCC(SF)  && getCC(OF)  && !getCC(ZF)) ||
                  (!getCC(SF) && !getCC(OF) && !getCC(ZF));
            break;
        default:
            Cnd = FALSE;
            break;
    }
    return E.valA;
}

/* performIrmovl
 *      Simulates the IRMOVL instruction. Well, kinda. Doesn't really do much.
 * Params:   none
 * Returns:  int - E.valC
 * Modifies: none
 */
int performIrmovl() {
    return E.valC;
}

/* performRmmovl
 *      Simulates the RMMOVL instruction. Again, just adding two numbers here.
 * Params:   none
 * Returns:  int - E.valC + E.valB
 * Modifies: none
 */
int performRmmovl() {
    return E.valC + E.valB;
}

/* performMrmovl
 *      Simulates the MRMOVL instruction. I dunno who I'm kidding anymore.
 * Params:   none
 * Returns:  int - E.valC + E.valB
 * Modifies: none
 */
int performMrmovl() {
    return E.valC + E.valB;
}

/* performPop
 *      Simulates the POPL instruction.  
 * Params:   none
 * Returns:  int - 4+ E.valB
 * Modifies: Cnd
 */
int performPop(){
    Cnd = FALSE;
    return 4 + E.valB;
}

/* performPush
 *      Simulates the PUSHL instruction.
 * Params:   none
 * Returns:  int - E.valB -4
 * Modifies: Cnd
 */
int performPush(){
    Cnd = FALSE;
    return -4 + E.valB;
}

/* performDump
 *      Simulates the DUMP instruction. *sigh*
 * Params:   none
 * Returns:  int - E.valC
 * Modifies: none
 */
int performDump() {
    return E.valC;
}

/* updateCC
 *      Helper method to update ZF and SF condition codes.
 * Params:   int val - the value to set the CCs based on
 * Returns:  void
 * Modifies: none
 */
void updateCC(int val) {    
    if (val == 0) {
        setCC(ZF, 1);
        setCC(SF, 0);
    } else if (isNeg(val)) {
        setCC(ZF, 0);
        setCC(SF, 1);
    } else {
        setCC(ZF, 0);
        setCC(SF, 0);
    }
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
 * Modifies: eregister E
 */
void clearEregister() {
    clearBuffer((char *) &E, sizeof(E));
}

void updateEregister(int stat, int icode, int ifun, int valC, int valA,
                     int valB, int dstE, int dstM, int srcA, int srcB) {
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

void initializeFuncPtrArray() {
    funcArr[HALT] = &doNothing;
    funcArr[NOP] = &doNothing;
    funcArr[CMOV] = &performRrmovl;
    funcArr[IRMOVL] = &performIrmovl;
    funcArr[RMMOVL] = &performRmmovl;
    funcArr[MRMOVL] = &performMrmovl;
    funcArr[OPL] = &performOpl;
    funcArr[JXX] = &doNothing;
    funcArr[CALL] = &doNothing;
    funcArr[RET] = &doNothing;
    funcArr[PUSHL] = &performPush;
    funcArr[POPL] = &performPop;
    funcArr[DUMP] = &performDump;
}

void printEregister() {
    printf("\n=== Execute Stage ===\n");
    printf("stat = %d\ticode = %x\tifun = %x\n", E.stat, E.icode, E.ifun);
    printf("valC = %x\tvalA = %x\tvalB = %x\n", E.valC, E.valA, E.valB);
    printf("dstE = %x\tdstM = %x\tsrcA = %x\tsrcB = %x\n", E.dstE, E.dstM, E.srcA, E.srcB);
}

