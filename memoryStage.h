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

mregister getMregister();

void memoryStage(fwdStruct *fwd);

void memoryControl(bool *readC, bool *writeC);
int memoryAddr();
void clearMregister();
void updateMregister(int stat, int icode, int Cnd, int valE, int valA,
                     int dstE, int dstM);
void printMregister();
#endif
