#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "dump.h"
#include "writebackStage.h"

static wregister W;

bool writebackStage(unsigned int *W_dstE, unsigned int *W_valE){
    //int memError = FALSE;
    //if (W.stat != SAOK) return TRUE; // unless stat is A-OK, we should stop.
    if (W.icode == DUMP) {
        //printf("W.valE = %x\n", W.valE);
        //unsigned char flagByte = getByteNumber(0, W.valE);
        unsigned char flagByte = W.valE;
        //int flags = getBits(5, 8, flagByte);
        //printf("flagbyte = %x\n", flagByte);
        //printf("flags: %x\t( %x & 0x1 )\n", flags, flags);
        if ((flagByte & 0x1) == 0x1)
            dumpProgramRegisters();
        if ((flagByte & 0x2) == 0x2)
            dumpProcessorRegisters();
        if ((flagByte & 0x4) == 0x4)
            dumpMemory();
    }
    if (W.stat != SAOK && W.stat != 0) return TRUE; // unless stat is A-OK, we should stop.
    
    return FALSE;
}

/* getWregsiter
 *      Returns a copy of the W register.
 * Params:   none
 * Returns:  wregister
 * Modifies: none
 */
wregister getWregister() {
    return W;
}

/* clearWregister
 *      Clears the contents of the W register.
 * Params:   none
 * Returns:  void
 * Modifies: W
 */
void clearWregister() {
    clearBuffer((char *) &W, sizeof(W));
}


void updateWregister(int stat, int icode, int valE, int valM,
                     int dstE, int dstM){
    W.stat = stat;
    W.icode = icode;
    W.valE = valE;
    W.valM = valM;
    W.dstE = dstE;
    W.dstM = dstM;
}

void printWregister() {
    printf("\n=== Writeback Stage ===\n");
    printf("stat = %d\ticode = %x\n", W.stat, W.icode);
    printf("valE = %x\tvalM = %x\n", W.valE, W.valM);
    printf("dstE = %x\tdstM = %x\n", W.dstE, W.dstM);
}
