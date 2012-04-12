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

void decodeStage(unsigned int *W_dstE, unsigned int *W_valE,
                 unsigned int *e_dstE, unsigned int *e_valE,
                 unsigned int *M_dstM, unsigned int *m_valM,
                 unsigned int *M_dstE, unsigned int *M_valE,
                 unsigned int *W_dstM, unsigned int *W_valM);

unsigned int getSrcA();
unsigned int getSrcB();
int getDstE();
int getDstM();
int selectFwdA(unsigned int d_srcA, unsigned int W_dstE, unsigned int W_valE, 
               unsigned int e_dstE, unsigned int e_valE, unsigned int M_dstM,
               unsigned int m_valM, unsigned int M_dstE, unsigned int M_valE,
               unsigned int W_dstM, unsigned int W_valM);
int forwardB(unsigned int d_srcB, unsigned int W_dstE, unsigned int W_valE, 
             unsigned int e_dstE, unsigned int e_valE, unsigned int M_dstM,
             unsigned int m_valM, unsigned int M_dstE, unsigned int M_valE,
             unsigned int W_dstM, unsigned int W_valM);

void clearDregister();
void updateDregister(int stat, int icode, int ifun, int rA, int rB, int valC,
                     int valP);
void printDregister();
#endif
