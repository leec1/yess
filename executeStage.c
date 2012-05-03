#include <stdio.h>
#include "types.h"
#include "tools.h"
#include "registers.h"
#include "memoryStage.h"
#include "executeStage.h"
#include "writebackStage.h"
static eregister E;
int (*funcArr[16])();
bool Cnd;
static bool canUpdateCC;

bool M_stall();
bool M_bubble(fwdStruct *fwd);

void setCnd(unsigned int fCode);

/* executeStage
 *      Handles the main combination logic of the execute stage.
 * Params:   fwdStruct *fwd - contains a bunch of forwarding stuff
 * Returns:  void
 * Modifies: Cnd
 */
void executeStage(fwdStruct *fwd) {
    Cnd = FALSE;
    int valE = 0;
    int dstE = E.dstE;

    if (E.stat == SINS || E.stat == SADR || E.stat == SHLT)
        canUpdateCC = FALSE;

    if (E.stat != SINS)
        valE = (*funcArr[E.icode])();
    
    if (E.icode == CMOV && !Cnd)
        dstE = RNONE;

    fwd->e_dstE = dstE;
    fwd->e_valE = valE;
    fwd->E_dstM = E.dstM;
    fwd->e_Cnd = Cnd;
    fwd->E_icode = E.icode;
        
    if(M_bubble(fwd))
        clearMregister();
    else if(!M_stall())
        updateMregister(E.stat, E.icode, Cnd, valE, E.valA, dstE, E.dstM);
}

/* doNothing
 *      Function that does nothing in the FuncPtr array
 * Params:   none
 * Returns:  int  - 0
 * Modifies: none
 */
int doNothing() {
    return 0;
}

/* performRrmovl
 *      Simulates the RRMOVL/CMOVXX instruction. Checks E.ifun and performs the
 *      correct type of move based on the icode given. Updates Cnd to reflect
 *      whether or not a conditional instruction should actually occur.
 * Params:   none
 * Returns:  int - E.valA
 * Modifies: Cnd
 */
int performRrmovl() {
    setCnd(E.ifun);
    return E.valA;
}

/* performIrmovl
 *      Simulates the IRMOVL instruction. Well, kinda. Doesn't really do much.
 * Params:   none
 * Returns:  int - E.valC
 * Modifies: none
 */
int performIrmovl() {
    return E.valC;
}

/* performRmmovl
 *      Simulates the RMMOVL instruction. Again, just adding two numbers here.
 * Params:   none
 * Returns:  int - E.valC + E.valB
 * Modifies: none
 */
int performRmmovl() {
    return E.valC + E.valB;
}

/* performMrmovl
 *      Simulates the MRMOVL instruction. I dunno who I'm kidding anymore.
 * Params:   none
 * Returns:  int - E.valC + E.valB
 * Modifies: none
 */
int performMrmovl() {
    return E.valC + E.valB;
}

/* performOpl
 *      Simulates the OPL instruction. Checks E.ifun and performs the correct
 *      operation based on the icode given. Also updates condition codes. If
 *      an invald function code is provided, sets E.stat to SINS.
 * Params:   none
 * Returns:  int - the result of the desired operation
 * Modifies: E.stat
 */
int performOpl() {
    int val = 0;
    switch (E.ifun) {
        case ADD:
            val = E.valB + E.valA;
            if (canUpdateCC &&
                ((isNeg(E.valB) && isNeg(E.valA) && !isNeg(val))  ||
                (!isNeg(E.valB) && !isNeg(E.valA) && isNeg(val))))
                setCC(OF, 1);
            else if(canUpdateCC)
                setCC(OF, 0);
            break;
        case SUB:
            val = E.valB - E.valA;
            if (canUpdateCC &&
                ((isNeg(E.valB) && !isNeg(E.valA) && !isNeg(val)) ||
                (!isNeg(E.valB) && isNeg(E.valA) && isNeg(val))))
                setCC(OF, 1);
            else if(canUpdateCC)
                setCC(OF, 0);
            break;
        case AND:
            if(canUpdateCC)
                setCC(OF, 0);
            val = E.valB & E.valA;
            break;
        case XOR:
            if(canUpdateCC)
                setCC(OF, 0);
            val =  E.valB ^ E.valA;
            break;
        default:
            E.stat = SINS;
            break;
    }
    updateCC(val);
    return val;
}

/* performJXX
 *      Simulates the JXX instruction.
 * Params:   none
 * Returns:  int - E.valA
 * Modifies: Cnd
 */
int performJXX() {
    setCnd(E.ifun);
    return E.valA;
}

/* performCall
 *      Simulates the CALL instruction.
 * Params:   none
 * Returns:  int - E.valB - 4
 * Modifies: none
 */
int performCall() {
    return E.valB - 4;
}

/* performRet
 *      Simulates the RET instruction.
 * Params:   none
 * Returns:  int - E.valB + 4
 * Modifies: none
 */
int performRet() {
    return E.valB + 4;
}

/* performPush
 *      Simulates the PUSHL instruction.
 * Params:   none
 * Returns:  int - E.valB -4
 * Modifies: Cnd
 */
int performPush(){
    Cnd = FALSE;
    return -4 + E.valB;
}

/* performPop
 *      Simulates the POPL instruction.  
 * Params:   none
 * Returns:  int - 4+ E.valB
 * Modifies: Cnd
 */
int performPop(){
    Cnd = FALSE;
    return 4 + E.valB;
}

/* performDump
 *      Simulates the DUMP instruction. *sigh*
 * Params:   none
 * Returns:  int - E.valC
 * Modifies: none
 */
int performDump() {
    return E.valC;
}

/* setCnd
 *      Sets the Cnd based on Condition Codes
 * Params:   u int fcode    - Function Code
 * Returns:  void
 * Modifies: Cnd
 */
void setCnd(unsigned int fCode) {
    switch (fCode) {
        case 0:   
            Cnd = TRUE;
            break;
        case CMOVLE:
            Cnd = (getCC(SF) && !getCC(OF)) ||
                  (!getCC(SF) && getCC(OF)) ||
                  getCC(ZF);
            break;
        case CMOVL: 
            Cnd = (getCC(SF) && !getCC(OF)) || (!getCC(SF) && getCC(OF));
            break;
        case CMOVE: 
            Cnd = getCC(ZF);
            break;
        case CMOVNE:
            Cnd = !getCC(ZF);
            break;
        case CMOVGE:
            Cnd = (getCC(SF) && getCC(OF)) || (!getCC(SF) && !getCC(OF));
            break;
        case CMOVG: 
            Cnd = (getCC(SF)  && getCC(OF)  && !getCC(ZF)) ||
                  (!getCC(SF) && !getCC(OF) && !getCC(ZF));
            break;
        default:
            Cnd = FALSE;
            break;
    }
}

/* updateCC
 *      Helper method to update ZF and SF condition codes.
 * Params:   int val - the value to set the CCs based on
 * Returns:  void
 * Modifies: none
 */
void updateCC(int val) {    
    if (!canUpdateCC)
        return;
    if (val == 0) {
        setCC(ZF, 1);
        setCC(SF, 0);
    } else if (isNeg(val)) {
        setCC(ZF, 0);
        setCC(SF, 1);
    } else {
        setCC(ZF, 0);
        setCC(SF, 0);
    }
}

/* M_stall
 *      Determines if the M Register needs to be stalled
 * Params:   none
 * Returns:  bool - TRUE if needs to be stalled
 * Modifies: none
 */
bool M_stall() {
    return FALSE;
}

/* M_bubble
 *      Determines if the M Register needs to be bubbled
 * Params:   fwdStruct fwd  - Forwarding Struct
 * Returns:  bool           - TRUE if needs to be bubbled
 * Modifies: none
 */
bool M_bubble(fwdStruct *fwd) {
    return (fwd->m_stat == SADR || fwd->m_stat == SINS || fwd->m_stat == SHLT) ||
           (fwd->W_stat == SADR || fwd->W_stat == SINS || fwd->W_stat == SHLT);
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
 * Modifies: eregister E
 */
void clearEregister() {
    canUpdateCC = TRUE;
    E.stat = SAOK;
    E.icode = NOP;
    E.ifun = 0;
    E.valC = 0;
    E.valA = 0;
    E.valB = 0;
    E.dstE = RNONE;
    E.dstM = RNONE;
    E.srcA = RNONE;
    E.srcB = RNONE;
}

/* updateEregister
 *      Sets the values in the Execute PIPE register to the specified values.
 * Params:   int stat    - Status of the Pipeline
 *           int icode   - Current Instruction
 *           int ifun    - Additional Information on Current Instruction
 *           int valC    - Current Condition
 *           int valA    - Val E of current Instruction
 *           int valB    - Val A of current Instruction
 *           int dstE    - Dst E of current Instruction
 *           int dstM    - Dst M of current Instruction
 *           int srcA    - Src A of current Instruction
 *           int srcB    - Src B of current Instruction
 *           
 * Returns:  void
 * Modifies: eregister E
 */
void updateEregister(int stat, int icode, int ifun, int valC, int valA,
                     int valB, int dstE, int dstM, int srcA, int srcB) {
    E.stat = stat;
    E.icode = icode;
    E.ifun = ifun;
    E.valC = valC;
    E.valA = valA;
    E.valB = valB;
    E.dstE = dstE;
    E.dstM = dstM;
    E.srcA = srcA;
    E.srcB = srcB;
}

/* initializeFuncPtrArray
 *      Intializes the Function Pointer Array
 * Params:   none
 * Returns:  void
 * Modifies: funcArr[]
 */
void initializeFuncPtrArray() {
    funcArr[HALT] = &doNothing;
    funcArr[NOP] = &doNothing;
    funcArr[CMOV] = &performRrmovl;
    funcArr[IRMOVL] = &performIrmovl;
    funcArr[RMMOVL] = &performRmmovl;
    funcArr[MRMOVL] = &performMrmovl;
    funcArr[OPL] = &performOpl;
    funcArr[JXX] = &performJXX;
    funcArr[CALL] = &performCall;
    funcArr[RET] = &performRet;
    funcArr[PUSHL] = &performPush;
    funcArr[POPL] = &performPop;
    funcArr[DUMP] = &performDump;
}


void printEregister() {
    printf("\n=== Execute Stage ===\n");
    printf("stat = %d\ticode = %x\tifun = %x\n", E.stat, E.icode, E.ifun);
    printf("valC = %x\tvalA = %x\tvalB = %x\n", E.valC, E.valA, E.valB);
    printf("dstE = %x\tdstM = %x\tsrcA = %x\tsrcB = %x\n", E.dstE, E.dstM, E.srcA, E.srcB);
}

