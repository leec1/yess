#ifndef TYPES_H
#define TYPES_H
#define TRUE 1
#define FALSE 0
typedef int bool;

//Y-86 Instruction Codes
#define HALT 0x0
#define NOP 0x1
#define CMOV 0x2
#define IRMOVL 0x3
#define RMMOVL 0x4
#define MRMOVL 0x5
#define OPL 0x6
#define JXX 0x7 
#define CALL 0x8
#define RET 0x9
#define PUSHL 0xa
#define POPL 0xb
#define DUMP 0xc

//Stat Codes
#define RNONE 0xf

#define SAOK 1
#define SHLT 2
#define SADR 3 
#define SINS 4

//Function Codes

//Operations

#define ADD 0
#define SUB 1
#define AND 2
#define XOR 3

#endif
