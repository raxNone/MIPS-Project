#ifndef __REG_HPP__
#define __REG_HPP__
#include <bitset>
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

typedef struct _REG_OUT{
    bitset<32> read_data1;
    bitset<32> read_data2;
} REG_OUT;


class Register{
private:
    bitset<32> reg[32];
public:
    bitset<32> operator[](REG_NUM num);
    bitset<32> operator[](unsigned int num);
    REG_OUT read(bitset<5> read_reg1, bitset<5> read_reg2);
    void write(bitset<5>write_reg, bitset<32>write_data, bitset<1> RegWrite);
};

#endif