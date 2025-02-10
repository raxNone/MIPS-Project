#ifndef __REG_HPP__
#define __REG_HPP__
#include "string"
using namespace std;

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

typedef struct _REG_OUT{
    unsigned int data1;
    unsigned int data2;
} REG_OUT;


class Register{
private:
    unsigned int reg[32] = {0,};
public:
    const string reg_name[32] = {
    "$zero",
    "$at",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9",
    "$k0", "$k1",
    "$gp",
    "$sp",
    "$fp",
    "$ra"
    };
    void clearReg();
    unsigned int operator[](REG_NUM num);   // for GUI and debug
    REG_OUT read(unsigned int rr1, unsigned int rr2);
    void write(unsigned int wr, unsigned int wd, bool RegWrite);
    
};

#endif