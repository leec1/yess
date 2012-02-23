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
void clearDregister();
#endif
