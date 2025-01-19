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
    bitset<2> RegDst;
    bitset<1> ALUSrc;
    bitset<2> MemtoReg;
    bitset<1> RegWrite;
    bitset<1> MemRead;
    bitset<1> MemWrite;
    bitset<1> Branch;
    bitset<1> Jump;
    bitset<1> JumpReg;
    bitset<2> ALUOp;
} CONTROL_OUT;

typedef struct _HAZARD_OUT{
    bitset<1> PCWrite;
    bitset<1> IFIDWrite;
    bitset<1> ControlMUX;
} HAZARD_OUT;

typedef struct _FORWARD_OUT{
    bitset<2> ForwardA;
    bitset<2> ForwardB;
} FORWARD_OUT;

bitset<28> shiftLeft2_26(bitset<26> i);
bitset<32> shiftLeft2_32(bitset<32> i);

bitset<32> adder_32(bitset<32> i1, bitset<32> i2);
bitset<5> mux_5_2to1_0(bitset<5> i1, bitset<5> i2, bitset<1> s);
bitset<32> mux_32_2to1_0(bitset<32> i1, bitset<32> i2, bitset<1> s);
bitset<32> mux_32_2to1_1(bitset<32> i1, bitset<32> i2, bitset<1> s);
bitset<5> mux_5_3to1_0(bitset<5> i1, bitset<5> i2, bitset<5> i3, bitset<2> s);
bitset<32> mux_32_3to1_1(bitset<32> i1, bitset<32> i2, bitset<32> i3, bitset<2> s);
bitset<32> signExt(bitset<16> i);

ALU_OUT ALU(bitset<32> i1, bitset<32> i2, bitset<32> op);
CONTROL_OUT Control(bitset<6> op);
bitset<4> ALUControl(bitset<2> ALUOp, bitset<6> funct);

HAZARD_OUT HazardDetectionUnit(bool MemRead, unsigned int IDrt, unsigned int IDrs, unsigned int EXrt);
FORWARD_OUT ForwardingUnit(bool MEMRegWrite, bool WBRegWrite, unsigned int EXrs, unsigned int EXrt, unsigned int MEMrd, unsigned int WBrd);

#endif