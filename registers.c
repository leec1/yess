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
    if (regNum >= 0 && regNum < REGSIZE)
        return registers[regNum];
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
    if (regNum >= 0 && regNum < REGSIZE)
        registers[regNum] = regValue;
}

/* clearRegisters
 *      Clears the current value of all registers.
 * Params:   none
 * Returns:  void
 * Modifies: regsiters
 */
void clearRegisters() {
    int i;
    for (i=0; i<REGSIZE; i++)
        registers[i] = 0;
}

/* setCC
 *      Sets the Condition Code giving to a particular value
 * Params:   u int bitNum - [0,2], which bit in the CC
 *           u int bitValue - [0,1], value to be inserted
 * Returns:  void
 * Modifies: CC
 */
void setCC(unsigned int bitNum, unsigned int bitValue){
    CC = assignOneBit(bitNum, bitValue, CC);
}

/* getCC
 *      Returns the specified Condition Code
 * Params:   u int bitNum - [0,2], the specific CC
 * Returns:  u int - the value of the specified CC
 * Modifies: none
 */
unsigned int getCC(unsigned int bitNum){
    return getBits(bitNum, bitNum, CC);
}

/* initializeCC
 *      Initializes the Condition Codes
 * Params:   none
 * Returns:  void
 * Modifies: Condition Codes
 */
void initializeCC() {
    assignOneBit(OF, 0, CC);
    assignOneBit(ZF, 0, CC);
    assignOneBit(SF, 0, CC);
}
