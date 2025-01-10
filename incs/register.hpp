#ifndef __REG_HPP__
#define __REG_HPP__

#include "memory.h" // for memset function

typedef enum _REG_NUM{
    $zero = 0,
    $at,
    $v0, $v1,
    $a0, $a1, $a2, $a3,
    $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7,
    $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7,
    $t8, $t9,
    $k0, $k1,
    $gp,
    $sp,
    $fp,
    $ra
} REG_NUM;

class Register{
private:
    unsigned int reg[32];

public:
    void clearReg(){    memset(reg,0,32);   }
    // void setReg(REG_NUM num, unsigned int value){   reg[num] = value;   }
    // unsigned int getReg(REG_NUM num){   return reg[num];    }
    unsigned int& operator[](REG_NUM num){   return reg[num];    }
};

#endif