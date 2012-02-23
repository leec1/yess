#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
typedef struct {
    unsigned int predPC;
} fregister;

fregister getFregister();
void clearFregister();
#endif
