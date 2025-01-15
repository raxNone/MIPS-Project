#ifndef __PIPELINE__
#define __PIPELINE__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "../incs/register.hpp"
#include "../incs/memory.hpp"
#include "../incs/circuit.hpp"
using namespace std;

extern Register reg;
extern InstructionMemory iMem;
extern DataMemory dMem;
class _IF;
class _ID;
class _EX;
class _MEM;
class _WB;
extern _IF IF ;
extern _ID ID ;
extern _EX EX ;
extern _MEM MEM ;
extern _WB WB;


typedef struct _EX_CONTROL{
    bool RegDst=0;
    bool ALUOp1=0;
    bool ALUOp0=0;
    bool ALUSrc=0;
} EX_CONTROL;

typedef struct _MEM_CONTROL{
    bool Branch=0;
    bool MemRead=0;
    bool MemWrite=0;
} MEM_CONTROL;

typedef struct _WB_CONTROL{
    bool RegWrite=0;
    bool MemtoReg=0;
} WB_CONTROL;

typedef struct _IF_OUT{
    unsigned int nextPc=0;
    unsigned int instruction=0;
    bool Flush=0;
} IF_OUT;

typedef struct _ID_OUT{
    WB_CONTROL control_wb;
    MEM_CONTROL control_mem;
    EX_CONTROL control_ex;
    unsigned int rs_data=0;
    unsigned int rt_data=0;
    unsigned int constant=0;
    unsigned int rs=0;
    unsigned int rt=0;
    unsigned int rd=0;

} ID_OUT;

typedef struct _EX_OUT{
    WB_CONTROL control_wb;
    MEM_CONTROL control_mem;
    unsigned int ALU_result=0;
    unsigned int write_data=0;
    unsigned int write_reg=0;

} EX_OUT;

typedef struct _MEM_OUT{
    WB_CONTROL control_wb;
    unsigned int read_data=0;
    unsigned int not_read_data=0;
    unsigned int write_reg=0;

} MEM_OUT;

class _IF{
public:
    unsigned int pc=0;
    bool branch_sig=0;
    bool PCWrite = 1;
    bool Flush = 0;
    unsigned int BTA=0;
    unsigned int nextPc=0;
    IF_OUT work();
};

class _ID{
public:
    unsigned int nextPc=0;
    unsigned int instruction=0;
    bool IFIDWrite=1;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int constant;
    CONTROL_OUT control_sig;
    HAZARD_OUT Hazard_result;
    REG_OUT reg_data;

    void load(IF_OUT IFID);
    void setSig();
    ID_OUT work();

};

class _EX{
public:
    WB_CONTROL control_wb;
    MEM_CONTROL control_mem;
    EX_CONTROL control_ex;
    FORWARD_OUT Forward_result;
    unsigned int ForwardA=0;
    unsigned int ForwardB=0;
    unsigned int ALU_control_sig;
    unsigned int rs_data=0;
    unsigned int rt_data=0;
    unsigned int constant=0;
    unsigned int rs=0;
    unsigned int rt=0;
    unsigned int rd=0;
    EX_OUT work();
    void setSig();
    void load(ID_OUT IDEX);
};

class _MEM{
public:
    WB_CONTROL control_wb;
    MEM_CONTROL control_mem;
    bool zero;
    unsigned int ALU_result=0;
    unsigned int write_data=0;
    unsigned int write_reg=0;
    void load(EX_OUT EXMEM);
    MEM_OUT work();

};

class _WB{
public:
    WB_CONTROL control_wb;
    unsigned int read_data=0;
    unsigned int not_read_data=0;
    unsigned int write_data = 0;
    unsigned int write_reg=0;
    void load(MEM_OUT MEMWB);
    void work();
};

unsigned int subBit(unsigned int num, int start, int end);
string toHexString(unsigned int num);


#endif