#ifndef MEMORYTAGE_H
#define MEMORYSTAGE_H
typedef struct {
    unsigned int stat; 
    unsigned int icode; 
    unsigned int Cnd; 
    unsigned int valE; 
    unsigned int valA; 
    unsigned int dstE; 
    unsigned int dstM; 
} mregister;

void memoryStage();
mregister getMregister();
void clearMregister();


void updateMregister(int stat, int icode, int Cnd, int valE, int valA,
                     int dstE, int dstM);

#endif
