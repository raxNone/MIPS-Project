#ifndef __MEM_HPP__
#define __MEM_HPP__
#include <vector>

typedef struct _MEM{
    unsigned int addr;
    unsigned int value;
} MEM;

class Memory{
private:
    std::vector<MEM> mem;

public:
    unsigned int& operator[](unsigned int addr){
        for(auto& entry : mem){
            if(entry.addr == addr){
                return entry.value;
            }
        }
    mem.push_back({addr, 0});
    return mem.back().value;
    }
    

};

#endif