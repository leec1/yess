#include "fetchStage.h"

// F register holds the input for the fetch stage. 
// It is only accessible from this file. (static)
static fregister F;

/* getFregister
 *      Returns a copy of the F register
 * Params:   none
 * Returns:  fregister
 * Modifies: none
 */
fregister getFregister() {
    return F;
}

/* clearFregister
 *      Clears the contents of the F register.
 * Params:   none
 * Returns:  none
 * Modifies: F 
 */
void clearFregister() {
    clearBuffer((char *) &F, sizeof(F));
}
