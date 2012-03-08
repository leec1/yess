#include "types.h"
#include "fetchStage.h"

#include "memoryStage.h"
#include "writebackStage.h"

// F register holds the input for the fetch stage. 
// It is only accessible from this file. (static)
static fregister F;

int selectPC(int predPC);

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
    
    switch (icode) {
        case HALT:
            stat = SHLT;
            F.predPC = F.predPC + 1;
            break;
        case NOP:
            F.predPC = F.predPC + 1;
            break;
        /*case CMOV:
            break;
        case IRMOVL:
            break;
        case RMMOVL:
            break;
        case MRMOVL:
            break;
        case OPL:
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
            break;*/
        case DUMP:
            F.predPC = F.predPC + 1;
            unsigned char byte0 = getByte(f_pc+1, &memError);
            unsigned char byte1 = getByte(f_pc+2, &memError);
            unsigned char byte2 = getByte(f_pc+3, &memError);
            unsigned char byte3 = getByte(f_pc+4, &memError);
            valC=buildWord(byte3, byte2, byte1, byte0);
            break;
        default:
            F.predPC = F.predPC + 1; 
            stat = SINS;
            break;
    }

    updateDregister(stat, icode, ifun, rA, rB, valC, valP); 
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


