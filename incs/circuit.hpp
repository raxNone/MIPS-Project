#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__

typedef struct _ALU_OUT{
    bool zero;
    unsigned int result;
} ALU_OUT;
typedef struct _CONTROL_OUT{
    bool RegDst;
    bool ALUSrc;
    bool MemtoReg;
    bool RegWrite;
    bool MemRead;
    bool MemWrite;
    bool Branch;
    bool ALUOp1;
    bool ALUOp0;
} CONTROL_OUT;

unsigned int adder(unsigned int i1, unsigned int i2);
unsigned int mux(unsigned int i1, unsigned int i2, bool s);
unsigned int sign_ext(int i);

ALU_OUT ALU(unsigned int i1, unsigned int i2, unsigned char op);
CONTROL_OUT Control(unsigned char opcode);
unsigned char ALU_control(unsigned char funct, bool op1, bool op0);

#endif