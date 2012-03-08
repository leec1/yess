#include "types.h"
#include "tools.h"
#include "memory.h"

// Array memory holds the contents of the phsyical memory.
// It is only accessible from this file. (static)
static unsigned int memory[MEMSIZE];

unsigned int fetch(int address, bool *memError);
void store(int address, unsigned int value, bool *memError);

/* getByte
 *      Reads a single byte from memory.
 * Params:   int address - the location in memory to look in
 *           bool *memError - a pointer to the memory error boolean
 * Returns:  u char - the value in memory at the specified (valid) byte address
 * Modifies: memError - sets to TRUE if an error, FALSE otherwise
 */
unsigned char getByte(int address, bool *memError) {
    if  (address < 0 || address >= 4*MEMSIZE) {
        *memError = TRUE;
        return 0;
    }
    return getByteNumber(address&3, fetch(address/4, memError));
}

/* putByte
 *      Writes a single byte to memory.
 * Params:   int address - the location in memory to write to
 *           u char value - the byte to write to memory
 *           bool *memError - a pointer to the memory error boolean
 * Returns:  void
 * Modifies: memError - sets to TRUE if an error, FALSE otherwise
 */
void putByte(int address, unsigned char value, bool *memError) {
    if  (address < 0 || address >= 4*MEMSIZE) *memError = TRUE;
    else {
        unsigned int o = fetch(address/4, memError);
        unsigned int n = putByteNumber(address&3, value, o);
        store(address/4, n, memError);
    }
}

/* getWord
 *      A wrapper method for fetch that checks to make sure the address
 *      is divisible by 4 ( X & 3 == X % 4 )
 * Params:   int address - the location in memory to look in
 *           bool *memError - a pointer to the memory error boolean
 * Returns:  u int - the value in memory at the specified (valid) address
 * Modifies: memError - sets to TRUE if an error, FALSE otherwise
 */
unsigned int getWord(int address, bool *memError) {
    if ((address & 3) != 0) {
        *memError = TRUE;
        return 0;
    }
    return fetch(address/4, memError);
}

/* putWord
 *      A wrapper method for store that checks to make sure the address
 *      is divisible by 4 ( X & 3 == X % 4 )
 * Params:   int address - the location in memory to write to
 *           u int value - the word to write to memory
 *           bool *memError - a pointer to the memory error boolean
 * Returns:  void
 * Modifies: memError - sets to TRUE if an error, FALSE otherwise
 */
void putWord(int address, unsigned int value, bool *memError) {
    if ((address & 3) != 0) *memError = TRUE;
    else store(address/4, value, memError);
}

/* clearMemory
 *      Clears the entire contents of the memory array.
 * Params:   none
 * Returns:  void
 * Modifies: memory
 */
void clearMemory() {
    int i;
    for (i=0; i<MEMSIZE; i++) memory[i] = 0;
}

/* fetch
 * Params:   int address - the location in memory to look in
 *           bool *memError - a pointer to the memory error boolean
 * Returns:  u int - the value in memory at the specified (valid) address
 * Modifies: memError - sets to TRUE if an error, FALSE otherwise
 */
unsigned int fetch(int address, bool *memError) {
    if (address < 0 || address >= MEMSIZE) {
        *memError = TRUE;
        return 0;
    }
    *memError = FALSE;
    return memory[address];
}

/* store
 *      Writes a word to the memory array.
 * Params:   int address - the location in memory to write to
 *           u int value - the word to write to memory
 *           bool *memError - a pointer to the memory error boolean
 * Returns:  void
 * Modifies: memError - sets to TRUE if an error, FALSE otherwise
 */
void store(int address, unsigned int value, bool *memError) {
    if (address < 0 || address >= MEMSIZE) *memError = TRUE;
    else {
        *memError = FALSE;
        memory[address] = value;
    }
}
