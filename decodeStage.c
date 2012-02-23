#include "decodeStage.h"

// D register holds the input for the decode stage.
// It is only accessible from this file. (static)
static dregister D;

/* getDregister
 *     Returns a copy of the D regsiter.
 * Params:   none
 * Returns:  dregsiter
 * Modifies: none
 */
dregister getDregister() {
    return D;
}

/* chearDregister
 *      Clears the contents of the D register.
 * Params:   none 
 * Returns:  void
 * Modifies: D
 */
void clearDregister() {
    clearBuffer((char *) &D, sizeof(D));
}
