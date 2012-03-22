#ifndef TOOLS_H
#define TOOLS_H
void clearBuffer(char *reg, unsigned int size);
unsigned char getByteNumber(int byteNumber, unsigned int value);
unsigned int putByteNumber(int byteNumber, unsigned char byte,
                            unsigned int value);
unsigned int buildWord(unsigned char byte0, unsigned char byte1, 
                        unsigned char byte2, unsigned char byte3);
unsigned int getBits(int low, int high, unsigned int source);
unsigned int setBits(int low, int high, unsigned int source);
unsigned int clearBits(int low, int high, unsigned int source);
unsigned int assignOneBit(int bitNumber, int bitValue, unsigned int source);
void printBits(unsigned int source);
bool isNeg(unsigned int source);
#endif
