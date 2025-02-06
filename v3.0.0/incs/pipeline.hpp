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
    bitset<1> flush = 0;
    struct _IFID{
        bitset<32> next_pc;
        bitset<32> instruction;
    } reg;
    void setReg(const _IFID& reg);
};

class IDEX_reg{
public:
    bitset<1> IDEXWrite = 1;
    bitset<1> flush = 0;
    struct _IDEX{
        WB_SIG signal_wb;
        MEM_SIG signal_mem;
        EX_SIG signal_ex;
        bitset<32> next_pc;
        bitset<32> a;
        bitset<32> b;
        bitset<32> constant;
        bitset<5> rs;
        bitset<5> rt;
        bitset<5> rd;
    } reg;
    void setReg(const _IDEX& reg);

};
    

class EXMEM_reg{
public:
    struct _EXMEM{
        WB_SIG signal_wb;
        MEM_SIG signal_mem;
        bitset<32> next_pc;
        bitset<1> zero;
        bitset<32> ALUOut;
        bitset<32> b;
        bitset<5> rd;
    } reg;
    void setReg(const _EXMEM& reg);
};

class MEMWB_reg{
public:
    struct _MEMWB{
        WB_SIG signal_wb;
        bitset<32> next_pc;
        bitset<32> wb_data;
        bitset<5> rd;
    } reg;
    void setReg(const _MEMWB& reg);
};


#endif