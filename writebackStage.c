#include "writebackStage.h"

// W register holds the input for the writeback stage. 
// It is only accessible from this file. (static)
static wregister W;

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
