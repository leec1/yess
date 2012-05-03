#ifndef DECODESTAGE_H
#define DECODESTAGE_H
typedef struct {
    unsigned int stat;
    unsigned int icode;
    unsigned int ifun;
    unsigned int rA;
    unsigned int rB;
    unsigned int valC;
    unsigned int valP;
} dregister;


dregister getDregister();

void decodeStage(fwdStruct *fwd);

unsigned int getSrcA();
unsigned int getSrcB();
int getDstE();
int getDstM();
int selectFwdA(unsigned int d_srcA, fwdStruct *fwd);
int forwardB(unsigned int d_srcB, fwdStruct *fwd);
void clearDregister();
void updateDregister(int stat, int icode, int ifun, int rA, int rB, int valC,
                     int valP);
void printDregister();
#endif
