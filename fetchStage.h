#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
typedef struct {
    unsigned int predPC;
} fregister;

void fetchStage();
fregister getFregister();
bool instructionNeedsRegByte(int icode);
void clearFregister();
void printFregister();

#endif
