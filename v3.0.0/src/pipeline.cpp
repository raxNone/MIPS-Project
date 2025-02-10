#include "../incs/pipeline.hpp"

void IFID_reg::setReg(const _IFID &reg){
    if (this->IFIDWrite[0])
        this->reg = reg;
    if (this->flush[0])
        this->reg = {};
}
// IFID_reg 클래스의 resetData 함수 구현
void IFID_reg::resetData() {
    IFIDWrite = 1;
    flush = 0;
    reg.next_pc.reset();
    reg.instruction.reset();
}

// IDEX_reg 클래스의 resetData 함수 구현
void IDEX_reg::resetData() {
    IDEXWrite = 1;
    flush = 0;
    reg.signal_wb = {};  // Reset WB_SIG
    reg.signal_mem = {}; // Reset MEM_SIG
    reg.signal_ex = {};  // Reset EX_SIG
    reg.next_pc.reset();
    reg.a.reset();
    reg.b.reset();
    reg.constant.reset();
    reg.rs.reset();
    reg.rt.reset();
    reg.rd.reset();
}

// EXMEM_reg 클래스의 resetData 함수 구현
void EXMEM_reg::resetData() {
    reg.signal_wb = {};  // Reset WB_SIG
    reg.signal_mem = {}; // Reset MEM_SIG
    reg.next_pc.reset();
    reg.zero.reset();
    reg.ALUOut.reset();
    reg.b.reset();
    reg.rd.reset();
}

// MEMWB_reg 클래스의 resetData 함수 구현
void MEMWB_reg::resetData() {
    reg.signal_wb = {};  // Reset WB_SIG
    reg.next_pc.reset();
    reg.wb_data.reset();
    reg.rd.reset();
}


void IDEX_reg::setReg(const _IDEX &reg)
{
    if (this->IDEXWrite[0])
        this->reg = reg;
    if (this->flush[0])
        this->reg = {};
}

void EXMEM_reg::setReg(const _EXMEM& reg){
    this->reg = reg;
}

void MEMWB_reg::setReg(const _MEMWB& reg){
    this->reg = reg;
}