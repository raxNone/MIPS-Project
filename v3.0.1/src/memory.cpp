#include "../incs/memory.hpp"

bool InstructionMemory::load(ifstream& inFile){
    if(inFile.is_open()){
        string line;
        unsigned int num;
        while(getline(inFile, line)){
            num = stoul(line,nullptr, 2);   //str(bin) to ui
            if(empty(mem))
                mem.push_back({0, num});
            else
                mem.push_back({adder_32(mem.back().addr, 4), num});
        }
        return true;
    }
    return false;
    
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