#ifndef TYPES_H
#define TYPES_H
#define TRUE 1
#define FALSE 0
typedef int bool;

//Y-86 Instruction Codes
#define HALT 0
#define NOP 1
#define CMOV 2
#define IRMOVL 3
#define RMMOVL 4
#define MRMOVL 5
#define OPL 6
#define JXX 7 
#define CALL 8
#define RET 9
#define PUSHL 10
#define POPL 11
#define DUMP 12

//Stat Codes- Valid?
#define RNONE 15

#define SAOK 1
#define SHLT 2
#define SADR 3 
#define SINS 4

#endif
