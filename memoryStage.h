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
void clearMregister();
#endif
