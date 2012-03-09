#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "memory.h"
#include "decodeStage.h"
#include "fetchStage.h"

int selectPC(int predPC);

static fregister F;

void fetchStage() {
    bool memError;
    unsigned int f_pc = selectPC(F.predPC);

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
        rB = getBits(0, 3, regs);
        rA = getBits(4, 7, regs);
    }
    int off;
    if (off = need_valC(icode)) {
        unsigned char byte0 = getByte(f_pc+off, &memError);
        unsigned char byte1 = getByte(f_pc+off+1, &memError);
        unsigned char byte2 = getByte(f_pc+off+2, &memError);
        unsigned char byte3 = getByte(f_pc+off+3, &memError);
        valC = buildWord(byte0, byte1, byte2, byte3);
    }
    
    switch (icode) {
        case HALT:
            stat = SHLT;
            F.predPC = F.predPC + 1;
            break;
        case NOP:
            F.predPC = F.predPC + 1;
            break;
        case CMOV:
            break;
        case IRMOVL:
            F.predPC = F.predPC + 6;
            break;
        case RMMOVL:
            break;
        case MRMOVL:
            break;
        case OPL:
            F.predPC = F.predPC + 2;
            break;
        case JXX:
            break;
        case CALL:
            break;
        case RET:
            break;
        case PUSHL:
            break;
        case POPL:
            break;
        case DUMP:
            F.predPC = F.predPC + 5;
            break;
        default:
            F.predPC = F.predPC + 1; 
            stat = SINS;
            break;
    }

    valP = F.predPC;

    updateDregister(stat, icode, ifun, rA, rB, valC, valP); 
}

bool instructionNeedsRegByte(int icode) {
    return (icode == CMOV) || (icode == OPL)    || (icode == PUSHL)  ||
           (icode == POPL) || (icode == IRMOVL) || (icode == RMMOVL) ||
           (icode == MRMOVL);
}

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
 * Returns:  none
 * Modifies: F 
 */
void clearFregister() {
    clearBuffer((char *) &F, sizeof(F));
}

int selectPC(int predPC) {
    //wregister W = getWregister();
    //mregister M = getMregister();

    //if (M.icode == JXX && !M.Cnd)
    //    return M.valA;
    //if (W.icode == RET)
    //    return W.valM;
    return predPC;
}

void printFregister() {
    printf("\n===========================================\n");
    printf("\t\t%x\n", F.predPC);
    printf("===========================================\n");
}
