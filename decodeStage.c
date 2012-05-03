#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "registers.h"
#include "memoryStage.h"
#include "executeStage.h"
#include "decodeStage.h"

static dregister D;

bool E_bubble(unsigned int d_srcA, unsigned int d_srcB, fwdStruct *fwd);
bool E_stall(); 

/* decodeStage
 *      Handles the main combinational logic of the decode stage.
 * Params:   fwdStruct *fwd - contains a bunch of forwarding values
 * Returns:  void
 * Modifies: Execute Register
 */
void decodeStage(fwdStruct *fwd) {
    int dstE = getDstE();
    int dstM = getDstM();
    
    unsigned int srcA = getSrcA();
    unsigned int srcB = getSrcB();
    fwd->d_srcA = srcA;
    fwd->d_srcB = srcB;
    fwd->D_icode = D.icode;
    int valA = selectFwdA(srcA, fwd);
    int valB = forwardB(srcB, fwd);
   
    if (E_bubble(srcA, srcB, fwd))
        clearEregister();
    else if (!E_stall())
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
 *           fwdStruct *fwd - contains a bunch of forwarding stuff
 * Returns:  int - the value to forward
 * Modifies: none
 */
int selectFwdA(unsigned int d_srcA, fwdStruct *fwd) {
    if (D.icode == CALL || D.icode == JXX)
        return D.valP;
    if (d_srcA == RNONE)
        return 0;
    if (d_srcA == fwd->e_dstE)
        return fwd->e_valE; 
    if (d_srcA == fwd->M_dstM)
        return fwd->m_valM;
    if (d_srcA == fwd->M_dstE)
        return fwd->M_valE;
    if (d_srcA == fwd->W_dstM)
        return fwd->W_valM;
    if (d_srcA == fwd->W_dstE)
        return fwd->W_valE;
    return getRegister(d_srcA);
}

/* forwardB
 *      Helper to decide if we need to forward data through the pipeline.
 * Params:   uint d_srcB - intermediate srcB
 *           fwdStruct *fwd - contains a bunch of forwarding stuff
 * Returns:  int - the value to forward
 * Modifies: none
 */
int forwardB(unsigned int d_srcB, fwdStruct *fwd) {
    if (d_srcB == RNONE)
        return 0;
    if (d_srcB == fwd->e_dstE)
        return fwd->e_valE;
    if (d_srcB == fwd->M_dstM)
        return fwd->m_valM;
    if (d_srcB == fwd->M_dstE)
        return fwd->M_valE;
    if (d_srcB == fwd->W_dstM)
        return fwd->W_valM;
    if (d_srcB == fwd->W_dstE)
        return fwd->W_valE;
    return getRegister(d_srcB);
}

/* E_bubble
 *      Determines if the E Register needs to be bubbled
 * Params:   u int d_srcA   - srcA from the current stage
 *           u int d_srcB   - srcB from the current stage
 *           fwdStruct *fwd - Forwarding Struct
 * Returns:  bool - TRUE if needs to be bubbled
 * Modifies: none
 */
bool E_bubble(unsigned int d_srcA, unsigned int d_srcB, fwdStruct *fwd) {
    return (fwd->E_icode == JXX && !(fwd->e_Cnd)) ||
             ((fwd->E_icode == MRMOVL || fwd->E_icode == POPL) &&
             (fwd->E_dstM == d_srcA || fwd->E_dstM == d_srcB));
}

/* E_stall
 *      Determines if the E Register needs to be stalled
 * Params:   none
 * Returns:  bool - TRUE if needs to be bubbled
 * Modifies: none
 */
bool E_stall() {
    return FALSE;
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
    D.stat = SAOK;
    D.icode = NOP;
    D.ifun = 0;
    D.rA = RNONE;
    D.rB = RNONE;
    D.valC = 0;
    D.valP = 0;
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
