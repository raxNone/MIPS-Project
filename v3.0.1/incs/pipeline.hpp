#ifndef __PIPELINE__
#define __PIPELINE__
#include <bitset>
#include "signal.hpp"
using namespace std;

// Pipeline Register


class IFID_Class{
private:
public:
    bitset<1> IFIDWrite = 1;
    struct IFID_REG{
        bitset<32> next_pc;
        bitset<32> instruction;
        bitset<1> flush;
    } IFID ;
    void setFlush(bitset<1> flush);
};

typedef struct _IDEX{
    WB_SIG signal_wb;
    MEM_SIG signal_mem;
    EX_SIG signal_ex;
    
} IDEX;

typedef struct _EXMEM{
    WB_SIG signal_wb;
    MEM_SIG signal_mem;
} EXMEM;

typedef struct _MEMWB{
    WB_SIG signal_wb;
} MEMWB;


#endif