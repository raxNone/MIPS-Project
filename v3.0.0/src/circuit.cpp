#include "../incs/circuit.hpp"

bitset<1> not_gate(bitset<32> A) {
    if (A)
        return FALSE;
    else
        return TRUE;
}

bitset<1> or_gate(bitset<32> A, bitset<32> B) {
    if (A || B)
        return TRUE;
    else
        return FALSE;
}

bitset<1> or_gate3(bitset<32> A, bitset<32> B, bitset<32> C) {
    return or_gate_1(A, or_gate_1(B, C));
}

bitset<1> or_gate6(bitset<32> A, bitset<32> B, bitset<32> C, bitset<32> D, bitset<32> E, bitset<32> F) {
    return or_gate(or_gate3(A, B, C), or_gate3(D, E, F));
}

bitset<1> and_gate(bitset<32> A, bitset<32> B) {
    if (A && B)
        return TRUE;
    else
        return FALSE;
}

bitset<1> and_gate3(bitset<32> A, bitset<32> B, bitset<32> C) {
    return and_gate(A, and_gate(B, C));
}

bitset<1> and_gate4(bitset<32> A, bitset<32> B, bitset<32> C, bitset<32> D) {
    return and_gate3(A, B, and_gate(C, D));
}

bitset<1> and_gate6(bitset<32> A, bitset<32> B, bitset<32> C, bitset<32> D, bitset<32> E, bitset<32> F) {
    return and_gate(and_gate3(A, B, C), and_gate3(D, E, F));
}

bitset<1> xor_gate(bitset<32> A, bitset<32> B) {
    if (A ^ B)
        return TRUE;
    else
        return FALSE;
}

bitset<1> nor_gate(bitset<32> A, bitset<32> B) {
    return not_gate(or_gate(A, B));
}

bitset<1> nand_gate(bitset<32> A, bitset<32> B) {
    return not_gate(and_gate(A, B));
}

bitset<32> adder_32(bitset<32> i1, bitset<32> i2) {
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

bitset<5> mux_5_2to1_0(bitset<5> i1, bitset<5> i2, bitset<1> s) { //i1:0, i2:1
    bitset<5> result;

    for (int i = 0; i < 5; ++i) {
        // MUX 구현: result[i] = (!s & i1[i]) | (s & i2[i])
        bool and1 = and_gate(not_gate(s), i1[i]);   // AND 게이트 (s가 0일 때 i1 선택)
        bool and2 = and_gate(s, i2[i]);       // AND 게이트 (s가 1일 때 i2 선택)
        result[i] = or_gate(and1, and2);     // OR 게이트
    }

    return result;
}

bitset<32> mux_32_2to1_0(bitset<32> i1, bitset<32> i2, bitset<1> s) { // i1:0, i2:1
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // MUX 구현: result[i] = (!s & i1[i]) | (s & i2[i])
        bool and1 = and_gate(not_gate(s), i1[i]);   // AND 게이트 (s가 0일 때 i1 선택)
        bool and2 = and_gate(s, i2[i]);       // AND 게이트 (s가 1일 때 i2 선택)
        result[i] = or_gate(and1, and2);     // OR 게이트
    }

    return result;
}

bitset<32> mux_32_2to1_1(bitset<32> i1, bitset<32> i2, bitset<1> s) { // i1:1, i2:0
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // MUX 구현: result[i] = (s & i1[i]) | (!s & i2[i])
        bool and1 = and_gate(s, i1[i]);   // AND 게이트 (s가 1일 때 i1 선택)
        bool and2 = and_gate(not_gate(s), i2[i]);       // AND 게이트 (s가 0일 때 i2 선택)
        result[i] = or_gate(and1, and2);     // OR 게이트
    }

    return result;
}

bitset<5> mux_5_3to1_0(bitset<5> i1, bitset<5> i2, bitset<5> i3, bitset<2> s) { // i1:1, i2:0, i3:2
    bitset<5> result;

    for (int i = 0; i < 5; ++i) {
        // 선택 신호 처리
        bool select_i1 = and_gate(not_gate(s[1]), s[0]); // s == 01 (1)
        bool select_i2 = and_gate(not_gate(s[1]), not_gate(s[0]));  // s == 00 (0)
        bool select_i3 = and_gate(s[1], not_gate(s[0]));  // s == 10 (2)

        // 각 입력을 선택 신호에 따라 결합
        result[i] = or_gate3(and_gate(select_i1, i1[i]), and_gate(select_i2, i2[i]), and_gate(select_i3, i3[i]));
    }

    return result;
}

bitset<32> mux_32_3to1_1(bitset<32> i1, bitset<32> i2, bitset<32> i3, bitset<2> s) { // i1:0, i2:2, i3:1
    bitset<32> result;

    for (int i = 0; i < 32; ++i) {
        // 선택 신호 처리
        bool select_i1 = and_gate(not_gate(s[1]), not_gate(s[0])); // s == 00 (0)
        bool select_i2 = and_gate(s[1], not_gate(s[0]));  // s == 10 (2)
        bool select_i3 = and_gate(not_gate(s[1]), s[0]);  // s == 01 (1)

        // 각 입력을 선택 신호에 따라 결합
        result[i] = or_gate3(and_gate(select_i1, i1[i]), and_gate(select_i2, i2[i]), and_gate(select_i3, i3[i]));
    }

    return result;
}

unsigned int adder(unsigned int i1, unsigned int i2) {
    return i1 + i2;
}

unsigned int mux2to1(unsigned int i1, unsigned int i2, bool s){ // i2(1), i1(0) 
    if(s)   return i2;
    return i1;
}
unsigned int mux3to1(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int s){
    if(s==0)    return i1;
    else if (s == 1) return i2;
    return i3;
}
unsigned int sign_ext(int i){
    return i;
}
ALU_OUT ALU(unsigned int i1, unsigned int i2, unsigned int op){
    switch(op%0xf){
        case 0b0000:    //AND
            return {0, i1&i2};
        case 0b0001:    //OR
            return {0, i1|i2};
        case 0b0010:    //add
            return {0, i1+i2};
        case 0b0110:    //sub
            return {(i1 == i2), i1-i2};
        case 0b0111:    //slt
            return {(i1==i2), i1<i2};
        case 0b1100:    //NOR
            return {0, ~(i1|i2)};
    }
    return {0,0};
}
CONTROL_OUT Control(unsigned int opcode){
    switch (opcode^0b111111){
        case 0b000000:  //R-format
            return {1,0,0,1,0,0,0,1,0};
        case 0b100011:  //lw
            return {0,1,1,1,1,0,0,0,0};
        case 0b101011:  //sw
            return {0,1,0,0,0,1,0,0,0};
        case 0b000100:  //beq
            return {0,0,0,0,0,0,1,0,1};
    }
    return {0,0,0,0,0,0,0,0,0};
}
unsigned int ALU_control(unsigned int funct , bool op1, bool op0){
    if (op1 == 1){  // R-type
        switch(funct^0b1111){
            case 0b0000:    return 0b0010;  //add
            case 0b0010:    return 0b0110;  //sub
            case 0b0100:    return 0b0000;  //AND
            case 0b0101:    return 0b0001;  //OR
            case 0b1010:    return 0b0111;  //slt
        }
    }
    else if (op0 == 1)  return 0b0110;  //sub
    return 0b0010;  //add
}

HAZARD_OUT Hazard_detection_unit(bool MemRead, unsigned int IDrt, unsigned int IDrs, unsigned int EXrt){
    if(MemRead && (EXrt == IDrs || EXrt == IDrt))
        return {0,0,0};
    return {1,1,1};
}

FORWARD_OUT Forwarding_unit(bool MEMRegWrite, bool WBRegWrite, unsigned int EXrs, unsigned int EXrt, unsigned int MEMrd, unsigned int WBrd){
    unsigned int ForwardA=0;
    unsigned int ForwardB=0;
    if (WBRegWrite){
        if(EXrs == WBrd)    ForwardA = 1;
        if(EXrt == WBrd)    ForwardB = 1;
    }
    if (MEMRegWrite){
        if (EXrs == MEMrd)  ForwardA = 2;
        if (EXrt == MEMrd)  ForwardB = 2;
    }
    return {ForwardA, ForwardB};
}