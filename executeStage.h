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

void executeStage();
int performOpl();
int performIrmovl();
int doNothing();
int dump();
void updateCC(int val);
void clearEregister();
void updateEregister(int stat, int icode, int ifun, int valC, int valA,
                     int valB, int dstE, int dstM, int srcA, int srcB);
void initializeFuncPtrArray();
void printEregister();
#endif
