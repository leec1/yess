#ifndef REGISTERS_H
#define REGISTERS_H

#define REGSIZE 8

#define EAX    0x0
#define ECX    0x1
#define EDX    0x2
#define EBX    0x3
#define ESP    0x4
#define EBP    0x5
#define ESI    0x6
#define EDI    0x7

#define OF 0
#define SF 1
#define ZF 2

unsigned int getRegister(int regNum);
void setRegister(int regNum, unsigned int regValue);
void clearRegisters();
unsigned int getCC(unsigned int bitNum);
void setCC(unsigned int bitNum, unsigned int bitValue);
void initializeCC();
#endif

