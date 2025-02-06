#ifndef __SIGNAL_HPP__
#define __SIGNAL_HPP__
#include <bitset>
using namespace std;

// Control Signal
typedef struct _EX_SIG{
    bitset<2> RegDst;
    bitset<1> ALUSrc;
    bitset<2> ALUOp;
} EX_SIG;

typedef struct _MEM_SIG{
    bitset<1> MemRead;
    bitset<1> MemWrite;
    bitset<1> Branch;
    bitset<1> Jump;
    bitset<1> JumpReg;
} MEM_SIG;

typedef struct _WB_SIG{
    bitset<2> MemtoReg;
    bitset<1> RegWrite;
} WB_SIG;


#endif