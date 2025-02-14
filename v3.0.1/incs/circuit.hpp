#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__
#include <bitset>
#include "signal.hpp"
using namespace std;
#define TRUE true
#define FALSE false

typedef struct _ALU_OUT {
    bitset<1> zero;
    bitset<32> result;
} ALU_OUT;
typedef struct _CONTROL_OUT {
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

typedef struct _HAZARD_OUT {
    bitset<1> PCWrite;
    bitset<1> IFIDWrite;
    bitset<1> IF_FLUSH;
    bitset<1> ID_FLUSH;
    bitset<1> stall;
    bitset<1> IDEXWrite;

} HAZARD_OUT;

typedef struct _FORWARD_OUT {
    bitset<2> ForwardA;
    bitset<2> ForwardB;
} FORWARD_OUT;

bitset<1> not_gate(const bitset<1>& A);
bitset<1> and_gate(const bitset<1>& A, const bitset<1>& B);
bitset<1> or_gate(const bitset<1>& A, const bitset<1>& B);
bitset<1> xor_gate(const bitset<1>& A, const bitset<1>& B);

bitset<1> EQUAL_32(const bitset<32>& A, const bitset<32>& B);

bitset<28> shiftLeft2_26(const bitset<26>& i);
bitset<32> shiftLeft2_32(const bitset<32>& i);

bitset<32> adder_32(const bitset<32>& i1, const bitset<32>& i2);
bitset<32> mux_32_2to1_01(const bitset<32>& i1, const bitset<32>& i2, const bitset<1>& s);
bitset<32> mux_32_2to1_10(const bitset<32>& i1, const bitset<32>& i2, const bitset<1>& s);
bitset<5> mux_5_3to1_021(const bitset<5>& i1, const bitset<5>& i2, const bitset<5>& i3, bitset<2> s);
bitset<32> mux_32_3to1_102(const bitset<32>& i1, const bitset<32>& i2, const bitset<32>& i3, const bitset<2>& s);
CONTROL_OUT mux_control_01(const CONTROL_OUT& i1, const CONTROL_OUT& i2, const bitset<1>& s);
bitset<32> signExt(const bitset<16>& i);

ALU_OUT ALU(const bitset<32>& i1, const bitset<32>& i2, const bitset<4>& operation);
CONTROL_OUT Control(const bitset<6>& opcode, const bitset<5>& rs, const bitset<5>& rd);
bitset<4> ALUControl(const bitset<2>& ALUOp, const bitset<6>& funct);

HAZARD_OUT HazardDetectionUnit(const bitset<5>& rs, const bitset<5>& rt, const bitset<1>& Jump, const bitset<1>& JumpReg, const bitset<1>& Branch, const bitset<5>& EX_rd, const bitset<1>& EX_MemRead, const bitset<1>& branch_taken);
FORWARD_OUT ForwardingUnit(bitset<1> MEM_RegWrite, bitset<1> WB_RegWrite, bitset<5> EX_rs, bitset<5> EX_rt, bitset<5> MEM_rd, bitset<5> WB_rd);

#endif