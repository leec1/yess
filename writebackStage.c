#include "types.h"
#include "writebackStage.h"

// W register holds the input for the writeback stage. 
// It is only accessible from this file. (static)
static wregister W;

bool writebackStage(){
    if (W.stat != SAOK) return TRUE; // unless stat is A-OK, we should stop.

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
