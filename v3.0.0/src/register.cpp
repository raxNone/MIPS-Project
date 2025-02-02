#include "../incs/register.hpp"
#include "memory.h" // for memset function

bitset<32> Register::operator[](REG_NUM num){   return reg[num];    }
bitset<32> Register::operator[](unsigned int num){   return reg[num];    }
REG_OUT Register::read(bitset<5> read_reg1, bitset<5> read_reg2){
    
}

void Register::write(bitset<5>write_reg, bitset<32>write_data, bitset<1> RegWrite){
    
}