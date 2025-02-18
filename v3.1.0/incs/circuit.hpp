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
    // idcontrol_zero
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
    bitset<1> IF_FLUSH;
    bitset<1> ID_FLUSH;
    bitset<1> stall;
    bitset<1> IDEXWrite;

} HAZARD_OUT;

typedef struct _FORWARD_OUT{
    bitset<2> ForwardA;
    bitset<2> ForwardB;
} FORWARD_OUT;
#include <bitset>

bool not_gate(const bool& A);
std::bitset<32> not_gate_32(const std::bitset<32>& A);

bool and_gate(const bool& A, const bool& B);
std::bitset<5> and_gate_5(const std::bitset<5>& A, const std::bitset<5>& B);
std::bitset<32> and_gate_32(const std::bitset<32>& A, const std::bitset<32>& B);
std::bitset<5> and_gate_1and5(const bool& A, const std::bitset<5>& B);
std::bitset<32> and_gate_1and32(const bool& A, const std::bitset<32>& B);

bool and_gate3(const bool& A, const bool& B, const bool& C);
bool and_gate4(const bool& A, const bool& B, const bool& C, const bool& D);
bool and_gate6(const bool& A, const bool& B, const bool& C, const bool& D, const bool& E, const bool& F);
bool and_gate8(const bool& A, const bool& B, const bool& C, const bool& D, const bool& E, const bool& F, const bool& G, const bool& H);

bool or_gate(const bool& A, const bool& B);
std::bitset<5> or_gate_5(const std::bitset<5>& A, const std::bitset<5>& B);
std::bitset<32> or_gate_32(const std::bitset<32>& A, const std::bitset<32>& B);
bool or_gate3(const bool& A, const bool& B, const bool& C);
std::bitset<32> or_gate3_32(const std::bitset<32> A, const std::bitset<32>& B, const std::bitset<32>& C);
std::bitset<32> or_gate6_32(const std::bitset<32>& A, const std::bitset<32>& B, const std::bitset<32>& C, const std::bitset<32>& D, const std::bitset<32>& E, const std::bitset<32>& F);

bool xor_gate(const bool& A, const bool& B);
std::bitset<5> xor_gate_5(const std::bitset<5>& A, const std::bitset<5>& B);
std::bitset<32> xor_gate_32(const std::bitset<32>& A, const std::bitset<32>& B);

bool EQUAL_32(const std::bitset<32>& A, const std::bitset<32>& B);
bool check_5bit_zero(const std::bitset<5>& A);
bool fullAdder(const bool& A, const bool& B, const bool& Cin, bool& Cout);
std::bitset<32> sub_32(const std::bitset<32>& A, const std::bitset<32>& B);
bool slt_32(const std::bitset<32>& A, const std::bitset<32>& B);

std::bitset<28> shiftLeft2_26(const std::bitset<26>& i);
std::bitset<32> shiftLeft2_32(const std::bitset<32>& i);
std::bitset<32> adder_32(const std::bitset<32>& i1, const std::bitset<32>& i2);

bool mux_2to1(const bool& sel, const bool& a, const bool& b);
std::bitset<32> mux_32_2to1_01(const std::bitset<32>& i1, const std::bitset<32>& i2, const bitset<1>& s);
std::bitset<32> mux_32_2to1_10(const std::bitset<32>& i1, const std::bitset<32>& i2, const std::bitset<1>& s);
std::bitset<5> mux_5_3to1_021(const std::bitset<5>& i1, const std::bitset<5>& i2, const std::bitset<5>& i3, std::bitset<2> s);
std::bitset<32> mux_32_3to1_102(const std::bitset<32>& i1, const std::bitset<32>& i2, const std::bitset<32>& i3, const std::bitset<2>& s);

CONTROL_OUT mux_control_10(const CONTROL_OUT& i1, const CONTROL_OUT& i2, const bitset<1>& sig);
std::bitset<32> signExt(const std::bitset<16>& i);
ALU_OUT ALU(const bitset<32>& A, const bitset<32>& B, const bitset<4>& op);
CONTROL_OUT Control(const bitset<6>& opcode, const bitset<5>& rs, const bitset<5>& rd);
bitset<4> ALUControl(const bitset<2>& ALUOp, const bitset<6>& funct);
HAZARD_OUT HazardDetectionUnit(const bitset<5>& rs, const bitset<5>& rt, const bitset<1>& Jump, const bitset<1>& JumpReg, const bitset<1>& Branch, const bitset<5>& EX_rd, const bitset<1>& EX_MemRead, const bitset<1>& branch_taken);
FORWARD_OUT ForwardingUnit(const bitset<1>& MEM_RegWrite, const bitset<1>& WB_RegWrite, const bitset<5>& EX_rs, const bitset<5>& EX_rt, const bitset<5>& MEM_rd, const bitset<5>& WB_rd);
#endif