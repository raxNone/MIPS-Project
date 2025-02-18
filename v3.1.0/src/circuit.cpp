#include "../incs/circuit.hpp"

bool not_gate(const bool& A) {
    return ( !A );
}

bitset<32> not_gate_32(const bitset<32>& A) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = not_gate(A[i]);
    }
    return result;
}

bool and_gate(const bool& A, const bool& B) {
    return ( A && B );
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

bitset<5> and_gate_1and5(const bool& A, const bitset<5>& B) {
    bitset<5> result;
    for (int i = 0; i < 5; i++) {
        result[i] = and_gate(A, B[i]);
    }
    return result;
}

bitset<32> and_gate_1and32(const bool& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = and_gate(A, B[i]);
    }
    return result;
}

bool and_gate3(const bool& A, const bool& B, const bool& C) {
    return and_gate(and_gate(A, B), C);
}

bool and_gate4(const bool& A, const bool& B, const bool& C, const bool& D) {
    return and_gate(and_gate(A, B), and_gate(C, D));
}

bool and_gate6(const bool& A, const bool& B, const bool& C, const bool& D, const bool& E, const bool& F) {
    return and_gate(and_gate3(A, B, C), and_gate3(D, E, F));
}

bool and_gate8(const bool& A, const bool& B, const bool& C, const bool& D, const bool& E, const bool& F, const bool& G, const bool& H) {
    return and_gate(and_gate4(A, B, C, D), and_gate4(E, F, G, H));
}

bool or_gate(const bool& A, const bool& B) {
    return ( A || B ) ;
}

bitset<5> or_gate_5(const bitset<5>& A, const bitset<5>& B) {
    bitset<5> result;
    for (int i = 0; i < 5; i++) {
        result[i] = or_gate(A[i], B[i]);
    }
    return result;
}

bitset<32> or_gate_32(const bitset<32>& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = or_gate(A[i], B[i]);
    }
    return result;
}

bool or_gate3(const bool& A, const bool& B, const bool& C) {
    return or_gate(A, or_gate(B, C));
}

bitset<32> or_gate3_32(const bitset<32> A, const bitset<32>& B, const bitset<32>& C) {
    return or_gate_32(A, or_gate_32(B, C));
}

bitset<32> or_gate6_32(const bitset<32>& A, const bitset<32>& B, const bitset<32>& C, const bitset<32>& D, const bitset<32>& E, const bitset<32>& F) {
    return or_gate_32(or_gate3_32(A, B, C), or_gate3_32(D, E, F));
}

bool xor_gate(const bool& A, const bool& B) {
    return (A ^ B);
}

bitset<5> xor_gate_5(const bitset<5>& A, const bitset<5>& B) {
    bitset<5> result;
    for (int i = 0; i < 5; i++) {
        result[i] = xor_gate(A[i], B[i]);
    }
    return result;
}

bitset<32> xor_gate_32(const bitset<32>& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result[i] = xor_gate(A[i], B[i]);
    }
    return result;
}

bool EQUAL_32(const bitset<32>& A, const bitset<32>& B) {
    bitset<32> result;
    for (int i = 0; i<32; i++){
        result[i] = not_gate(xor_gate(A[i], B[i]));
    }
    return result.all();    // all bits are 1
}

bool check_5bit_zero(const bitset<5>& A) {
    bool result = true;
    for (int i = 0; i < 5; i++) {
        result = and_gate(result, not_gate(A[i]));
    }
    return result;
}

bool fullAdder(const bool& A, const bool& B, const bool& Cin, bool& Cout) {
    bool sum = xor_gate(xor_gate(A, B), Cin);  // XOR
    Cout = or_gate(and_gate(A, B), and_gate(Cin, xor_gate(A, B)));  // Carry 
    return sum;
}

bitset<32> sub_32(const bitset<32>& A, const bitset<32>& B) {
    bitset<32> result;
    bitset<32> NOT_B;
    bool carry = 1;

    for (int i = 0; i < 32; i++) {
        NOT_B[i] = not_gate(B[i]);
        result[i] = fullAdder(A[i], NOT_B[i], carry, carry);
    }

    return result;
}

bool slt_32(const bitset<32>& A, const bitset<32>& B) {
    bitset<32> result = sub_32(A, B);  // A - B
    return result[31]; // MSB of (A-B)
}

bitset<28> shiftLeft2_26(const bitset<26>& i) {
    bitset<28> result;

    // ����: ���� 26��Ʈ �״�� ����
    for (int bit = 0; bit < 26; ++bit) {
        result[bit] = i[bit];
    }

    // ��ȣ Ȯ��: �ֻ��� ��Ʈ(MSB)�� ���� 2��Ʈ�� ����
    bool sign = i[25]; // 26��Ʈ�� MSB
    for (int bit = 26; bit < 28; ++bit) {
        result[bit] = sign;
    }

    return result;
}

bitset<32> shiftLeft2_32(const bitset<32>& i) {
    bitset<32> result;

    // �� ��Ʈ�� �������� 2ĭ �̵�
    for (int j = 31; j >= 2; --j) {
        result[j] = i[j - 2]; // ���� ��Ʈ�� ���� ��ġ�� �̵�
    }

    // ���� ä������ ��Ʈ�� 0 (���� ����Ʈ)
    result[0] = 0;
    result[1] = 0;

    return result;
}

// bool or_gate6(bitset<32> A, bitset<32> B, bitset<32> C, bitset<32> D, bitset<32> E, bitset<32> F) {
//     return or_gate(or_gate3(A, B, C), or_gate3(D, E, F));
// }
// bool and_gate(bitset<32> A, bitset<32> B) {
//     if (A && B)
//         return TRUE;
//     else
//         return FALSE;
// }


bitset<32> adder_32(const bitset<32>& i1, const bitset<32>& i2) {
    bitset<32> sum;
    bool carry = 0;

    for (int i = 0; i < 32; ++i) {
        // �� ��Ʈ�� ���� full adder ����
        bool a = i1[i];
        bool b = i2[i];

        // sum[i] = a XOR b XOR carry
        sum[i] = xor_gate(xor_gate(a, b), carry);

        // carry = (a AND b) OR (carry AND (a XOR b))
        carry = or_gate(and_gate(a, b), and_gate(carry, xor_gate(a, b)));
    }

    return sum;
}

bool mux_2to1(const bool& sel, const bool& a, const bool& b) {
    return or_gate(and_gate(sel, a), and_gate(not_gate(sel), b));
}

bitset<32> mux_32_2to1_10(const bitset<32>& i1, const bitset<32>& i2, const bitset<1>& s) {
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // MUX ����: result[i] = (s & i1[i]) | (!s & i2[i])
        bool and1 = and_gate(s[0], i1[i]);   // AND ����Ʈ (s�� 1�� �� i1 ����)
        bool and2 = and_gate(not_gate(s[0]), i2[i]);       // AND ����Ʈ (s�� 0�� �� i2 ����)
        result[i] = or_gate(and1, and2);     // OR ����Ʈ
    }

    return result;
}

bitset<32> mux_32_2to1_01(const bitset<32>& i1, const bitset<32>& i2, const bitset<1>& s) {
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // MUX ����: result[i] = (!s & i1[i]) | (s & i2[i])
        bool and1 = and_gate(not_gate(s[0]), i1[i]);   // AND ����Ʈ (s�� 0�� �� i1 ����)
        bool and2 = and_gate(s[0], i2[i]);       // AND ����Ʈ (s�� 1�� �� i2 ����)
        result[i] = or_gate(and1, and2);     // OR ����Ʈ
    }

    return result;
}

bitset<5> mux_5_3to1_021(const bitset<5>& i1, const bitset<5>& i2, const bitset<5>& i3, bitset<2> s) {
    bitset<5> result;

    for (int i = 0; i < 5; ++i) {
        // ���� ��ȣ ó��
        bool select_i1 = and_gate(not_gate(s[1]), not_gate(s[0])); // s == 00 (0)
        bool select_i2 = and_gate(s[1], not_gate(s[0]));  // s == 10 (2)
        bool select_i3 = and_gate(not_gate(s[1]), s[0]);  // s == 01 (1)

        // �� �Է��� ���� ��ȣ�� ���� ����
        result[i] = or_gate3(and_gate(select_i1, i1[i]), and_gate(select_i2, i2[i]), and_gate(select_i3, i3[i]));
    }

    return result;
}



bitset<32> mux_32_3to1_102(const bitset<32>& i1, const bitset<32>& i2, const bitset<32>& i3, const bitset<2>& s) {
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // ���� ��ȣ ó��
        bool select_i1 = and_gate(not_gate(s[1]), s[0]); // s == 01 (1)
        bool select_i2 = and_gate(not_gate(s[1]), not_gate(s[0]));  // s == 00 (0)
        bool select_i3 = and_gate(s[1], not_gate(s[0]));  // s == 10 (2)

        // �� �Է��� ���� ��ȣ�� ���� ����
        result[i] = or_gate3(and_gate(select_i1, i1[i]), and_gate(select_i2, i2[i]), and_gate(select_i3, i3[i]));
    }

    return result;
}


CONTROL_OUT mux_control_10(const CONTROL_OUT& i1, const CONTROL_OUT& i2, const bitset<1>& sig) {
    CONTROL_OUT result;
    bool s = sig[0];
    result.RegDst[1] = or_gate(and_gate(not_gate(s), i2.RegDst[1]), and_gate(s, i1.RegDst[1])); //s�� 0 -> i2 ����, s�� 1 -> i1 ����
    result.RegDst[0] = or_gate(and_gate(not_gate(s), i2.RegDst[0]), and_gate(s, i1.RegDst[0]));
    result.ALUSrc[0] = or_gate(and_gate(not_gate(s), i2.ALUSrc[0]), and_gate(s, i1.ALUSrc[0]));
    result.MemtoReg[1] = or_gate(and_gate(not_gate(s), i2.MemtoReg[1]), and_gate(s, i1.MemtoReg[1]));
    result.MemtoReg[0] = or_gate(and_gate(not_gate(s), i2.MemtoReg[0]), and_gate(s, i1.MemtoReg[0]));
    result.RegWrite[0] = or_gate(and_gate(not_gate(s), i2.RegWrite[0]), and_gate(s, i1.RegWrite[0]));
    result.MemRead[0] = or_gate(and_gate(not_gate(s), i2.MemRead[0]), and_gate(s, i1.MemRead[0]));
    result.MemWrite[0] = or_gate(and_gate(not_gate(s), i2.MemWrite[0]), and_gate(s, i1.MemWrite[0]));
    result.Branch[0] = or_gate(and_gate(not_gate(s), i2.Branch[0]), and_gate(s, i1.Branch[0]));
    result.Jump[0] = or_gate(and_gate(not_gate(s), i2.Jump[0]), and_gate(s, i1.Jump[0]));
    result.JumpReg[0] = or_gate(and_gate(not_gate(s), i2.JumpReg[0]), and_gate(s, i1.JumpReg[0]));
    result.ALUOp[1] = or_gate(and_gate(not_gate(s), i2.ALUOp[1]), and_gate(s, i1.ALUOp[1]));
    result.ALUOp[0] = or_gate(and_gate(not_gate(s), i2.ALUOp[0]), and_gate(s, i1.ALUOp[0]));

    return result;
}

bitset<32> signExt(const bitset<16>& i) {
    bitset<32> result;

    // ����: ���� 16��Ʈ �״�� ����
    for (int bit = 0; bit < 16; ++bit) {
        result[bit] = i[bit];
    }

    // ��ȣ Ȯ��: �ֻ��� ��Ʈ(MSB)�� ���� 16��Ʈ�� ����
    bool sign = i[15]; // 16��Ʈ�� MSB
    for (int bit = 16; bit < 32; ++bit) {
        result[bit] = sign;
    }

    return result;
}

ALU_OUT ALU(const bitset<32>& A, const bitset<32>& B, const bitset<4>& op) {
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
    bool jr = and_gate8(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), not_gate(opcode[1]), not_gate(opcode[0]), (is_rd_zero), not_gate(is_rs_zero)); // 000000 , rd==0, rs!=0
    bool nop = and_gate8(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), not_gate(opcode[1]), not_gate(opcode[0]), (is_rd_zero), (is_rs_zero)); // 000000 , rd==0, r=!=0
    bool j = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), opcode[1], not_gate(opcode[0])); // 000010
    bool jal = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), opcode[1], opcode[0]); // 000011
    bool beq = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), not_gate(opcode[3]), opcode[2], not_gate(opcode[1]), not_gate(opcode[0])); // 000100
    bool lw = and_gate6(opcode[5], not_gate(opcode[4]), not_gate(opcode[3]), not_gate(opcode[2]), opcode[1], opcode[0]); // 100011
    bool sw = and_gate6(opcode[5], not_gate(opcode[4]), opcode[3], not_gate(opcode[2]), opcode[1], opcode[0]); // 101011
    bool addi = and_gate6(not_gate(opcode[5]), not_gate(opcode[4]), opcode[3], not_gate(opcode[2]), not_gate(opcode[1]), not_gate(opcode[0])); // 001000

    result.RegDst[1] = jal;
    result.RegDst[0] = and_gate(R_type, not_gate(nop));
    result.ALUSrc = or_gate3(lw, sw, addi);
    result.MemtoReg[1] = jal;
    result.MemtoReg[0] = lw;
    result.RegWrite[0] = or_gate(or_gate(and_gate(R_type, not_gate(or_gate(jr, nop))), jal), or_gate(lw, addi));
    result.MemRead[0] = lw;
    result.MemWrite[0] = sw;
    result.Branch[0] = beq;
    result.Jump[0] = or_gate(j, jal);
    result.JumpReg[0] = and_gate3(R_type, jr, not_gate(nop));
    result.ALUOp[1] = and_gate(R_type, not_gate(nop));
    result.ALUOp[0] = beq;
    
    return result;
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

    is_dataHazard = and_gate_1and5(
        EX_MemRead[0], 
        or_gate_5(
            and_gate_1and5(
                JumpReg[0], and_gate_5(rs, EX_rd)
            ), 
            and_gate_1and5(
                Branch[0], or_gate_5(and_gate_5(rs, EX_rd), and_gate_5(rt, EX_rd))
            )
        )).all();
    is_branchHazard = or_gate3(Jump[0], JumpReg[0], branch_taken[0]);

    result.PCWrite[0] = or_gate(is_branchHazard, not_gate(is_dataHazard));
    result.IFIDWrite[0] = not_gate(is_dataHazard);
    result.IF_FLUSH[0] = is_branchHazard;
    result.ID_FLUSH[0] = is_dataHazard;
    result.stall[0] = or_gate(is_branchHazard, is_dataHazard);
    result.IDEXWrite[0] = or_gate(is_branchHazard, not_gate(is_dataHazard));

    return result;
}

FORWARD_OUT ForwardingUnit(const bitset<1>& MEM_RegWrite, const bitset<1>& WB_RegWrite, const bitset<5>& EX_rs, const bitset<5>& EX_rt, const bitset<5>& MEM_rd, const bitset<5>& WB_rd) {
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
    // EX_rs == WB_rd Ȯ�� (WB Forwarding)
    bool EXrs_eq_WBrd = check_5bit_zero(xor_gate_5(EX_rs, WB_rd));
    bool EXrt_eq_WBrd = check_5bit_zero(xor_gate_5(EX_rt, WB_rd));

    // EX_rs == MEM_rd Ȯ�� (MEM Forwarding)
    bool EXrs_eq_MEMrd = check_5bit_zero(xor_gate_5(EX_rs, MEM_rd));
    bool EXrt_eq_MEMrd = check_5bit_zero(xor_gate_5(EX_rt, MEM_rd));

    // ForwardA ���� (MUX Ȱ��)
    result.ForwardA[1] = and_gate(MEM_RegWrite[0], EXrs_eq_MEMrd);
    result.ForwardA[0] = and_gate(not_gate(result.ForwardA[1]), and_gate(WB_RegWrite[0], EXrs_eq_WBrd));

    // ForwardB ���� (MUX Ȱ��)
    result.ForwardB[1] = and_gate(MEM_RegWrite[0], EXrt_eq_MEMrd);
    result.ForwardB[0] = and_gate(not_gate(result.ForwardB[1]), and_gate(WB_RegWrite[0], EXrt_eq_WBrd));


    return result;
}