#include "../incs/register.hpp"
#include "memory.h" // for memset function

void Register::clearReg(){    memset(reg,0,32);   }
unsigned int Register::operator[](REG_NUM num){   return reg[num];    }
REG_OUT Register::read(unsigned char rr1, unsigned char rr2){
    return {reg[rr1%0x1f], reg[rr2%0x1f]};
}
void Register::write(unsigned char wr, unsigned int wd, bool RegWrite){
    if (RegWrite)        reg[wr%0x1f] = wd;
}