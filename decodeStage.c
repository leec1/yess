#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "registers.h"
#include "executeStage.h"
#include "decodeStage.h"

static dregister D;

/* decodeStage
 *      Handles the main combinational logic of the decode stage.
 * Params:   uint W_dstE - 
 *           uint W_valE -
 * Returns:  none
 * Modifies: none?
 */
void decodeStage(unsigned int W_dstE, unsigned int W_valE){
    int dstE = getDstE();
    int dstM = getDstM();
    unsigned int srcA = getSrcA();
    unsigned int srcB = getSrcB();
    int valA = selectFwdA(srcA, W_dstE, W_valE);
    int valB = forwardB(srcB, W_dstE, W_valE);
    updateEregister(D.stat, D.icode, D.ifun, D.valC, valA, valB, dstE,
                    dstM, srcA, srcB);
}

/* getSrcA
 *      Helper for deciding which register to use as src A.
 * Params:   none
 * Returns:  uint - the register to use as src A
 * Modifies: none
 */
unsigned int getSrcA() {
    if (D.icode == CMOV || D.icode == RMMOVL ||
        D.icode == OPL || D.icode == PUSHL)
        return D.rA;
    else if (D.icode == POPL || D.icode == RET)
        return ESP;
    return RNONE;
}

/* getSrcB
 *      Helper for deciding which register to use as src B.
 * Params:   none
 * Returns:  uint - the register to use as src B
 * Modifies: none
 */
unsigned int getSrcB() {
    if (D.icode == OPL || D.icode == RMMOVL || D.icode == MRMOVL)
        return D.rB;
    else if (D.icode == PUSHL || D.icode == POPL ||
             D.icode == CALL  || D.icode == RET)
        return ESP;
    return RNONE;
}

/* getDstE
 *      Helper for deciding which register to use as dst E.
 * Params:   none
 * Returns:  int - the register to use as dst E
 * Modifies: none
 */
int getDstE() {
    if (D.icode == CMOV || D.icode == IRMOVL || D.icode == OPL)
        return D.rB;
    else if (D.icode == PUSHL || D.icode == POPL ||
             D.icode == CALL  || D.icode == RET)
        return ESP;
    return RNONE;
}

/* getDstM
 *      Helper for deciding which register to use as dst M
 * Params:   none
 * Returns:  int - the register to use as dst M
 * Modifies: none
 */
int getDstM() {
    if (D.icode == MRMOVL || D.icode == POPL)
        return D.rA;
    return RNONE;
}

/* selectFwdA
 *      Helper to decide if we need to forward data through the pipeline.
 * Params:   uint d_srcA -
 *           uint W_dstE -
 *           uint W_valE -
 * Returns:  int - the value to forward
 * Modifies: none
 */
int selectFwdA(unsigned int d_srcA, unsigned int W_dstE, unsigned int W_valE) {
    if(d_srcA == RNONE) return 0;
    //if(D.icode == CALL || D.icode == JXX) return D.valP;
    if(d_srcA == W_dstE) return W_valE; 
    return getRegister(d_srcA);
}

/* selectFwdA
 *      Helper to decide if we need to forward data through the pipeline.
 * Params:   uint d_srcA -
 *           uint W_dstE -
 *           uint W_valE -
 * Returns:  int - the value to forward
 * Modifies: none
 */
int forwardB(unsigned int d_srcB, unsigned int W_dstE, unsigned int W_valE) {
    if(d_srcB == RNONE) return 0;
    if(d_srcB == W_dstE) return W_valE;
    return getRegister(d_srcB);
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
