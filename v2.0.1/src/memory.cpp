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
                mem.push_back({mem.back().addr+4, num});
        }
        return true;
    }
    return false;
    
}

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