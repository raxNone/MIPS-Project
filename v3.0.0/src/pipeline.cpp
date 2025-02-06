#include "../incs/pipeline.hpp"

void IFID_reg::setReg(const _IFID &reg){
    if (this->IFIDWrite[0])
        this->reg = reg;
    if (this->flush[0])
        this->reg = {};
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