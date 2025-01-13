#include "../incs/circuit.hpp"

unsigned int adder(unsigned int i1, unsigned int i2){
    return i1 + i2;
}
unsigned int mux(unsigned int i1, unsigned int i2, bool s){
    if(s)   return i2;
    return i1;
}
unsigned int sign_ext(int i){
    return i;
}
ALU_OUT ALU(unsigned int i1, unsigned int i2, unsigned char op){
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
CONTROL_OUT Control(unsigned char opcode){
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
unsigned char ALU_control(unsigned char funct , bool op1, bool op0){
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