#ifndef MEMORY_H
#define MEMORY_H

#define MEMSIZE 1024     //1024 words of memory

unsigned char getByte(int address, bool * memError);
void putByte(int address, unsigned char value, bool * memError);
unsigned int getWord(int address, bool * memError);
void putWord(int address, unsigned int value, bool * memError);
void clearMemory();
#endif 

