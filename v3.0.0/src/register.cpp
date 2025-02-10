#include "../incs/register.hpp"
#include "memory.h" // for memset function

bitset<32> Register::operator[](REG_NUM num){   return reg[num];    }
bitset<32> Register::operator[](unsigned int num){   return reg[num];    }
REG_OUT Register::read(const bitset<5>& rs, const bitset<5>& rt){
    return {reg[rs.to_ulong()], reg[rt.to_ulong()]};
}

void Register::write(const bitset<5>& rd, const bitset<32>& data, bitset<1> RegWrite){
    if (RegWrite[0])
        reg[rd.to_ulong()] = data;
}
void Register::reset(){
    std::fill(std::begin(reg), std::end(reg), std::bitset<32>(0));
}