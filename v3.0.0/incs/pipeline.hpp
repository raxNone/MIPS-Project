#ifndef __PIPELINE__
#define __PIPELINE__
#include <bitset>
#include "signal.hpp"
#include "circuit.hpp"
using namespace std;

// Pipeline Register


class IFID_reg{
public:
    bitset<1> IFIDWrite = 1;
    bitset<32> next_pc;
    bitset<32> instruction;
    bitset<1> flush;
    void setReg(bitset<32> next_pc, bitset<32> instruction);
    void setFlush(bitset<1> flush);
};

class IDEX_reg{
public:
    WB_SIG signal_wb;
    MEM_SIG signal_mem;
    EX_SIG signal_ex;
    bitset<32> next_pc;
    bitset<32> rs_data;
    bitset<32> rt_data;
    bitset<32> constant;
    bitset<5> rs;
    bitset<5> rt;
    bitset<5> rd;
    void setSig(CONTROL_OUT sig);

};
    

class EXMEM_reg{
public:
    WB_SIG signal_wb;
    MEM_SIG signal_mem;
    bitset<32> next_pc;
    bitset<32> BTA;
    bitset<1> zero;
    bitset<32> ALU_result;
    bitset<32> rt_data;
    bitset<5> write_reg;
};

class MEMWB_reg{
public:
    WB_SIG signal_wb;
    bitset<32> next_pc;
    bitset<32> mem_data;
    bitset<32> ALU_result;
    bitset<5> write_reg;
};


#endif