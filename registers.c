#include "registers.h"

static unsigned int registers[REGSIZE];

/* getRegister
 *      Returns the value of the specified register.
 * Params:   int regNum - [0,7], which register to return
 * Returns:  u int - the value of the specified register
 * Modifies: none
 */
unsigned int getRegister(int regNum) {
    if (regNum >= 0 && regNum < REGSIZE) {
        return registers[regNum];
    }
    return 0;
}

/* setRegister
 *      Writes the specified value to the specified register.
 * Params:   int regNum - [0,7], which register to write to
 *           u int regValue - the value to write
 * Returns:  void
 * Modifies: registers
 */
void setRegister(int regNum, unsigned int regValue) {
    if (regNum >= 0 && regNum < REGSIZE) {
        registers[regNum] = regValue;
    }
}

/* clearRegisters
 *      Clears the current value of all registers.
 * Params:   none
 * Returns:  void
 * Modifies: regsiters
 */
void clearRegisters() {
    int i;
    for (i=0; i<REGSIZE; i++) {
        registers[i] = 0;
    }
}
