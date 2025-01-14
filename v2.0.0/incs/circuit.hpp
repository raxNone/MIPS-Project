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

typedef struct _HAZARD_OUT{
    bool PCWrite;
    bool IFIDWrite;
    bool ControlMUX;
} HAZARD_OUT;

typedef struct _FORWARD_OUT{
    unsigned char ForwardA;
    unsigned char ForwardB;
} FORWARD_OUT;

unsigned int adder(unsigned int i1, unsigned int i2);
unsigned int mux2to1(unsigned int i1, unsigned int i2, bool s);
unsigned int mux3to1(unsigned int i1, unsigned int i2, unsigned int i3, unsigned char s);
unsigned int sign_ext(int i);

ALU_OUT ALU(unsigned int i1, unsigned int i2, unsigned char op);
CONTROL_OUT Control(unsigned char opcode);
unsigned char ALU_control(unsigned char funct, bool op1, bool op0);

HAZARD_OUT Hazard_detection_unit(bool MemRead, unsigned char IDrt, unsigned char IDrs, unsigned char EXrt);
FORWARD_OUT Forwarding_unit(bool MEMRegWrite, unsigned char EXrs, unsigned char EXrt, unsigned char MEMrd, unsigned char WBrd);

#endif