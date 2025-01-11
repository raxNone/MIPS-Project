#include "../incs/circuit.hpp"

unsigned int adder(unsigned int i1, unsigned int i2){
    return i1 + i2;
}
unsigned int mux(unsigned int i1, unsigned int i2, bool s){
    if(s)   return i1;
    return i2;
}
unsigned int sign_ext(int i){
    return i;
}
ALU_OUT ALU(unsigned int i1, unsigned int i2, unsigned char op){
    switch(op%0xf){
        case 0b0000:    //AND
            break;
        case 0b0001:    //OR
            break;
        case 0b0010:    //add
            break;
        case 0b0110:    //sub
            break;
        case 0b0111:    //slt
            break;
        case 0b1100:    //NOR
            break;
    }
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
    else    return 0b0010;  //add
}