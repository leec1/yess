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

bool writebackStage();
wregister getWregister();
void clearWregister();

void updateWregister(int stat, int icode, int valE, int valM,
                     int dstE, int dstM);

#endif
