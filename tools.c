#include <stdio.h>
#include "types.h"
#include "tools.h"

/* clearBuffer
 *      Sets SIZE bytes at *REG in memory to 0.
 * Params:   char *reg - the byte address in memory
 *           uint size - the number of elements to zero out
 * Returns:  void
 * Modifies: Memory. Can I be more general than that?
 */
void clearBuffer(char *reg, unsigned int size) {
    unsigned int i;
    for (i=0; i<size; i++) {
        reg[i] = 0;
    }
}

/* getBits
 *      Returns only the specified portion of the int, denoted by a bit range.
 * Params:   int low - [0,high], the lower bound of the bit range
 *           int high - [low,31], the upper bound of the bit range
 *           u int source - the bit pattern to mask
 * Returns:  u int - source with the bit pattern requested in the less
 *           significant part of the int.
 * Modifies: none
 */ 
unsigned int getBits(int low, int high, unsigned int source) {
    if (low < 0 || high > 31 || low > high) return 0;
    source =  source << (31-high);
    source = source >> (31-(high-low));
    return source;
}

/* printBits
 *      Prints to stdout the specified unsigned int as a binary bit pattern
 *      formatted in groups of 8.
 * Params:   u int source - the bit pattern to print out
 * Returns:  void
 * Modifies: none
 */
void printBits(unsigned int source) {
    int i; 
    for (i=31; i>=0; i--) {
        printf("%d",getBits(i,i,source));
        if (i%8==0) printf(" ");
    }
}

/* setBits
 *      Set the specified bits (to 1) in the provided int
 * Params:   int low - [0,high], the lower bound of the bit range
 *           int high - [low, 31], the upper bound of the bit range
 *           u int source - the bit pattern to set bits of
 * Returns:  u int - source with the specified bits set (1)
 * Modifies: none
 */
unsigned int setBits(int low, int high, unsigned int source) {
    if (low < 0 || high > 31 || low > high) return 0;
    unsigned int ones = getBits(low, high, -1);
    ones = ones << low;
    return source | ones;
}

/* clearBits
 *      Clear the specified bits (set to 0) in the provided int.
 * Params:   int low - [0, high], the lower bound of the bit range
 *           int high - [low, 31], the upper bound of the bit range
 *           u int source - the bit pattern to clear bits of
 * Returns:  u int - source with the specified bits cleared (0)
 * Modifies: none
 */
unsigned int clearBits(int low, int high, unsigned int source) {
    if (low < 0 || high > 31 || low > high) return 0;
    unsigned int zeros = getBits(low, high, -1);
    zeros = ~(zeros << low);
    return source & zeros;
}

/* assignOneBit
 *      Set a single specific bit of an int to 1 or 0.
 * Params:   int bitNumber - [0,31], which bit to set/clear
 *           int bitValue - (0 || 1) what to set the bit to
 *           u int source - the bit pattern to set a bit in
 * Returns:  u int - source with the specified bit set or cleared
 * Modifies: none
 */
unsigned int assignOneBit(int bitNumber, int bitValue, unsigned int source) {
    if (bitValue == 1) return setBits(bitNumber, bitNumber, source);
    else if (bitValue == 0) return clearBits(bitNumber, bitNumber, source);
    else return 0;
}

/* getByteNumber
 *      Get a single specific byte from an int bit pattern.
 * Params:   int byteNumber - [0,3], which byte to retrieve
 *           u int source - the bit pattern to retrieve a byte from
 * Returns:  u char - the single byte value from source
 * Modifies: none
 */
unsigned char getByteNumber(int byteNumber, unsigned int source) {
    if (byteNumber < 0 || byteNumber > 3) return 0;
    return getBits(byteNumber*8, (byteNumber*8)+7, source);
}

/* putByteNumber
 *      Insert the specified byte into the provided bit pattern
 *      at the specified byte number location.
 * Params:   int byteNumber - [0,3], which byte to replace
 *           u char byte - the byte value to insert into source
 *           u int source - the bit pattern to insert the byte into
 * Returns:  u int - source, with the specified byte replaced
 * Modifies: none
 */
unsigned int putByteNumber(int byteNumber, unsigned char byte,
                            unsigned int source) {
    if (byteNumber < 0 || byteNumber > 3) return 0;
    source = clearBits(byteNumber*8, (byteNumber*8)+7, source);
    unsigned int bits = byte << byteNumber*8;
    return source | bits;
}

/* buildWord
 *      Creates a full 32bit int bit pattern from the 4 provided bytes.
 * Params:   u char byte0 - least significant byte
 *           u char byte1 - second-least significant byte
 *           u char byte2 - second-most significant byte
 *           u char byte3 - most significant byte
 * Returns:  u int - the constructed int bit pattern
 * Modifies: none
 */
unsigned int buildWord(unsigned char byte0, unsigned char byte1,
                       unsigned char byte2, unsigned char byte3) {
    unsigned int word = 0;
    word = putByteNumber(0,byte0,word);
    word = putByteNumber(1,byte1,word);
    word = putByteNumber(2,byte2,word); 
    word = putByteNumber(3,byte3,word);
    return word;
}

/* isNegative
 *      Tests if the provided bit pattern is negative when interpreted as a
 *      signed 32bit int.
 * Params:   u int source - the bit pattern to test
 * Returns:  bool (true || false), true if source is negative, false otherwise
 * Modifies: none
 */
bool isNeg(unsigned int source) {
    return getBits(31,31,source) == 1;
}
