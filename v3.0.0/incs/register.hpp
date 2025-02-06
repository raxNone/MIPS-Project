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
    bitset<32> a;
    bitset<32> b;
} REG_OUT;


class Register{
private:
    bitset<32> reg[32];
public:
    bitset<32> operator[](REG_NUM num);
    bitset<32> operator[](unsigned int num);
    REG_OUT read(const bitset<5>& rs, const bitset<5>& rt);
    void write(const bitset<5>& rd, const bitset<32>& data, bitset<1> RegWrite);
};

#endif