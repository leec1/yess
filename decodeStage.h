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

void decodeStage();

dregister getDregister();

void clearDregister();

void updateDregister(int stat, int icode, int ifun, int rA, int rB,
        int valC, int valP);
#endif
