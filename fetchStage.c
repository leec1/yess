#include "types.h"
#include "fetchStage.h"

// F register holds the input for the fetch stage. 
// It is only accessible from this file. (static)
static fregister F;

int selectPC(int a);

void fetchStage() {
    bool memError;
    unsigned int f_pc = selectPC(F.predPC);

    unsigned char inst = getByte(f_pc, &memError);

    int stat = SAOK;
    int ifun = getBits(0, 3, inst);
    int icode = getBits(4, 7, inst);
    int rA = RNONE;
    int rB = RNONE;
    int valC = 0;
    int valP = 0;
    
    switch (icode) {
        case HALT:
            stat = SHLT;
            break;
        case NOP:
            break;
        case CMOV:
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
            break;
        case DUMP:
            break;
        default:
            stat = SINS;
            break;
    }
    //fetch instruction from memory by call getByte
    //use instruction and f_pc to figure out values
    //for stat, icode, ifun, rA, rB, valC, and vaP

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
