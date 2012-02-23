#ifndef REGISTERS_H
#define REGISTERS_H

#define EAX    0x0
#define ECX    0x1
#define EDX    0x2
#define EBX    0x3
#define ESP    0x4
#define EBP    0x5
#define ESI    0x6
#define EDI    0x7
#define REGSIZE 8    //number of registers

unsigned int getRegister(int regNum);
void setRegister(int regNum, unsigned int regValue);
void clearRegisters();
#endif

