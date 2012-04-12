#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "memory.h"
#include "fetchStage.h"
#include "decodeStage.h"

#include "memoryStage.h"
#include "writebackStage.h"

int selectPC(int predPC, unsigned int *M_Cnd, unsigned int *M_icode,
             unsigned int *M_valA, unsigned int *W_icode);

static fregister F;

/* fetchStage
 *     Contains the main combinational logic of the fetch stage
 * Params:   none
 * Returns:  void
 * Modifies: Decode Register
 */
void fetchStage(unsigned int *M_Cnd, unsigned int *M_icode, unsigned int *M_valA, unsigned int *W_icode) {
    bool memError;
    unsigned int f_pc = selectPC(F.predPC, M_Cnd, M_icode, M_valA, W_icode);

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
        if (memError) stat = SADR;
    }

    if (icode == HALT) {
        F.predPC += 1;
        valP = F.predPC;
        stat = SHLT;
    } else if (icode == NOP || icode == RET){
        F.predPC += 1;
        valP = F.predPC;
    }
    else if (icode == CMOV || icode == OPL || icode == PUSHL || icode == POPL){
        F.predPC += 2;
        valP = F.predPC;
    }
    else if (icode == DUMP){
        F.predPC += 5;
        valP = F.predPC;
    }
    else if (icode == IRMOVL || icode == RMMOVL || icode == MRMOVL){
        F.predPC += 6;
        valP = F.predPC;
    }
    else if (icode == JXX || icode == CALL){
        valP = F.predPC + 5;
        F.predPC = valC;
    }
    else {
        F.predPC += 1;
        valP = F.predPC;
        stat = SINS;
    }

    //valP = F.predPC;
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
    clearBuffer((char *) &F, sizeof(F));
}

int selectPC(int predPC, unsigned int *M_Cnd, unsigned int *M_icode, unsigned int *M_valA, unsigned int *W_icode) {
    //wregister W = getWregister();
    //mregister M = getMregister();

    if (*M_icode == JXX && !(*M_Cnd))
        return *M_valA;
    if (*W_icode == RET)
        return *M_valA; //W.valM;
    return predPC;
}

void printFregister() {
    printf("\n===========================================\n");
    printf("\t\t%x\n", F.predPC);
    printf("===========================================\n");
}
