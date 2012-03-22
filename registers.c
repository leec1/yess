#include "types.h"
#include "tools.h"
#include "registers.h"

static unsigned int registers[REGSIZE];
static unsigned int CC;

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

void setCC(unsigned int bitNum, unsigned int bitValue){
    CC=assignOneBit(bitNum, bitValue, CC);
}

unsigned int getCC(unsigned int bitNum){
    return getBits(bitNum, bitNum, CC);
}

void initializeCC() {
    assignOneBit(OF, 0, CC);
    assignOneBit(ZF, 0, CC);
    assignOneBit(SF, 0, CC);
}
