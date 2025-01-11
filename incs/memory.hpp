#ifndef __MEM_HPP__
#define __MEM_HPP__
#include <vector>

typedef struct _MEM{
    unsigned int addr;
    unsigned int value;
} MEM;

class InstructionMemory{
private:
    std::vector<MEM> mem;    
public:
    unsigned int work(unsigned int addr);
};

class DataMemory{
private:
    std::vector<MEM> mem;
public:
    unsigned int work(unsigned int addr, unsigned int writeData, bool MemWrite);
};

#endif