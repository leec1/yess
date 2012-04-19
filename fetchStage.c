#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "memory.h"
#include "fetchStage.h"
#include "decodeStage.h"
#include "executeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"

/*int selectPC(int predPC, unsigned int *M_Cnd, unsigned int *M_icode,
             unsigned int *M_valA, unsigned int *W_icode, unsigned int *W_valM);*/
int selectPC(int predPC, fwdStruct *fwd);

bool F_bubble();

/*bool F_stall(unsigned int E_icode, unsigned int E_dstM, unsigned int d_srcA,
             unsigned int d_srcB, unsigned int D_icode, unsigned int E_icode,
             unsigned int M_icode);

bool D_stall(unsigned int E_icode, unsigned int E_dstM, unsigned int d_srcA, unsigned int d_srcB);

bool D_bubble(unsigned int E_icode, unsigned int e_Cnd, unsigned int E_dstM, unsigned int d_srcA,
              unsigned int d_srcB, unsigned int D_icode, unsigned int E_icode, unsigned int M_icode);*/

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
/*void fetchStage(unsigned int *M_Cnd, unsigned int *M_icode,
                unsigned int *M_valA, unsigned int *W_icode,
                unsigned int *W_valM) {*/
void fetchStage(fwdStruct *fwd) {
    bool memError;
    unsigned int f_pc = selectPC(F.predPC, fwd);
    //printf("PC = %x\n", f_pc);
    //F.predPC = f_pc;

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
        //F.predPC += 1;
        f_pc += 1;
        valP = f_pc;//F.predPC;
        stat = SHLT;
    } else if (icode == NOP || icode == RET){
        //F.predPC += 1;
        f_pc += 1;
        valP = f_pc;//F.predPC;
    }
    else if (icode == CMOV || icode == OPL || icode == PUSHL || icode == POPL){
        //F.predPC += 2;
        f_pc += 2;
        valP = f_pc;//F.predPC;
    }
    else if (icode == DUMP){
        //F.predPC += 5;
        f_pc += 5;
        valP = f_pc;//F.predPC;
    }
    else if (icode == IRMOVL || icode == RMMOVL || icode == MRMOVL){
        //F.predPC += 6;
        f_pc += 6;
        valP = f_pc; //F.predPC;
    }
    else if (icode == JXX || icode == CALL){
        valP = f_pc + 5; //F.predPC + 5;
        //printf("\nvalP = %d\n", valP);
        //F.predPC = valC;
        f_pc = valC;
    }
    else {
        //F.predPC += 1;
        f_pc += 1;
        valP = f_pc; //F.predPC;
        stat = SINS;
    }
   
    if(F_stall(fwd)){
       // clearDregister();   
    }else{
        F.predPC = f_pc;
    }
    if(F_bubble()){
        clearFregister();
    }
    
    if(D_bubble(fwd))
        clearDregister();
    if(D_stall(fwd)){
        //clearEregister();
    }else{
        updateDregister(stat, icode, ifun, rA, rB, valC, valP); 
    }
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

bool F_bubble() {
    return FALSE;
}

/*bool F_stall(unsigned int E_icode, unsigned int E_dstM, unsigned int d_srcA,
             unsigned int d_srcB, unsigned int D_icode, M_icode) {*/
bool F_stall(fwdStruct *fwd) {
    return  ((fwd->E_icode == MRMOVL || fwd->E_icode == POPL) &&
                (fwd->E_dstM == fwd->d_srcA || fwd->E_dstM == fwd->d_srcB)) ||
            (fwd->D_icode == RET || fwd->E_icode == RET || fwd->M_icode == RET);
}

/*bool D_bubble(unsigned int E_icode, unsigned int e_Cnd, unsigned int E_dstM, unsigned int d_srcA,
              unsigned int d_srcB, unsigned int D_icode, unsigned int E_icode, unsigned int M_icode){*/
bool D_bubble(fwdStruct *fwd) { 
    /*
     * # Mispredicted branch
     * (E_icode == IJXX && !e_Cnd) ||
     * # Stalling at fetch while ret passes through pipeline
     * # but not condition for a load/use hazar
     * !(E_icode in { IMRMOVL, IPOPL } && E_dstM in { d_srcA, d_srcB }) &&
     *   IRET in { D_icode, E_icode, M_icode };
     */
    return (fwd->E_icode == JXX && !(fwd->e_Cnd)) ||
           !((fwd->E_icode == MRMOVL || fwd->E_icode == POPL) &&
               ((fwd->E_dstM == fwd->d_srcA || fwd->E_dstM == fwd->d_srcB)) &&
               (fwd->D_icode == RET || fwd->E_icode == RET || fwd->M_icode == RET));
}

//bool D_stall(unsigned int E_icode, unsigned int E_dstM, unsigned int d_srcA, unsigned int d_srcB){
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
    clearBuffer((char *) &F, sizeof(F));
}

//int selectPC(int predPC, unsigned int *M_Cnd, unsigned int *M_icode, unsigned int *M_valA, unsigned int *W_icode, unsigned int *W_valM) {
int selectPC(int predPC, fwdStruct *fwd) {
    //wregister W = getWregister();
    //mregister M = getMregister();

    if (fwd->M_icode == JXX && !fwd->M_Cnd) {
        //printf("Mispredicted Branch, returning %d\n", *M_valA);
        return fwd->M_valA;
    }
    if (fwd->W_icode == RET){
        //printf("\nRET, returning %d\n", *W_valM);
        return fwd->W_valM;
    }
    return predPC;
}

void printFregister() {
    printf("\n===========================================\n");
    printf("\t\t%x\n", F.predPC);
    printf("===========================================\n");
}
