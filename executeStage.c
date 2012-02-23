#include "executeStage.h"

// E register holds the input for the execute stage.
// It is only accessible from this file. (static)
static eregister E;

void executeStage(){

}

/* getEregister
 *      Returns a copy of the eregister.
 * Params:   none
 * Returns:  eregister
 * Modifies: none
 */
eregister getEregister() {
    return E;
}

/* clearEregister
 *      Clears the contents of the E register.
 * Params:   none
 * Returns:  void
 * Modifies: E
 */
void clearEregister() {
    clearBuffer((char *) &E, sizeof(E));
}


void updateEregister(int stat, int icode, int ifun, int rA, int rB,
        int valC, int valP){

}
