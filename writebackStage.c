#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "dump.h"
#include "registers.h"
#include "writebackStage.h"

static wregister W;

/* writebackStage
 *      Handles the main combinational logic fo the writeback stage
 * Params:   uint *W_dstE - dstE of the writeback register
 *           uint *W_valE - valE of the writeback register
 * Returns:  bool - stat == AOK
 * Modifies: *W_dstE, *W_valE
 */
bool writebackStage(unsigned int *W_dstE, unsigned int *W_valE){
    *W_dstE = W.dstE;
    *W_valE = W.valE;
    
    setRegister(W.dstE, W.valE);
    setRegister(W.dstM, W.valM);
    //if (W.dstE == EAX || W.dstM == EAX) 
        //printf("writing to EAX... W.valE= %x\tW.valM= %x\n", W.valE, W.valM);

    if (W.icode == DUMP) {
        unsigned char flagByte = W.valE;
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
 * Modifies: wregister W
 */
void clearWregister() {
    clearBuffer((char *) &W, sizeof(W));
}

/* updateWregister
 *      Updates the current values of the writeback register
 * Params:   int stat - Current status of the pipeline
 *           int icode - Instruction Type
 *           int valE - valE of current Instruction
 *           int valM - valM of current Instruction
 *           int dstE - dstE of current Instruction
 *           int dstM - dstM of current Instruction
 * Returns:  void
 * Modifies: Writeback Register
 */
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
