#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "memory.h"
#include "fetchStage.h"
#include "decodeStage.h"
#include "executeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"

int selectPC(int predPC, fwdStruct *fwd);

bool F_bubble();
bool F_stall(fwdStruct *fwd);

bool D_bubble(fwdStruct *fwd);
bool D_stall(fwdStruct *fwd);

static fregister F;

/* fetchStage
 *     Contains the main combinational logic of the fetch stage
 * Params:   none
 * Returns:  void
 * Modifies: Decode Register
 */
void fetchStage(fwdStruct *fwd) {
    bool memError;
    unsigned int f_pc = selectPC(F.predPC, fwd);

    unsigned char inst = getByte(f_pc, &memError);
    
    int stat = memError ? SADR : SAOK;
    int ifun = getBits(0, 3, inst);
    int icode = getBits(4, 7, inst);
    int rA = RNONE;
    int rB = RNONE;
    int valC = 0;
    int valP = 0;

    if (instructionNeedsRegByte(icode)) {
        int regs = getByte(f_pc+1, &memError);
        if (memError) stat = SINS;
        rB = getBits(0, 3, regs);
        rA = getBits(4, 7, regs);
    }
    int off;
    if ((off = need_valC(icode))) {
        unsigned char byte0 = getByte(f_pc+off, &memError);
        unsigned char byte1 = getByte(f_pc+off+1, &memError);
        unsigned char byte2 = getByte(f_pc+off+2, &memError);
        unsigned char byte3 = getByte(f_pc+off+3, &memError);
        valC = buildWord(byte0, byte1, byte2, byte3);
        if (memError)
            stat = SADR;
    }

    if (icode == HALT) {
        f_pc += 1;
        valP = f_pc;
        stat = SHLT;
    }
    else if (icode == NOP || icode == RET){
        f_pc += 1;
        valP = f_pc;
    }
    else if (icode == CMOV || icode == OPL || icode == PUSHL || icode == POPL){
        f_pc += 2;
        valP = f_pc;
    }
    else if (icode == DUMP){
        f_pc += 5;
        valP = f_pc;
    }
    else if (icode == IRMOVL || icode == RMMOVL || icode == MRMOVL){
        f_pc += 6;
        valP = f_pc;
    }
    else if (icode == JXX || icode == CALL){
        valP = f_pc + 5;
        f_pc = valC;
    }
    else {
        f_pc += 1;
        valP = f_pc; 
        stat = SINS;
    }
   
    if(F_bubble())
        clearFregister();
    else if(!F_stall(fwd))
        F.predPC = f_pc;
    
    if(D_bubble(fwd))
        clearDregister();
    else if(!D_stall(fwd))
        updateDregister(stat, icode, ifun, rA, rB, valC, valP);
        
}

/* instructionNeedsRegByte
 *      Returns whether the instruction needs a Register
 * Params:   icode - Current Instruction Type
 * Returns:  bool
 * Modifies: none
 */
bool instructionNeedsRegByte(int icode) {
    return (icode == CMOV) || (icode == OPL)    || (icode == PUSHL)  ||
           (icode == POPL) || (icode == IRMOVL) || (icode == RMMOVL) ||
           (icode == MRMOVL);
}

/* need_valC
 *      Returns whether the instruction requires fetching valC
 * Params:   icode - Current Instruction Type
 * Returns:  int
 * Modifies: none
 */
int need_valC(int icode) {
    if ((icode == JXX) || (icode == CALL) || (icode == DUMP)) return 1;
    if ((icode == IRMOVL) || (icode == RMMOVL) || (icode == MRMOVL)) return 2;
    return 0;
}

/* F_bubble
 *      Determines if the F Register needs to be bubbled
 * Params:   none
 * Returns:  bool - TRUE if needs to be bubbled
 * Modifies: none
 */
bool F_bubble() {
    return FALSE;
}

/* F_stall
 *      Determines if the F Register needs to be stalled
 * Params:   fwdStruct *fwd  - Fowarding Struct
 * Returns:  bool            - TRUE if needs to be stalled
 * Modifies: none
 */
bool F_stall(fwdStruct *fwd) {
    return  ((fwd->E_icode == MRMOVL || fwd->E_icode == POPL) &&
                (fwd->E_dstM == fwd->d_srcA || fwd->E_dstM == fwd->d_srcB)) ||
            (fwd->D_icode == RET || fwd->E_icode == RET || fwd->M_icode == RET);
}

/* D_bubble
 *      Determines if the D Register needs to be bubbled
 * Params:   fwdStruct *fwd  - Forwarding Struct
 * Returns:  bool            - TRUE if needs to be bubbled
 * Modifies: none
 */
bool D_bubble(fwdStruct *fwd) { 
    return (fwd->E_icode == JXX && !(fwd->e_Cnd)) ||
           (!((fwd->E_icode == MRMOVL || fwd->E_icode == POPL) &&
               (fwd->E_dstM == fwd->d_srcA || fwd->E_dstM == fwd->d_srcB)) &&
               (fwd->D_icode == RET || fwd->E_icode == RET || fwd->M_icode == RET));
}

/* D_stall
 *      Determines if the D Register needs to be stalled
 * Params:   fwdStruct *fwd  - Forwarding Struct
 * Returns:  bool            - TRUE if needs to be stalled
 * Modifies: none
 */
bool D_stall(fwdStruct *fwd) {
    return (fwd->E_icode == MRMOVL || fwd->E_icode == POPL) &&
           (fwd->E_dstM == fwd->d_srcA || fwd->E_dstM == fwd->d_srcB);
}

/* getFregister
 *      Returns a copy of the F register
 * Params:   none
 * Returns:  fregister
 * Modifies: none
 */
fregister getFregister() {
    return F;
}

/* clearFregister
 *      Clears the contents of the F register.
 * Params:   none
 * Returns:  void
 * Modifies: fregister F
 */
void clearFregister() {
    F.predPC = 0;
}

/* selectPC
 *      Determines the PC, based on forwarded values
 * Params:   predPC - Incremented PC
 *           fwd    - Fowarding Struct
 * Returns:  int    - Selected PC value
 * Modifies: none
 */
int selectPC(int predPC, fwdStruct *fwd) {
    if (fwd->M_icode == JXX && !fwd->M_Cnd)
        return fwd->M_valA;
    
    if (fwd->W_icode == RET)
        return fwd->W_valM;
   
    return predPC;
}

void printFregister() {
    printf("\n===========================================\n");
    printf("\t\t%x\n", F.predPC);
    printf("===========================================\n");
}
