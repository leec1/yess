#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
typedef struct {
    unsigned int predPC;
} fregister;

fregister getFregister();

void fetchStage(fwdStruct *fwd);

bool instructionNeedsRegByte(int icode);
int need_valC(int icode);
void clearFregister();
void printFregister();
#endif
