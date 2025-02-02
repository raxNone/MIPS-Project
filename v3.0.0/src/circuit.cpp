#include "../incs/circuit.hpp"

unsigned int adder(unsigned int i1, unsigned int i2){
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

HAZARD_OUT HazardDetectionUnit(bitset<1> EX_MemRead, bitset<1> EX_Branch, bitset<1> MEM_Branch, bitset<5> ID_rt, bitset<5> ID_rs, bitset<5> EX_rt){
    
}

FORWARD_OUT ForwardingUnit(bool MEMRegWrite, bool WBRegWrite, unsigned int EXrs, unsigned int EXrt, unsigned int MEMrd, unsigned int WBrd){
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