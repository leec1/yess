#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "registers.h"
#include "executeStage.h"
#include "decodeStage.h"

static dregister D;

/* decodeStage
 *      Handles the main combinational logic of the decode stage.
 * Params:   uint *W_dstE - dstE from Writeback Register
 *           uint *W_valE - valE from Writeback Register
 *           uint *e_dstE -
 *           uint *e_valE -
 *           uint *M_dstM -
 *           uint *m_valM -
 *           uint *M_dstE -
 *           uint *M_valE -
 *           uint *W_dstM -
 *           uint *W_valM -
 * Returns:  void
 * Modifies: Execute Register
 */
void decodeStage(unsigned int *W_dstE, unsigned int *W_valE,
                 unsigned int *e_dstE, unsigned int *e_valE,
                 unsigned int *M_dstM, unsigned int *m_valM,
                 unsigned int *M_dstE, unsigned int *M_valE,
                 unsigned int *W_dstM, unsigned int *W_valM) {
    int dstE = getDstE();
    int dstM = getDstM();
    unsigned int srcA = getSrcA();
    unsigned int srcB = getSrcB();
    int valA = selectFwdA(srcA, *W_dstE, *W_valE, *e_dstE, *e_valE, *M_dstM,
                          *m_valM, *M_dstE, *M_valE, *W_dstM, *W_valM);
    int valB = forwardB(srcB, *W_dstE, *W_valE, *e_dstE, *e_valE, *M_dstM,
                        *m_valM, *M_dstE, *M_valE, *W_dstM, *W_valM);
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
 * Params:   uint d_srcA - intermediate srcA
 *           uint W_dstE - dstE from Writeback Register
 *           uint W_valE - valE from Writeback Register
 *           uint e_dstE -
 *           uint e_valE -
 *           uint M_dstM -
 *           uint m_valM -
 *           uint M_dstE -
 *           uint M_valE -
 *           uint W_dstM -
 *           uint W_valM -
 * Returns:  int - the value to forward
 * Modifies: none
 */
int selectFwdA(unsigned int d_srcA, unsigned int W_dstE, unsigned int W_valE,
               unsigned int e_dstE, unsigned int e_valE, unsigned int M_dstM,
               unsigned int m_valM, unsigned int M_dstE, unsigned int M_valE,
               unsigned int W_dstM, unsigned int W_valM) {
    //printf("\nSelecting FwdA: D.icode == %d\n", D.icode);
    if (D.icode == CALL || D.icode == JXX) {
        //printf("\nA: Using D_valP\n");
        return D.valP;
    }
    if (d_srcA == RNONE) return 0;
    if (d_srcA == e_dstE){
//        printf("\nA: Using e_valE\n");   
        return e_valE; 
    }
    if (d_srcA == M_dstM){
//        printf("\nA: Using m_valM\n");   
        return m_valM;
    }
    if (d_srcA == M_dstE){
//        printf("\nA: Using M_valE\n");   
        return M_valE;
    }
    if (d_srcA == W_dstM){
//        printf("\nA: Using W_valM\n");   
        return W_valM;
    }
    if (d_srcA == W_dstE){
//        printf("\nA: Using W_valE\n");   
        return W_valE;
    }
//    printf("\nA: No forwarding neccessary\n");
    return getRegister(d_srcA);
}

/* forwardB
 *      Helper to decide if we need to forward data through the pipeline.
 * Params:   uint d_srcA - intermediate srcA
 *           uint W_dstE - dstE from Writeback Register
 *           uint W_valE - valE from Writeback Register
 *           uint e_dstE -
 *           uint e_valE -
 *           uint M_dstM -
 *           uint m_valM -
 *           uint M_dstE -
 *           uint M_valE -
 *           uint W_dstM -
 *           uint W_valM -
 * Returns:  int - the value to forward
 * Modifies: none
 */
int forwardB(unsigned int d_srcB, unsigned int W_dstE, unsigned int W_valE,
             unsigned int e_dstE, unsigned int e_valE, unsigned int M_dstM,
             unsigned int m_valM, unsigned int M_dstE, unsigned int M_valE,
             unsigned int W_dstM, unsigned int W_valM) {
    if (d_srcB == RNONE) return 0;
    if (d_srcB == e_dstE){
//        printf("\nB: Using e_valE\n");   
        return e_valE;
    }
    if (d_srcB == M_dstM){
//        printf("\nB: Using m_valM\n");
        return m_valM;
    }
    if (d_srcB == M_dstE){
//        printf("\nB: Using M_valE\n");   
        return M_valE;
    }
    if (d_srcB == W_dstM){
//        printf("\nB: Using W_valM\n");   
        return W_valM;
    }
    if (d_srcB == W_dstE){
//        printf("\nB: Using W_valE\n");   
        return W_valE;
    }
//    printf("\nB: No forwarding neccessary\n");
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

/* clearDregister
 *      Clears the contents of the D register.
 * Params:   none 
 * Returns:  void
 * Modifies: D
 */
void clearDregister() {
    clearBuffer((char *) &D, sizeof(D));
    D.stat = SAOK;
    D.icode = NOP;
}

/* updateDregister
 *      Sets the values in the Decode PIPE register to the specified values.
 * Params:   int stat - Status of the Pipeline
 *           int icode - Current Instruction
 *           int ifun - Current Instruction (more information)
 *           int rA - Register A from current Instruction
 *           int rB - Register B from current Instruction
 *           int valC - Immediate Value of current Instruction
 *           int valP - Incremented PC for current Instruction
 * Returns:  void
 * Modifies: dregister D
 */
void updateDregister(int stat, int icode, int ifun, int rA, int rB, int valC,
                     int valP) {
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
