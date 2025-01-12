#include "../incs/main.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iostream>
using namespace std;

Register reg = Register();
InstructionMemory iMem = InstructionMemory();
DataMemory dMem = DataMemory();
unsigned int pc = 0;

unsigned int subBit(unsigned int num, int start, int end){
    unsigned int mask = (1 << (start - end + 1)) - 1;
    return (num>>end) & mask;
}

string toHexString(unsigned int num) {
    ostringstream oss;
    oss << "0x"
        << setfill('0') << setw(8) 
        << uppercase << hex << num;
    return oss.str();
}
void print(){
    cout << "pc : " << toHexString(pc) << endl;
}
void nonpipeline(){
    unsigned int inst = iMem.work(pc);
    unsigned int nextPc = adder(pc, 4);
    
    CONTROL_OUT control_sig =  Control(subBit(inst, 31, 26));
    unsigned int write_reg = mux(subBit(inst, 20, 16), subBit(inst, 15, 11), control_sig.RegDst);
    REG_OUT reg_data = reg.read(subBit(inst, 25, 21), 
        subBit(inst, 20, 16),
        write_reg
    );
    unsigned int constant = sign_ext(subBit(inst, 15, 0));
    unsigned int BTA = adder(nextPc, (constant<<2));
    unsigned char ALU_control_data = ALU_control(subBit(inst, 5, 0), control_sig.ALUOp1, control_sig.ALUOp0);
    ALU_OUT ALU_data = ALU(reg_data.data1, mux(reg_data.data2, constant, control_sig.ALUSrc), ALU_control_data);
    pc = mux(nextPc, BTA, (control_sig.Branch & ALU_data.zero));
    unsigned int read_data = dMem.work(ALU_data.result, reg_data.data2, control_sig.MemWrite, control_sig.MemRead);
    unsigned int write_data = mux(ALU_data.result, read_data, control_sig.MemtoReg);
    reg.write(write_reg, write_data, control_sig.RegWrite);

}
int main(int argc, char **argv){
    char input = '\n';
    while(input != 'q'){
        nonpipeline();
        print();
        cin.get(input);
    }
    
}