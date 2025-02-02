#include "../incs/memory.hpp"

void InstructionMemory::load(bitset<32> addr, bitset<32> data){
    mem.push_back({addr, data});
}

bitset<32> InstructionMemory::read(bitset<32> addr){
    for(auto& entry : mem){
        if(entry.addr == addr){
            return entry.value;
        }
    }
    return 0;
}
bitset<32> DataMemory::work(bitset<32> addr, bitset<32> writeData, bitset<1> MemRead, bitset<1> MemWrite){
    if (MemWrite[0]){
        for(auto& entry : mem){
            if(entry.addr == addr){
                entry.value = writeData;
                return 0;
            }
        }
        mem.push_back({addr, writeData});
        return 0;
    }
    else if(MemRead[0]){
        for(auto& entry : mem){
            if(entry.addr == addr){
                return entry.value;
            }
        }
        return 0;
    }
    return 0;
}