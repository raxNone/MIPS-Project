#include "../incs/circuit.hpp"

bitset<1> not_gate(const bitset<1>& A) {
    if (A)
        return FALSE;
    else
        return TRUE;
}

bitset<32> not_gate_32(const bitset<32>& A) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = not_gate(A[i]);
    }
    return result;
}

bitset<1> and_gate(const bitset<1>& A, const bitset<1>& B) {
    if (A[0] && B[0])
        return TRUE;
    else
        return FALSE;
}

bitset<5> and_gate_5(const bitset<5>& A, const bitset<5>& B) {
    bitset<5> result;
    for (int i = 0; i < 5; i++) {
        result[i] = and_gate(A[i], B[i]);
    }
    return result;
}

bitset<32> and_gate_32(const bitset<32>& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = and_gate(A[i], B[i]);
    }
    return result;
}

bitset<5> and_gate_1and5(const bitset<1>& A, const bitset<5>& B) {
    bitset<5> result;
    for (int i = 0; i < 5; i++) {
        result[i] = and_gate(A, B[i]);
    }
    return result;
}

bitset<32> and_gate_1and32(const bitset<1>& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = and_gate(A, B[i]);
    }
    return result;
}

bitset<1> and_gate3(bitset<1> A, bitset<1> B, bitset<1> C) {
    return and_gate(and_gate(A, B), C);
}

bitset<1> and_gate4(bitset<1> A, bitset<1> B, bitset<1> C, bitset<1> D) {
    return and_gate(and_gate(A, B), and_gate(C, D));
}

bitset<1> and_gate6(bitset<1> A, bitset<1> B, bitset<1> C, bitset<1> D, bitset<1> E, bitset<1> F) {
    return and_gate(and_gate3(A, B, C), and_gate3(D, E, F));
}

bitset<1> and_gate8(bitset<1> A, bitset<1> B, bitset<1> C, bitset<1> D, bitset<1> E, bitset<1> F, bitset<1> G, bitset<1> H) {
    return and_gate(and_gate4(A, B, C, D), and_gate4(E, F, G, H));
}

bitset<1> or_gate(const bitset<1>& A, const bitset<1>& B) {
    if (A[0] || B[0])
        return TRUE;
    else
        return FALSE;
}

bitset<5> or_gate_5(const bitset<5>& A, const bitset<5>& B) {
    bitset<5> result;
    for (int i = 0; i < 5; i++) {
        result[i] = or_gate(A[i], B[i]);
    }
    return result;
}

bitset<32> or_gate_32(const bitset<1>& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = or_gate(A[i], B[i]);
    }
    return result;
}

bitset<1> or_gate3(bitset<1> A, bitset<1> B, bitset<1> C) {
    return or_gate(A, or_gate(B, C));
}

bitset<32> or_gate3_32(bitset<32> A, bitset<32> B, bitset<32> C) {
    return or_gate_32(A, or_gate_32(B, C));
}

bitset<32> or_gate6_32(bitset<32> A, bitset<32> B, bitset<32> C, bitset<32> D, bitset<32> E, bitset<32> F) {
    return or_gate_32(or_gate3_32(A, B, C), or_gate3_32(D, E, F));
}

bitset<1> xor_gate(const bitset<1>& A, const bitset<1>& B) {
    if (A[0] ^ B[0])
        return TRUE;
    else
        return FALSE;
}

bitset<5> xor_gate_5(const bitset<5>& A, const bitset<5>& B) {
    bitset<5> result;
    for (int i = 0; i < 5; i++) {
        result[i] = xor_gate(A[i], B[i]);
    }
    return result;
}

bitset<32> xor_gate_32(const bitset<1>& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = xor_gate(A[i], B[i]);
    }
    return result;
}

bitset<1> EQUAL_32(const bitset<32>& A, const bitset<32>& B) {
    if (A == B)
        return TRUE;
    else
        return FALSE;
}

bool check_5bit_zero(const bitset<5>& A) {
    bool result = TRUE;
    for (int i = 0; i < 5; i++) {
        result = and_gate(result, not_gate(A[i]));
    }
    return result;
}

bitset<1> fullAdder(bitset<1> A, bitset<1> B, bitset<1> Cin, bitset<1>& Cout) {
    bitset<1> sum = xor_gate(xor_gate(A, B), Cin);  // XOR을 이용한 덧셈
    Cout = or_gate(and_gate(A, B), and_gate(Cin, xor_gate(A, B)));  // Carry 계산
    return sum;
}

bitset<32> sub_32(bitset<32> A, bitset<32> B) {
    bitset<32> result;
    bitset<32> NOT_B; // B의 NOT 연산
    bitset<1> carry = 1;  // 2의 보수 계산을 위한 +1 (초기 캐리 설정)

    // 32비트 덧셈기 수행 (A + (-B))
    for (int i = 0; i < 32; i++) {
        NOT_B[i] = not_gate(B[i]);
        result[i] = fullAdder(A[i], NOT_B[i], carry, carry);
    }

    return result;
}

bitset<1> slt_32(bitset<32> A, bitset<32> B) {
    bitset<32> result = sub_32(A, B);  // A - B 수행
    return result[31]; // 부호 비트(Sign Bit) 반환 (1이면 A < B)
}

bitset<28> shiftLeft2_26(const bitset<26>& i) {
    bitset<28> result;

    // 복사: 하위 26비트 그대로 복사
    for (int bit = 0; bit < 26; ++bit) {
        result[bit] = i[bit];
    }

    // 부호 확장: 최상위 비트(MSB)를 상위 2비트에 복사
    bool sign = i[25]; // 26비트의 MSB
    for (int bit = 26; bit < 28; ++bit) {
        result[bit] = sign;
    }

    return result;
}

bitset<32> shiftLeft2_32(const bitset<32>& i) {
    bitset<32> result;

    // 각 비트를 왼쪽으로 2칸 이동
    for (int j = 31; j >= 2; --j) {
        result[j] = i[j - 2]; // 이전 비트를 현재 위치로 이동
    }

    // 새로 채워지는 비트는 0 (논리 쉬프트)
    result[0] = 0;
    result[1] = 0;

    return result;
}

// bitset<1> or_gate6(bitset<32> A, bitset<32> B, bitset<32> C, bitset<32> D, bitset<32> E, bitset<32> F) {
//     return or_gate(or_gate3(A, B, C), or_gate3(D, E, F));
// }
// bitset<1> and_gate(bitset<32> A, bitset<32> B) {
//     if (A && B)
//         return TRUE;
//     else
//         return FALSE;
// }


bitset<32> adder_32(const bitset<32>& i1, const bitset<32>& i2) {
    bitset<32> sum;
    bool carry = 0;

    for (int i = 0; i < 32; ++i) {
        // 각 비트에 대해 full adder 수행
        bool a = i1[i];
        bool b = i2[i];

        // sum[i] = a XOR b XOR carry
        sum[i] = xor_gate(xor_gate(a, b), carry);

        // carry = (a AND b) OR (carry AND (a XOR b))
        carry = or_gate(and_gate(a, b), and_gate(carry, xor_gate(a, b)));
    }

    return sum;
}

bitset<1> mux_2to1(const bitset<1>& sel, const bitset<1>& a, const bitset<1>& b) {
    return or_gate(and_gate(sel, a), and_gate(not_gate(sel), b));
}

bitset<32> mux_32_2to1_01(const bitset<32>& i1, const bitset<32>& i2, const bitset<1>& s) {
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // MUX 구현: result[i] = (s & i1[i]) | (!s & i2[i])
        bool and1 = and_gate(s, i1[i]);   // AND 게이트 (s가 1일 때 i1 선택)
        bool and2 = and_gate(not_gate(s), i2[i]);       // AND 게이트 (s가 0일 때 i2 선택)
        result[i] = or_gate(and1, and2);     // OR 게이트
    }

    return result;
}

bitset<32> mux_32_2to1_10(const bitset<32>& i1, const bitset<32>& i2, const bitset<1>& s) {
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // MUX 구현: result[i] = (!s & i1[i]) | (s & i2[i])
        bool and1 = and_gate(not_gate(s), i1[i]);   // AND 게이트 (s가 0일 때 i1 선택)
        bool and2 = and_gate(s, i2[i]);       // AND 게이트 (s가 1일 때 i2 선택)
        result[i] = or_gate(and1, and2);     // OR 게이트
    }

    return result;
}

bitset<5> mux_5_3to1_021(const bitset<5>& i1, const bitset<5>& i2, const bitset<5>& i3, bitset<2> s) {
    bitset<5> result;

    for (int i = 0; i < 5; ++i) {
        // 선택 신호 처리
        bool select_i1 = and_gate(not_gate(s[1]), not_gate(s[0])); // s == 00 (0)
        bool select_i2 = and_gate(s[1], not_gate(s[0]));  // s == 10 (2)
        bool select_i3 = and_gate(not_gate(s[1]), s[0]);  // s == 01 (1)

        // 각 입력을 선택 신호에 따라 결합
        result[i] = or_gate3(and_gate(select_i1, i1[i]), and_gate(select_i2, i2[i]), and_gate(select_i3, i3[i]));
    }

    return result;
}



bitset<32> mux_32_3to1_102(const bitset<32>& i1, const bitset<32>& i2, const bitset<32>& i3, const bitset<2>& s) {
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // 선택 신호 처리
        bool select_i1 = and_gate(not_gate(s[1]), s[0]); // s == 01 (1)
        bool select_i2 = and_gate(not_gate(s[1]), not_gate(s[0]));  // s == 00 (0)
        bool select_i3 = and_gate(s[1], not_gate(s[0]));  // s == 10 (2)

        // 각 입력을 선택 신호에 따라 결합
        result[i] = or_gate3(and_gate(select_i1, i1[i]), and_gate(select_i2, i2[i]), and_gate(select_i3, i3[i]));
    }

    return result;
}


CONTROL_OUT mux_control_01(const CONTROL_OUT& i1, const CONTROL_OUT& i2, const bitset<1>& s) {
    CONTROL_OUT result;

    result.RegDst = or_gate(and_gate(not_gate(s), i2.RegDst), and_gate(s, i1.RegDst)); //s가 0 -> i2 선택, s가 1 -> i1 선택
    result.ALUSrc = or_gate(and_gate(not_gate(s), i2.ALUSrc), and_gate(s, i1.ALUSrc));
    result.MemtoReg = or_gate(and_gate(not_gate(s), i2.MemtoReg), and_gate(s, i1.MemtoReg));
    result.RegWrite = or_gate(and_gate(not_gate(s), i2.RegWrite), and_gate(s, i1.RegWrite));
    result.MemRead = or_gate(and_gate(not_gate(s), i2.MemRead), and_gate(s, i1.MemRead));
    result.MemWrite = or_gate(and_gate(not_gate(s), i2.MemWrite), and_gate(s, i1.MemWrite));
    result.Branch = or_gate(and_gate(not_gate(s), i2.Branch), and_gate(s, i1.Branch));
    result.Jump = or_gate(and_gate(not_gate(s), i2.Jump), and_gate(s, i1.Jump));
    result.JumpReg = or_gate(and_gate(not_gate(s), i2.JumpReg), and_gate(s, i1.JumpReg));
    result.ALUOp = or_gate(and_gate(not_gate(s), i2.ALUOp), and_gate(s, i1.ALUOp));

    return result;
}

bitset<32> signExt(const bitset<16>& i) {
    bitset<32> result;

    // 복사: 하위 16비트 그대로 복사
    for (int bit = 0; bit < 16; ++bit) {
        result[bit] = i[bit];
    }

    // 부호 확장: 최상위 비트(MSB)를 상위 16비트에 복사
    bool sign = i[15]; // 16비트의 MSB
    for (int bit = 16; bit < 32; ++bit) {
        result[bit] = sign;
    }

    return result;
}

ALU_OUT ALU(const bitset<32>& i1, const bitset<32>& i2, const bitset<4>& op) {
    /*
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
     */

    bitset<32> result = { 0, };

    bool is0000 = and_gate4(not_gate(op[3]), not_gate(op[2]), not_gate(op[1]), not_gate(op[0])); // 0000
    bool is0001 = and_gate4(not_gate(op[3]), not_gate(op[2]), not_gate(op[1]), op[0]); // 0001
    bool is0010 = and_gate4(not_gate(op[3]), not_gate(op[2]), op[1], not_gate(op[0])); // 0010
    bool is0110 = and_gate4(not_gate(op[3]), op[2], op[1], not_gate(op[0])); // 0110
    bool is0111 = and_gate4(not_gate(op[3]), op[2], op[1], op[0]); // 0111
    bool is1100 = and_gate4(op[3], op[2], not_gate(op[1]), not_gate(op[0])); // 1100

    bitset<32> AND_AB = and_gate_32(A, B);
    bitset<32> OR_AB = or_gate_32(A, B);
    bitset<32> add_AB = adder_32(A, B);
    bitset<32> sub_AB = sub_32(A, B);
    bitset<32> slt_AB = slt_32(A, B);
    bitset<32> NOR_AB = not_gate_32(or_gate_32(A, B));

    result = or_gate6_32(and_gate_1and32(is0000, AND_AB), and_gate_1and32(is0001, OR_AB), and_gate_1and32(is0010, add_AB),
        and_gate_1and32(is0110, sub_AB), and_gate_1and32(is0111, slt_AB), and_gate_1and32(is1100, NOR_AB));

    return { 0,result };
}
CONTROL_OUT Control(const bitset<6>& opcode, const bitset<5>& rs, const bitset<5>& rd) {
    CONTROL_OUT result;
    /*
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
    */

    bool is_rs_zero = check_5bit_zero(rs);
    bool is_rd_zero = check_5bit_zero(rd);

    bool R_type = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), not_gate(opcode[1]), not_gate(opcode[0])); // 000000
    bool jr = and_gate8(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), not_gate(opcode[1]), not_gate(opcode[0]), not_gate(is_rd_zero), is_rs_zero); // 000000 , rd==0, rs!=0
    bool j = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), opcode[1], not_gate(opcode[0])); // 000010
    bool jal = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), opcode[1], opcode[0]); // 000011
    bool beq = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), opcode[2], not_gate(opcode[1]), not_gate(opcode[0])); // 000100
    bool lw = and_gate6(opcode[5], not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), opcode[1], opcode[0]); // 100011
    bool sw = and_gate6(opcode[5], not_gate(opcode[4]), opcode[3], not_gate(opcode[2]), opcode[1], opcode[0]); // 101011
    bool addi = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), opcode[3], not_gate(opcode[2]), not_gate(opcode[1]), not_gate(opcode[0])); // 001000

    result.RegDst[1] = jal;
    result.RegDst[0] = R_type;
    result.ALUSrc = or_gate3(lw, sw, addi);
    result.MemToReg[1] = jal;
    result.MemToReg[0] = lw;
    result.RegWrite = or_gate(or_gate(and_gate(R_type, not_gate(jr)), jal), or_gate(lw, addi));
    result.MemRead = lw;
    result.MemWrite = sw;
    result.Branch = beq;
    result.Jump = or_gate(j, jal);
    result.JMPReg = and_gate(R_type, jr);
    result.ALUOp[1] = R_type;
    result.ALUOp[0] = beq;

}
bitset<4> ALUControl(const bitset<2>& ALUOp, const bitset<6>& funct) {
    bitset<4> result;
    /*
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
    */

    result[0] = and_gate(or_gate(funct[0], funct[3]), ALUOp[1]);
    result[1] = or_gate(not_gate(ALUOp[1]), not_gate(funct[2]));
    result[2] = or_gate(and_gate(ALUOp[1], funct[1]), ALUOp[0]);
    result[3] = and_gate(ALUOp[0], not_gate(ALUOp[0]));

    return result;
}

HAZARD_OUT HazardDetectionUnit(const bitset<5>& rs, const bitset<5>& rt, const bitset<1>& Jump, const bitset<1>& JumpReg, const bitset<1>& Branch, const bitset<5>& EX_rd, const bitset<1>& EX_MemRead, const bitset<1>& branch_taken) {
    /*
    if (EX_MemRead[0]){
        if (JumpReg[0] && rs == EX_rd)  // data hazard
            return {0, 0, 0, 1, 0, 0}; // result.ID_FLUSH = 1;
        if (Branch[0] && (rs == EX_rd || rt == EX_rd))  // data hazard
            return {0, 0, 0, 1, 0, 0}; // result.ID_FLUSH = 1;
    }
    if (Jump[0] || JumpReg[0] || branch_taken[0])   // branch hazard
        return {1, 0, 1, 0, 1, 1}; // result.(PCWrite, IF_FLUSH, stall, IDEXWrite) = 1;
    return {1, 1, 0, 0, 1, 1}; // result.(PCWrite, IFIDWrite, stall, IDEXWrite) = 1;
    */
    HAZARD_OUT result;
    // rs, rt, Ex_rd -> 5bit
    bool is_dataHazard; // EX_MemRead[0]!=0, (JumpReg[0]!=0, rs == Ex_rd) or (Branch[0] != 0 , rs == Ex_rd or rt == Ex_rd)
    bool is_branchHazard; // Jump[0] != 0 or JumpReg != 0 or branch_taken[0] != 0

    is_dataHazard = and_gate_1and5(Ex_MemRead[0], or_gate_5(and_gate_1and5(JumpReg[0], and_gate_5(rs, EX_rd), and_gate_1and5(Branch[0], or_gate_5(and_gate_5(rs, Ex_rd), and_gate_5(rt, Ex_rd))))));
    is_branchHazard = or_gate3(Jump[0], JumpReg, branch_taken[0]);

    result.PCWrite = or_gate(is_branchHazard, not_gate(is_dataHazard));
    result.IFIDWrite = not_gate(is_dataHazard);
    result.IF_FLUSH = is_branchHazard;
    result.ID_FLUSH = is_dataHazard;
    result.stall = or_gate(is_branchHazard, is_dataHazard);
    result.IDEXWrite = or_gate(is_branchHazard, not_gate(is_dataHazard));

    return result;
}

FORWARD_OUT ForwardingUnit(bitset<1> MEM_RegWrite, bitset<1> WB_RegWrite, bitset<5> EX_rs, bitset<5> EX_rt, bitset<5> MEM_rd, bitset<5> WB_rd) {
    FORWARD_OUT result;
    /*
    if (WB_RegWrite[0]) {
        if (EX_rs == WB_rd)    ForwardA = 1;
        if (EX_rt == WB_rd)    ForwardB = 1;
    }
    if (MEM_RegWrite[0]) {
        if (EX_rs == MEM_rd)  ForwardA = 2;
        if (EX_rt == MEM_rd)  ForwardB = 2;
    }
    */
    // EX_rs == WB_rd 확인 (WB Forwarding)
    bitset<1> EXrs_eq_WBrd = check_5bit_zero(xor_gate_5(EX_rs, WB_rd));
    bitset<1> EXrt_eq_WBrd = check_5bit_zero(xor_gate_5(EX_rt, WB_rd));

    // EX_rs == MEM_rd 확인 (MEM Forwarding)
    bitset<1> EXrs_eq_MEMrd = check_5bit_zero(xor_gate_5(EX_rs, MEM_rd));
    bitset<1> EXrt_eq_MEMrd = check_5bit_zero(xor_gate_5(EX_rt, MEM_rd));

    // ForwardA 결정 (MUX 활용)
    result.ForwardA[0] = mux_2to1(WB_RegWrite, EXrs_eq_WBrd, EXrs_eq_MEMrd);
    result.ForwardA[1] = and_gate(MEM_RegWrite, EXrs_eq_MEMrd);

    // ForwardB 결정 (MUX 활용)
    result.ForwardB[0] = mux_2to1(WB_RegWrite, EXrt_eq_WBrd, EXrt_eq_MEMrd);
    result.ForwardB[1] = and_gate(MEM_RegWrite, EXrt_eq_MEMrd);


    return result;
}