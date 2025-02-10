#include "../incs/circuit.hpp"

bitset<1> not_gate(const bitset<1>& A) {
    if (A[0])
        return FALSE;
    else
        return TRUE;
}
bitset<1> and_gate(const bitset<1>& A, const bitset<1>& B){
    if(A[0] && B[0])
        return TRUE;
    else
        return FALSE;
}
bitset<1> or_gate(const bitset<1>& A, const bitset<1>& B) {
    if (A[0] || B[0])
        return TRUE;
    else
        return FALSE;
}
bitset<1> xor_gate(const bitset<1>& A, const bitset<1>& B) {
    if (A[0] == B[0])
        return TRUE;
    else
        return FALSE;
}

bitset<1> EQUAL_32(const bitset<32>& A, const bitset<32>& B){
    if(A == B)
        return TRUE;
    else
        return FALSE;
}

bitset<28> shiftLeft2_26(const bitset<26>& i){
    return static_cast<bitset<28>>(i.to_ulong())<<2;
}
bitset<32> shiftLeft2_32(const bitset<32>& i){
    return i<<2;
}



// bitset<1> or_gate3(bitset<1> A, bitset<1> B, bitset<32> C) {
//     return or_gate_1(A, or_gate_1(B, C));
// }
// bitset<1> or_gate6(bitset<32> A, bitset<32> B, bitset<32> C, bitset<32> D, bitset<32> E, bitset<32> F) {
//     return or_gate(or_gate3(A, B, C), or_gate3(D, E, F));
// }
// bitset<1> and_gate(bitset<32> A, bitset<32> B) {
//     if (A && B)
//         return TRUE;
//     else
//         return FALSE;
// }


bitset<32> adder_32(const bitset<32>& i1, const bitset<32>& i2){
    return i1.to_ulong() + i2.to_ulong();
}
bitset<32> mux_32_2to1_01(const bitset<32>&i1, const bitset<32>& i2, const bitset<1>& s){
    if(s[0])
        return i2;
    else
        return i1;
}
bitset<32> mux_32_2to1_10(const bitset<32>&i1, const bitset<32>& i2, const bitset<1>& s){
    if(s[0])
        return i1;
    else
        return i2;
}

bitset<5> mux_5_3to1_021(const bitset<5>& i1, const bitset<5>& i2, const bitset<5>& i3, bitset<2> s){
    if(s[1])
        return i2;
    else if(s[0])
        return i3;
    else
        return i1;
}



bitset<32> mux_32_3to1_102(const bitset<32>& i1, const bitset<32>& i2, const bitset<32>& i3, const bitset<2>& s){
    // 1 0 2
    if(s[1])
        return i3;
    if(s[0])
        return i1;
    else
        return i2;
}


CONTROL_OUT mux_control_01(const CONTROL_OUT& i1, const CONTROL_OUT& i2, const bitset<1>& s){
    // 0 1
    if(s[0])
        return i2;
    else
        return i1;
    
}

bitset<32> signExt(const bitset<16>& i){
    return i.to_ulong();
}

ALU_OUT ALU(const bitset<32>& i1, const bitset<32>& i2, const bitset<4>& op){
    uint32_t A = i1.to_ullong();
    uint32_t B = i2.to_ulong();
    switch(op.to_ulong()){
        case 0b0000:    //AND
            return {!(A&B), A&B};
        case 0b0001:    //OR
            return {!(A|B), A|B};
        case 0b0010:    //add
            return {!(A+B), A+B};
        case 0b0110:    //sub
            return {!(A-B), A-B};
        case 0b0111:    //slt
            return {(A==B), A<B};
        case 0b1100:    //NOR
            return {(A|B), ~(A|B)};
    }
    return {0,0};
}
CONTROL_OUT Control(const bitset<6>& opcode, const bitset<5>& rs, const bitset<5>& rd){
    CONTROL_OUT result;
    switch (opcode.to_ulong()){
        case 0b000000:  //R-format
            if(!rd.to_ulong()){
                if (rs.to_ulong()){
                    // jr
                    result.RegWrite = 0;
                    result.JumpReg = 1;
                }else{
                    //nop
                }
            }
            else{
                result.RegDst = 1;
                result.RegWrite = 1;
            }
            break;
        case 0b000010:  // j
            result.Jump = 1;
            break;
        case 0b000011:  // jal
            result.RegDst = 2;
            result.MemtoReg = 2;
            result.RegWrite = 1;
            result.Jump = 1;
            break;
        case 0b000100:  // beq
            result.Branch = 1;
            break;
        case 0b100011:  //lw
            result.ALUSrc = 1;
            result.MemtoReg = 1;
            result.RegWrite = 1;
            result.MemRead = 1;
            break;
        case 0b101011:  //sw
            result.ALUSrc = 1;
            result.MemWrite = 1;
            break;
        case 0b001000:  //addi
            result.ALUSrc = 1;
            result.RegWrite = 1;
            break;
    }
    return result;
}
bitset<4> ALUControl(const bitset<2>& ALUOp, const bitset<6>& funct){
    if (ALUOp[1]){  // R-type
        switch(funct.to_ulong()){
            case 0b0000:    return 0b0010;  //add
            case 0b0010:    return 0b0110;  //sub
            case 0b0100:    return 0b0000;  //AND
            case 0b0101:    return 0b0001;  //OR
            case 0b1010:    return 0b0111;  //slt
        }
    }
    else if (ALUOp[0])  return 0b0110;  //sub
    return 0b0010;  //add
}

HAZARD_OUT HazardDetectionUnit(const bitset<5>& rs, const bitset<5>& rt, const bitset<1>& Jump, const bitset<1>& JumpReg, const bitset<1>& Branch, const bitset<5>& EX_rd, const bitset<1>& EX_MemRead, const bitset<1>& branch_taken){
    if (EX_MemRead[0]){
        if (JumpReg[0] && rs == EX_rd)  // data hazard
            return {0, 0, 0, 1, 0, 0};
        if (Branch[0] && (rs == EX_rd || rt == EX_rd))  // data hazard
            return {0, 0, 0, 1, 0, 0};
    }
    if (Jump[0] || JumpReg[0] || branch_taken[0])   // branch hazard
        return {1, 0, 1, 0, 1, 1};
    return {1, 1, 0, 0, 1, 1};
}

FORWARD_OUT ForwardingUnit(bitset<1> MEM_RegWrite, bitset<1> WB_RegWrite, bitset<5> EX_rs, bitset<5> EX_rt, bitset<5> MEM_rd, bitset<5> WB_rd){
    bitset<2> ForwardA;
    bitset<2> ForwardB;
    if (WB_RegWrite[0]){
        if(EX_rs == WB_rd)    ForwardA = 1;
        if(EX_rt == WB_rd)    ForwardB = 1;
    }
    if (MEM_RegWrite[0]){
        if (EX_rs == MEM_rd)  ForwardA = 2;
        if (EX_rt == MEM_rd)  ForwardB = 2;
    }
    return {ForwardA, ForwardB};
}