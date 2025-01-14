#include "../incs/pipeline.hpp"

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

IF_OUT _IF::work(){
    if (PCWrite)
        pc = mux2to1(adder(pc,4), BTA, branch_sig);
    if (Flush)
        return {adder(pc, 4), 0, 1};
    return {adder(pc, 4), iMem.work(pc), 0};
}

void _ID::upload(IF_OUT IFID){
    if (IFIDWrite){
        nextPc = IFID.nextPc;
        instruction = IFID.instruction;
    }
    rs = subBit(instruction, 25, 21);
    rt = subBit(instruction, 20, 16);
    rd = subBit(instruction, 15, 11);
    constant = sign_ext(subBit(instruction, 15, 0));

    return;
}

void _ID::setSig(){
    control_sig = Control(subBit(instruction, 31, 26));
    Hazard_result = Hazard_detection_unit(EX.control_mem.MemRead, rt, rs, EX.rt);
    
    IF.PCWrite = Hazard_result.IFIDWrite;
    IF.BTA = adder(sign_ext(constant)<<2, nextPc);
    IFIDWrite = Hazard_result.IFIDWrite;

    return;
}

ID_OUT _ID::work(){
    reg_data = reg.read(rs, rt);
    IF.Flush = (reg_data.data1 == reg_data.data2) & control_sig.Branch;

    if (Hazard_result.ControlMUX)   return {
        {control_sig.RegWrite, control_sig.MemtoReg},
        {control_sig.Branch, control_sig.MemRead, control_sig.MemWrite},
        {control_sig.RegDst, control_sig.ALUOp1, control_sig.ALUOp0, control_sig.ALUSrc},
        reg_data.data1, reg_data.data2, constant, rs, rt, rd
    };
    return {{0,0}, {0,0,0}, {0,0,0,0}, 
        reg_data.data1, reg_data.data2, constant, rs, rt, rd
    };
    
}

void _EX::upload(ID_OUT IDEX){
    control_wb = IDEX.control_wb;
    control_mem = IDEX.control_mem;
    control_ex = IDEX.control_ex;
    rs_data = IDEX.rs_data;
    rt_data = IDEX.rt_data;
    constant = IDEX.constant;
    rs = IDEX.rs;
    rt = IDEX.rt;
    rd = IDEX.rd;
    
    return;
}

void _EX::setSig(){
    Forward_result =  Forwarding_unit(MEM.control_mem.MemWrite, EX.rs, EX.rt, MEM.write_reg, WB.write_reg);
    ForwardA = Forward_result.ForwardA;
    ForwardB = Forward_result.ForwardB;
    
    ALU_control_sig = ALU_control(subBit(constant, 5,0), control_ex.ALUOp1, control_ex.ALUOp0);
    return ;
}

EX_OUT _EX::work(){
    unsigned int i1 = mux3to1(rs_data, MEM.ALU_result, WB.write_data, ForwardA);
    unsigned int i2 = mux3to1(rt_data, MEM.ALU_result, WB.write_data, ForwardB);
    return {control_wb, control_mem, 
        ALU(i1, mux2to1(i2, constant, control_ex.ALUSrc), ALU_control_sig).result,
        i2, mux2to1(rt, rd, control_ex.RegDst)
    };
}

void _MEM::upload(EX_OUT EXMEM){
    control_wb = EXMEM.control_wb;
    control_mem = EXMEM.control_mem;
    ALU_result = EXMEM.ALU_result;
    write_data = EXMEM.write_data;
    write_reg = EXMEM.write_reg;
    return;
}

MEM_OUT _MEM::work(){
    return {
        control_wb,
        dMem.work(ALU_result, write_data, control_mem.MemWrite, control_mem.MemRead),
        ALU_result, write_reg
    };
}

void _WB::upload(MEM_OUT MEMWB){
    control_wb = MEMWB.control_wb;
    read_data = MEMWB.read_data;
    not_read_data = MEMWB.not_read_data;
    write_reg = MEMWB.write_reg;
    write_data = mux2to1(not_read_data, read_data, control_wb.MemtoReg);
    return;
}

void _WB::work(){
    reg.write(write_reg, write_data, control_wb.RegWrite);
    return;
}