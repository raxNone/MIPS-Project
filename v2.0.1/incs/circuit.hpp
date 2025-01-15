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
    unsigned int ForwardA;
    unsigned int ForwardB;
} FORWARD_OUT;

unsigned int adder(unsigned int i1, unsigned int i2);
unsigned int mux2to1(unsigned int i1, unsigned int i2, bool s);
unsigned int mux3to1(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int s);
unsigned int sign_ext(int i);

ALU_OUT ALU(unsigned int i1, unsigned int i2, unsigned int op);
CONTROL_OUT Control(unsigned int opcode);
unsigned int ALU_control(unsigned int funct, bool op1, bool op0);

HAZARD_OUT Hazard_detection_unit(bool MemRead, unsigned int IDrt, unsigned int IDrs, unsigned int EXrt);
FORWARD_OUT Forwarding_unit(bool MEMRegWrite, bool WBRegWrite, unsigned int EXrs, unsigned int EXrt, unsigned int MEMrd, unsigned int WBrd);

#endif