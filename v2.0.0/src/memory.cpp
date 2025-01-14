#include "../incs/memory.hpp"

unsigned int InstructionMemory::work(unsigned int addr){
    for(auto& entry : mem){
        if(entry.addr == addr){
        return entry.value;
        }
    }
    return 0;
}
unsigned int DataMemory::work(unsigned int addr, unsigned int writeData, bool MemWrite, bool MemRead){
    if (MemWrite){
        for(auto& entry : mem){
            if(entry.addr == addr){
                entry.value = writeData;
                return 0;
            }
        }
        mem.push_back({addr, writeData});
        return 0;
    }
    else if(MemRead){
        for(auto& entry : mem){
            if(entry.addr == addr){
                return entry.value;
            }
        }
        return 0;
    }
    return 0;
}