#ifndef TYPES_H
#define TYPES_H
#define TRUE  1
#define FALSE 0
typedef int bool;

typedef struct {
    //fetch
    unsigned int d_srcA;
    unsigned int d_srcB;
    //decode
    unsigned int D_icode;
    //execute
    unsigned int E_icode;
    unsigned int e_Cnd;
    unsigned int e_dstE;
    unsigned int e_valE;
    unsigned int E_dstM;
    //memory
    unsigned int M_icode;
    unsigned int M_Cnd;
    unsigned int m_stat;
    unsigned int M_dstM;
    unsigned int M_dstE;
    unsigned int m_valM;
    unsigned int M_valE;
    unsigned int M_valA;
    //writeback
    unsigned int W_icode;
    unsigned int W_stat;
    unsigned int W_dstM;
    unsigned int W_dstE;
    unsigned int W_valM;
    unsigned int W_valE;
} fwdStruct;

//Y-86 Instruction Codes
#define HALT   0x0
#define NOP    0x1
#define CMOV   0x2
#define IRMOVL 0x3
#define RMMOVL 0x4
#define MRMOVL 0x5
#define OPL    0x6
#define JXX    0x7 
#define CALL   0x8
#define RET    0x9
#define PUSHL  0xa
#define POPL   0xb
#define DUMP   0xc

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

//Branches
#define JMP 0
#define JLE 1
#define JL  2
#define JE  3
#define JNE 4
#define JGE 5
#define JG  6

//Moves
#define RRMOVL 0
#define CMOVLE 1
#define CMOVL  2
#define CMOVE  3
#define CMOVNE 4
#define CMOVGE 5
#define CMOVG  6

#endif
