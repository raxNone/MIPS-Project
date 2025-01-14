#ifndef __MEM_HPP__
#define __MEM_HPP__
#include <vector>

typedef struct _MEMORY{
    unsigned int addr;
    unsigned int value;
} MEMORY;

class InstructionMemory{
private:
    std::vector<MEMORY> mem;    
public:
    unsigned int work(unsigned int addr);
};

class DataMemory{
private:
    std::vector<MEMORY> mem;
public:
    unsigned int work(unsigned int addr, unsigned int writeData, bool MemWrite, bool MemRead);
};

#endif