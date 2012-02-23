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

void executeStage();
eregister getEregister();
void clearEregister();

void updateEregister(int stat, int icode, int ifun, int rA, int rB,
        int valC, int valP);

#endif
