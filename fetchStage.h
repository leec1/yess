#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
typedef struct {
    unsigned int predPC;
} fregister;

fregister getFregister();

void fetchStage(unsigned int *M_Cnd, unsigned int *M_icode,
                unsigned int *M_valA, unsigned int *W_icode);
bool instructionNeedsRegByte(int icode);
int need_valC(int icode);
void clearFregister();
void printFregister();
#endif
