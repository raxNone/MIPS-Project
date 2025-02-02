#include "../incs/main.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>

#include <thread>
#include <memory>
#include <mutex>

using namespace std;

// mutex mtx_reg;      // for register I/O

// declare thread
shared_ptr<thread> t_hazard;
shared_ptr<thread> t_control;
shared_ptr<thread> t_forward;

shared_ptr<thread> t_register_read;
shared_ptr<thread> t_register_write;

shared_ptr<thread> t_calc_BTA;
shared_ptr<thread> t_calc_wb_data;

// PC
    bitset<32> pc = globl.addr;

// Memory
    InstructionMemory iMem = InstructionMemory();
    DataMemory dMem = DataMemory();

// Register
    Register reg = Register();

// Pipeline Register
    IFID_reg IFID = IFID_reg();
    IDEX_reg IDEX = IDEX_reg();
    EXMEM_reg EXMEM = EXMEM_reg();
    MEMWB_reg MEMWB = MEMWB_reg();

// Signal
    // Control ID Signal
    bitset<1> Branch;
    bitset<1> Jump;
    bitset<1> JumpReg;

    // ALUcontrol Signal
    bitset<4> operation;

    // ALU
    bitset<2> ALUOp;

    // Forwarding Signal
    bitset<2> ForwardA;
    bitset<2> ForwardB;

    // Hazard Signal
    bitset<1> IFIDWrite;
    bitset<1> PCWrite;
    bitset<1> stall;


// Assemble + Linking
    LABEL_TABLE globl;
    unordered_map<string, unsigned int> labelMap;
    unordered_map<string, unsigned int> section;  // [text, rodata, data, bss, heap, stack]

// temp
    bitset<32> beq_addr;
    bitset<32> j_addr;
    bitset<32> jr_addr;
    
    const CONTROL_OUT control_zero;
    bitset<1> reg_equal;

    bitset<32> wb_data;


// void IF(thread& t_IF){
//     bitset<32> next_pc;
//     bitset<32> instruction;
//     thread t_iMem_read([&]{        
//         instruction = iMem.read(pc);
//     });

//     thread t_calc_pc([&]{
//         next_pc = adder_32(pc, 4);

//         joinThread(*t_control);
//         joinThread(*t_calc_BTA);
//         // joinThread(*t_register_read); // joined at t_calc_BTA
//         bitset<32> tmp = mux_32_2to1_0(next_pc, beq_addr, AND_1(Branch,reg_equal));
//         bitset<32> tmp = mux_32_2to1_1(tmp, j_addr, Jump);
//         bitset<32> tmp = mux_32_2to1_0(tmp, jr_addr, JumpReg);
        
//         joinThread(*t_hazard);
//         pc = tmp;

//     });

//     t_IF = thread([]{
//         joinThread(t_calc_pc);
//         joinThread(t_iMem_read);
//         IFID.setReg(next_pc, instruction);
//     });
// }

// void ID(thread& t_ID){
//     CONTROL_OUT control_result;
//     REG_OUT reg_result;
//     bitset<32> constant;

//     t_control = make_shared<thread>([&]{    
//         Branch = control_result.Branch;
//         Jump = control_result.Jump;
//         JumpReg = control_result.JumpReg;
        
//     });
//     t_hazard = make_shared<thread>([&]{
//         HAZARD_OUT hazard_result =  HazardDetectionUnit(IDEX.signal_mem.MemRead, IDEX.signal_mem.Branch, EXMEM.signal_mem.Branch, subBit(IFID.instruction, 25, 21), subBit(IFID.instruction, 20, 16), IDEX.rt);
//         stall = hazard_result.ControlMUX;
//         IFIDWrite = hazard_result.IFIDWrite;
//         PCWrite = hazard_result.PCWrite;

//     });
//     t_register_read = make_shared<thread>([&]{
//         joinThread(*t_control);
//         mtx.lock();
//         reg_result = reg.read(subBit(IFID.instruction,25,21), subBit(IFID.instruction,20,16));
//         mtx.unlock();
//         reg_equal = EQUAL_32(reg_result.read_data1, reg_result.read_data2);

//     });

//     t_calc_BTA = make_shared<thread>([&]{
//         constant = signExt(subBit(IFID.instruction, 15, 0));
//         beq_addr = shiftLeft2_32(constant);
//         j_addr = (subBit(IFID.next_pc, 31, 26) <<26) | bitset<32>(shiftLeft2_26(subBit(IFID.instruction, 25, 0)).to_ullong());

//         joinThread(*t_register_read);
//         jr_addr = reg_result.read_data1;
//     });

//     t_ID = thread([]{
//         control_result = mux_control_0(control_zero, control_result, stall);
//         IDEX.setSig(control_result);
//         IDEX.rs_data = reg_result.read_data1;
//         IDEX.rt_data = reg_result.read_data2;
//         IDEX.constant = constant;
//         IDEX.next_pc = IFID.next_pc;
//         IDEX.rs = subBit(IFID.instruction, 25, 21);
//         IDEX.rt = subBit(IFID.instruction, 20, 16);
//         IDEX.rd = subBit(IFID.instruction, 15, 11);

//     });
// }
// void EX(thread& t_EX){
//     bitset<32> rs_data;
//     bitset<32> rt_data;
//     bitset<32> ALU_result;

//     t_forward = make_shared<thread>([]{
//         FORWARD_OUT forward_result = ForwardingUnit(EXMEM.signal_wb.RegWrite, MEMWB.signal_wb.RegWrite, IDEX.rs, IDEX.rt, EXMEM.write_reg, MEMWB.write_reg);
//         ForwardA = forward_result.ForwardA;
//         ForwardB = forward_result.ForwardB;
//     });
    
//     thread t_ALU([&]{
//         joinThread(*t_calc_wb_data);
//         joinThread(*t_forward);
//         rs_data = mux_32_3to1_1(wb_data, IDEX.rs_data, EXMEM.ALU_result, ForwardA);
//         rt_data = mux_32_3to1_1(wb_data, IDEX.rt_data, EXMEM.ALU_result, ForwardB);
//     });

//     t_EX = thread([]{
//         EXMEM.rt_data = rt_data;
//         EXMEM.ALU_result = ALU_result;
//         EXMEM.next_pc = IDEX.next_pc;
//         EXMEM.write_reg = mux_5_3to1_0(IDEX.rt, bitset<5>("11111"), IDEX.rd, IDEX.signal_wb.RegDst);
//     });

// }
// void MEM(thread& t_MEM){
//     t_MEM = thread([]{
//         joinThread(*t_control);
//         MEMWB.mem_data = dMem.work(EXMEM.ALU_result, EXMEM.rt_data, EXMEM.signal_mem.MemRead, EXMEM.signal_mem.MemWrite);
//         MEMWB.ALU_result = EXMEM.ALU_result;
//         MEMWB.next_pc = EXMEM.next_pc;
//     });
    
// }
// void WB(thread& t_WB){
//     t_calc_wb_data = make_shared<thread>([]{
//         joinThread(*t_control);
//         wb_data = mux_32_3to1_1(MEMWB.mem_data, MEMWB.ALU_result, MEMWB.next_pc, MEMWB.signal_wb.MemtoReg);
//     });
//     t_register_write = make_shared<thread>([]{
//         // joinThread(*t_control);  //joined at t_calc_wb_data
//         joinThread(*t_calc_wb_data);
//         reg.write(WB.write_reg, wb_data, MEMWB.signal_wb.RegWrite);
//     });
    
//     t_WB = thread([]{
//         joinThread(*t_register_write);
//     });
// }

// void work(){
//     thread t_IF;
//     thread t_ID;
//     thread t_EX;
//     thread t_MEM;
//     thread t_WB;
    
//     ID(t_ID);
//     IF(t_IF);
//     WB(t_WB);
//     EX(t_EX);
//     MEM(t_MEM);
    
    
//     joinThread(t_IF);
//     joinThread(t_ID);
//     joinThread(t_EX);
//     joinThread(t_MEM);
//     joinThread(t_WB);

// }


void print(){
    cout << globl.name << ": " << globl.addr<<endl;
    cout << iMem.read(globl.addr) << endl;
}

int main(int argc, char **argv){
    char input = '\n';
    assemble();
    print();
    // while(input != 'q'){
    //     setSig();
    
    //     print();
    //     cin.get(input);
    // }
    
}