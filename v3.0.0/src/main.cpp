#include "../incs/main.hpp"
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include <thread>
#include <memory>
#include <mutex>

using namespace std;

mutex mtx;
unordered_map<thread*, shared_ptr<mutex>> thread_mutex_map;

// Assemble + Linking
    LABEL_TABLE globl;
    unordered_map<string, uint32_t> labelMap;
    unordered_map<string, uint32_t> section;  // [text, rodata, data, bss, heap, stack]
// PC
    bitset<32> pc;

// Memory
    InstructionMemory iMem = InstructionMemory("../../test/IMEM");
    DataMemory dMem = DataMemory("../../test/DMEM");

// Register
    Register reg = Register();

// Pipeline Register
    IFID_reg IFID = IFID_reg();
    IDEX_reg IDEX = IDEX_reg();
    EXMEM_reg EXMEM = EXMEM_reg();
    MEMWB_reg MEMWB = MEMWB_reg();

// Signal
    const CONTROL_OUT control_zero;
    CONTROL_OUT control_result;
    FORWARD_OUT forward_result;
    FORWARD_OUT bforward_result;
    HAZARD_OUT hazard_result;
    ALU_OUT ALU_result;


    bitset<4> ALUOp;    // Output of ALUControl

    bitset<1> reg_equal;
    bitset<1> branch_taken;

    // if
    bitset<32> next_pc;
    bitset<32> instruction;
    bitset<32> tmp;
    bitset<32> b_addr;
    bitset<32> j_addr;
    bitset<32> jr_addr;

    // id
    REG_OUT reg_result;
    bitset<32> a; // rs_data
    bitset<32> b; // rt_data
    bitset<6> opcode;
    bitset<5> rs;
    bitset<5> rt;
    bitset<5> rd;
    bitset<6> funct;
    bitset<32> constant;
    

    // ex
    bitset<5> write_reg;
    bitset<32> f_a;
    bitset<32> f_b;
    bitset<5> EX_rd;    // rt or rd (destination reg)
    
    // mem
    bitset<32> mem_data;
    bitset<32> wb_data;

    
void run(bool clock){
    if (clock){
        opcode = subBit(IFID.reg.instruction, 31, 26);
        rs = subBit(IFID.reg.instruction, 25, 21);
        rt = subBit(IFID.reg.instruction, 20, 16);
        rd = subBit(IFID.reg.instruction, 15, 11);
        funct = subBit(IFID.reg.instruction, 5, 0);
        constant = signExt(subBit(IFID.reg.instruction, 15, 0));

        thread t_control([&]{
            // id
            control_result = Control(opcode, rs, rd);
        });
        thread_mutex_map.emplace(&t_control, make_shared<mutex>());

        thread t_calc_rd([&]{
            // ex
            joinThread(t_control);
            EX_rd = mux_5_3to1_021(IDEX.reg.rt, bitset<5>("11111"), IDEX.reg.rd, IDEX.reg.signal_ex.RegDst);        
            
        });
        thread_mutex_map.emplace(&t_calc_rd, make_shared<mutex>());

        thread t_forward([&]{
            // ex
            forward_result = ForwardingUnit(EXMEM.reg.signal_wb.RegWrite, MEMWB.reg.signal_wb.RegWrite, IDEX.reg.rs, IDEX.reg.rt, EXMEM.reg.rd, MEMWB.reg.rd);
        });
        thread_mutex_map.emplace(&t_forward, make_shared<mutex>());

        thread t_bforward([&]{
            // id
            joinThread(t_calc_rd);
            bforward_result = ForwardingUnit(IDEX.reg.signal_wb.RegWrite, EXMEM.reg.signal_wb.RegWrite, rs, rt, EX_rd, EXMEM.reg.rd);
        });
        thread_mutex_map.emplace(&t_bforward, make_shared<mutex>());

        thread t_iMem([&]{
            // if
            instruction = iMem.work(pc);
        });
        thread_mutex_map.emplace(&t_iMem, make_shared<mutex>());

        thread t_dMem([&]{
            // mem
            joinThread(t_control);
            mem_data = dMem.work(EXMEM.reg.ALUOut, EXMEM.reg.b, EXMEM.reg.signal_mem.MemRead, EXMEM.reg.signal_mem.MemWrite);
        });
        thread_mutex_map.emplace(&t_dMem, make_shared<mutex>());

        thread t_calc_wb_data([&]{
            // mem
            joinThread(t_dMem);
            wb_data = mux_32_3to1_102(mem_data, EXMEM.reg.ALUOut, EXMEM.reg.next_pc, EXMEM.reg.signal_wb.MemtoReg);
        });
        thread_mutex_map.emplace(&t_calc_wb_data, make_shared<mutex>());

        thread t_register([&]{
            joinThread(t_control);
            joinThread(t_calc_wb_data);
            // write - wb
            reg.write(MEMWB.reg.rd, MEMWB.reg.wb_data, MEMWB.reg.signal_wb.RegWrite);
            // read - id
            reg_result = reg.read(rs, rt);
        });
        thread_mutex_map.emplace(&t_register, make_shared<mutex>());

        thread t_ALU([&]{
            joinThread(t_forward);
            joinThread(t_calc_rd);
            f_a = mux_32_3to1_102(MEMWB.reg.wb_data, IDEX.reg.a, EXMEM.reg.ALUOut, forward_result.ForwardA);
            f_b = mux_32_3to1_102(MEMWB.reg.wb_data, IDEX.reg.b, EXMEM.reg.ALUOut, forward_result.ForwardB);
            ALUOp = ALUControl(IDEX.reg.signal_ex.ALUOp, funct);
            ALU_result = ALU(f_a, mux_32_2to1_01(f_b, IDEX.reg.constant, IDEX.reg.signal_ex.ALUSrc), ALUOp);
        });
        thread_mutex_map.emplace(&t_ALU, make_shared<mutex>());

        thread t_branch([&]{
            joinThread(t_register);
            joinThread(t_calc_wb_data);
            joinThread(t_ALU);
            joinThread(t_bforward);
            
            a = mux_32_3to1_102(wb_data, reg_result.a, ALU_result.result, bforward_result.ForwardA);
            b = mux_32_3to1_102(wb_data, reg_result.b, ALU_result.result, bforward_result.ForwardB);
            reg_equal = EQUAL_32(a, b);
            branch_taken = and_gate(control_result.Branch, xor_gate(reg_equal, bitset<1>(opcode[0])));
        });
        thread_mutex_map.emplace(&t_branch, make_shared<mutex>());

        thread t_calc_pc([&]{
            next_pc = adder_32(pc, 4);
            
            joinThread(t_control);
            j_addr = (subBit(IFID.reg.next_pc, 31, 28) << 28) | (shiftLeft2_26(subBit(IFID.reg.instruction, 25, 0)).to_ullong());
            tmp = mux_32_2to1_01(next_pc, j_addr, control_result.Jump);

            joinThread(t_branch);
            b_addr = shiftLeft2_32(constant);
            tmp = mux_32_2to1_01(tmp, b_addr, branch_taken);

            jr_addr = a;
            tmp = mux_32_2to1_01(tmp, jr_addr, control_result.JumpReg);
        });
        thread_mutex_map.emplace(&t_calc_pc, make_shared<mutex>());
        
        thread t_hazard([&]{
            // id
            joinThread(t_branch);
            hazard_result =  HazardDetectionUnit(rs, rt, control_result.Jump, control_result.JumpReg, control_result.Branch, EX_rd, IDEX.reg.signal_mem.MemRead, branch_taken);
            
            IFID.flush = hazard_result.IF_FLUSH;
            IFID.IFIDWrite = hazard_result.IFIDWrite;
            IDEX.flush = hazard_result.ID_FLUSH;
            IDEX.IDEXWrite = hazard_result.IDEXWrite;
            
        });
        thread_mutex_map.emplace(&t_hazard, make_shared<mutex>());

        
        joinThread(t_control);
        joinThread(t_forward);
        joinThread(t_bforward);
        joinThread(t_iMem);
        joinThread(t_dMem);
        joinThread(t_calc_wb_data);
        joinThread(t_calc_rd);
        joinThread(t_register);
        joinThread(t_ALU);
        joinThread(t_branch);
        joinThread(t_calc_pc);
        joinThread(t_hazard);

        mux_control_01(control_zero, control_result, hazard_result.stall);
        thread_mutex_map.clear();

    }else{
        if (hazard_result.PCWrite[0])
            pc = tmp;
        MEMWB.setReg({
            EXMEM.reg.signal_wb,
            EXMEM.reg.next_pc,
            wb_data,
            EXMEM.reg.rd
        });
        EXMEM.setReg({
            IDEX.reg.signal_wb,
            IDEX.reg.signal_mem,
            IDEX.reg.next_pc,
            ALU_result.zero,
            ALU_result.result,
            f_b,
            EX_rd
        });    
        IDEX.setReg({
            {
                control_result.MemtoReg,
                control_result.RegWrite
            },
            {
                control_result.MemRead,
                control_result.MemWrite,
                control_result.Branch,
                control_result.Jump,
                control_result.JumpReg
            },
            {
                control_result.RegDst,
                control_result.ALUSrc,
                control_result.ALUOp
            },
            IFID.reg.next_pc,
            reg_result.a, reg_result.b,
            constant,
            rs, rt, rd
        });
        IFID.setReg({
            next_pc, instruction
        });
    }

}


void print(const string& line){
    cout << "pc : ";  printHex(pc);
    cout << "instruction : ";   printHex(instruction);
    cout << "t0 : ";    printHex(reg[$t0]);
    cout << "t1 : ";    printHex(reg[$t1]);

}

int main(int argc, char **argv){
    string line;
    bool clock = 1;
    // cout << "thread : " << thread::hardware_concurrency() << endl;
    assemble();
    cout << "[globl] "<< globl.name << ": " << *globl.addr<<endl;
    while(getline(cin, line)){
        run(clock);
        clock = !clock;
        run(clock);
        clock = !clock;
        print(line);
    }
    
}