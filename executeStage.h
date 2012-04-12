#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
typedef struct {
    unsigned int stat;
    unsigned int icode;
    unsigned int ifun;
    unsigned int valC;
    unsigned int valA;
    unsigned int valB;
    unsigned int dstE;
    unsigned int dstM;
    unsigned int srcA;
    unsigned int srcB;
} eregister;

eregister getEregister();

void executeStage(unsigned int *e_dstE, unsigned int *e_valE);

int doNothing();
int performRrmovl();
int performIrmovl();
int performRmmovl();
int performMrmovl();
int performOpl();
int performJXX();
int performCall();
int performRet();
int performPop();
int performPush();
int performDump();

void updateCC(int val);
void clearEregister();
void updateEregister(int stat, int icode, int ifun, int valC, int valA,
                     int valB, int dstE, int dstM, int srcA, int srcB);

void initializeFuncPtrArray();
void printEregister();
#endif
