#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__
#include <bitset>
#include "signal.hpp"
using namespace std;

typedef struct _ALU_OUT{
    bitset<1> zero;
    bitset<32> result;
} ALU_OUT;
typedef struct _CONTROL_OUT{
    // id
    bitset<1> Branch;
    bitset<1> Jump;
    bitset<1> JumpReg;

    // ex
    bitset<1> ALUSrc;
    bitset<2> RegDst;
    bitset<2> ALUOp;

    // mem
    bitset<1> MemRead;
    bitset<1> MemWrite;

    // wb
    bitset<2> MemtoReg;
    bitset<1> RegWrite;
} CONTROL_OUT;

typedef struct _HAZARD_OUT{
    bitset<1> PCWrite;
    bitset<1> IFIDWrite;
    bitset<1> ControlMUX;
    bitset<1> IF_FLUSH;

} HAZARD_OUT;

typedef struct _FORWARD_OUT{
    bitset<2> ForwardA;
    bitset<2> ForwardB;
} FORWARD_OUT;

bitset<1> AND_1(bitset<1> i1, bitset<1> i2);
bitset<1> EQUAL_32(bitset<32> i1, bitset<32> i2);

bitset<28> shiftLeft2_26(bitset<26> i);
bitset<32> shiftLeft2_32(bitset<32> i);

bitset<32> adder_32(bitset<32> i1, bitset<32> i2);
bitset<5> mux_5_2to1_0(bitset<5> i1, bitset<5> i2, bitset<1> s);
bitset<32> mux_32_2to1_0(bitset<32> i1, bitset<32> i2, bitset<1> s);
bitset<32> mux_32_2to1_1(bitset<32> i1, bitset<32> i2, bitset<1> s);
bitset<5> mux_5_3to1_0(bitset<5> i1, bitset<5> i2, bitset<5> i3, bitset<2> s);  // 0 2 1
bitset<32> mux_32_3to1_1(bitset<32> i1, bitset<32> i2, bitset<32> i3, bitset<2> s); // 1 0 2
CONTROL_OUT mux_control_0(CONTROL_OUT i1, CONTROL_OUT i2, bitset<1> s);
bitset<32> signExt(bitset<16> i);

ALU_OUT ALU(bitset<32> i1, bitset<32> i2, bitset<4> operation);
CONTROL_OUT Control(bitset<6> op);
bitset<4> ALUControl(bitset<2> ALUOp, bitset<6> funct);

HAZARD_OUT HazardDetectionUnit(bitset<1> EX_MemRead, bitset<1> EX_Branch, bitset<1> MEM_Branch, bitset<5> ID_rt, bitset<5> ID_rs, bitset<5> EX_rt);
FORWARD_OUT ForwardingUnit(bitset<1> MEM_RegWrite, bitset<1> WB_RegWrite, bitset<5> EX_rs, bitset<5> EX_rt, bitset<5> MEM_rd, bitset<5> WB_rd);

#endif