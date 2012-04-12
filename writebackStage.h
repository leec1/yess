#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H
typedef struct {
    unsigned int stat;
    unsigned int icode; 
    unsigned int valE; 
    unsigned int valM; 
    unsigned int dstE; 
    unsigned int dstM;
} wregister;

wregister getWregister();

bool writebackStage(unsigned int *W_dstE, unsigned int *W_valE, unsigned int *W_icode);
void clearWregister();
void updateWregister(int stat, int icode, int valE, int valM,
                     int dstE, int dstM);
void printWregister();
#endif
