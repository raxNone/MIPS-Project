#include "../incs/pipeline.hpp"
void IFID_reg::setFlush(bitset<1> flush){
    if(flush[0]){
        instruction = 0;
    }
}

void IFID_reg::setReg(bitset<32> next_pc, bitset<32> instruction){
    if(IFIDWrite[0]){
        this->next_pc = next_pc;
        this->instruction = instruction;
    }
}

void IDEX_reg::setSig(CONTROL_OUT sig){
    signal_ex.RegDst = sig.RegDst;
    signal_ex.ALUSrc = sig.ALUSrc;
    signal_ex.ALUOp = sig.ALUOp;
    signal_mem.MemRead = sig.MemRead;
    signal_mem.MemWrite = sig.MemWrite;
    signal_wb.MemtoReg = sig.MemtoReg;
    signal_wb.RegWrite = sig.RegWrite;
}