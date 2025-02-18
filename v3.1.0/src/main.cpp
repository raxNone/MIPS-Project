#include "../incs/main.hpp"
#include "crow.h"
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include <thread>
#include <memory>
#include <mutex>


using namespace std;

const vector<u_char> ELF_MAGIC = {0x7F, 'E', 'L', 'F'};
const vector<u_char> PE_MAGIC = {'M', 'Z'};

mutex mtx;
unordered_map<thread*, shared_ptr<mutex>> thread_mutex_map;

// Assemble + Linking
    LABEL_TABLE globl;
    unordered_map<string, uint32_t> labelMap;
    unordered_map<string, uint32_t> section;  // [text, rodata, data, bss, heap, stack]
// PC
    bitset<32> pc;

// Memory
    InstructionMemory iMem = InstructionMemory("test/IMEM");
    DataMemory dMem = DataMemory("test/DMEM");

// Register
    Register reg = Register();

// Pipeline Register
    IFID_reg IFID = IFID_reg();
    IDEX_reg IDEX = IDEX_reg();
    EXMEM_reg EXMEM = EXMEM_reg();
    MEMWB_reg MEMWB = MEMWB_reg();

// Signal
    const CONTROL_OUT control_zero;
    

    
void run(bool clock){
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
            branch_taken = and_gate(control_result.Branch[0], xor_gate(reg_equal[0], opcode[0]));
        });
        thread_mutex_map.emplace(&t_branch, make_shared<mutex>());

        thread t_calc_pc([&]{
            next_pc = adder_32(pc, 4);
            
            joinThread(t_control);
            j_addr = (subBit(IFID.reg.next_pc, 31, 28) << 28) | (shiftLeft2_26(subBit(IFID.reg.instruction, 25, 0)).to_ullong()<<2);
            tmp = mux_32_2to1_01(next_pc, j_addr, control_result.Jump);
            instruction = iMem.work(pc);

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

        mux_control_10(control_result, control_zero, hazard_result.stall);
        thread_mutex_map.clear();

    }


    clock = !clock;

    if (!clock){
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
    clock = !clock;
}


int main(int argc, char **argv){
    string line;
    bool clock = 1;
    bool stop = false;

    crow::SimpleApp app;
    

    CROW_ROUTE(app, "/")
    ([](){
        std::ifstream t("p0tat019.github.io/index.html");  // HTML 파일 열기
        std::string html((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());  // HTML 파일 내용 읽기
        pc = 0;
        return html;  // HTML 파일 내용 반환
    });

    CROW_ROUTE(app, "/asset/index.css")
    ([](){
        std::ifstream t("p0tat019.github.io/asset/index.css");  // CSS 파일 열기
        std::string css((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());  // CSS 파일 내용 읽기
        return crow::response{css};  // CSS 파일 내용 반환
    });
    
    CROW_ROUTE(app, "/asset/index.js")
    ([](){
        std::ifstream t("p0tat019.github.io/asset/index.js");  // JS 파일 열기
        std::string js((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());  // JS 파일 내용 읽기
        return crow::response{js};  // JS 파일 내용 반환
    });    

    CROW_ROUTE(app,"/read_reg") // 모든 reg 전송. js에서 조회
    .methods("GET"_method)([](const crow::request& req){
        std::string data = req.url_params.get("data");
        crow::json::wvalue res;
        if(!isNumber(data)){
            data = "$" + data;
            for(int i=0; i<32; i++){
                if(reg_name[i] == data){
                    res["content"] = printHex(reg[i]);
                }
            }
        }else{
            res["content"] = printHex(reg[stoui(data)]);
        }
        
        return crow::response(200, res);
    });

    CROW_ROUTE(app,"/read_mem")
    .methods("GET"_method)([](const crow::request& req){
        std::string data = req.url_params.get("data");
        crow::json::wvalue res;
        res["content"] = printHex(dMem.work(bitset<32>(stoui(data)),0,1,0));
        return crow::response(200, res);
    });

    CROW_ROUTE(app, "/run")
    ([&](){
        stop = false;
        int count = 3;
        while(count-- && !stop){
            run(clock);
        }
        crow::json::wvalue res;

        // 레지스터들 출력
        to_json(res["MEMWB"], MEMWB);
        to_json(res["EXMEM"], EXMEM);
        to_json(res["IDEX"], IDEX);
        to_json(res["IFID"], IFID);
        res["pc"] = printHex(pc);
        // JSON 응답
        return crow::response(200, res);
    });

    CROW_ROUTE(app, "/stop")
    ([&](){
        stop = true;
        return crow::response(200);
    });

    CROW_ROUTE(app, "/next")
    ([&](){
        if (!clock)
            return crow::response(400,"processing");
        run(clock);

        crow::json::wvalue res;

        // 레지스터들 출력
        to_json(res["MEMWB"], MEMWB);
        to_json(res["EXMEM"], EXMEM);
        to_json(res["IDEX"], IDEX);
        to_json(res["IFID"], IFID);
        res["pc"] = printHex(pc);

        // JSON 응답
        return crow::response(200, res);

    });
    
    CROW_ROUTE(app, "/reset").methods("GET"_method)
    ([&](){
        if (!clock)
            return crow::response(400,"processing");
        globl = {"",nullptr};
        labelMap.clear();
        section.clear();
        iMem.reset();
        dMem.reset();
        reg.reset();
        
        IFID.resetData();
        IDEX.resetData();
        EXMEM.resetData();
        MEMWB.resetData();
        try{
            assemble();
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
        if (globl.name.empty())
            return crow::response(400,"Globl Not Found");
        pc = *globl.addr;
        crow::json::wvalue res;
        to_json(res["MEMWB"], MEMWB);
        to_json(res["EXMEM"], EXMEM);
        to_json(res["IDEX"], IDEX);
        to_json(res["IFID"], IFID);
        res["pc"] = printHex(pc);  // pc 값을 16진수로 변환하여 포함

        return crow::response(200, res);
    });
    
    
    CROW_ROUTE(app, "/upload").methods("POST"_method)
    ([](const crow::request& req) {
        std::string content_type = req.get_header_value("Content-Type");
        crow::json::wvalue response_json;

        if (content_type == "text/plain") {
            // 텍스트 파일 처리
            std::string file_content = req.body;
            std::ofstream t("test/test.s");  // ASM 파일 저장

            if (!t) {
                return crow::response(500, "Internal Server Error: Cannot open test/test.s");
            }

            t << file_content; // 텍스트 내용을 파일로 저장
            response_json["file_content"] = file_content;

        } else if (content_type.find("multipart/form-data") != std::string::npos) {
            // multipart/form-data 처리
            auto body = req.body;

            // boundary 추출
            size_t boundary_pos = content_type.find("boundary=");
            if (boundary_pos == std::string::npos) {
                return crow::response(400, "Boundary not found in Content-Type");
            }
            std::string boundary = "--" + content_type.substr(boundary_pos + 9);

            // boundary를 기준으로 파트들을 분리
            size_t part_start = body.find(boundary);
            size_t part_end = 0;
            size_t file_start, file_end;
            bool file_found = false;

            while (part_start != std::string::npos) {
                part_end = body.find(boundary, part_start + 1); // 다음 boundary 찾기

                // 파트가 파일인지 아닌지 확인
                file_start = body.find("\r\n\r\n", part_start); // 헤더 끝
                if (file_start != std::string::npos) {
                    file_start += 4;  // `\r\n\r\n` 다음부터 파일 데이터 시작

                    // 파일 데이터 끝 부분을 찾음
                    file_end = (part_end == std::string::npos) ? body.size() : part_end;

                    // 헤더 파싱 (파일 이름, 파일 유형 등)
                    size_t filename_pos = body.find("filename=\"", part_start);
                    if (filename_pos != std::string::npos) {
                        filename_pos += 10;
                        size_t filename_end = body.find("\"", filename_pos);
                        std::string filename = body.substr(filename_pos, filename_end - filename_pos);
                        std::cout << "Uploaded file: " << filename << std::endl;
                    }

                    std::string file_content = body.substr(file_start, file_end - file_start);

                    // 파일 저장
                    std::ofstream out("test/test.s", std::ios::binary);
                    if (!out) {
                        return crow::response(500, "Failed to open file for writing");
                    }
                    out.write(file_content.data(), file_content.size());

                    response_json["file_content"] = file_content;
                    file_found = true;
                }

                part_start = part_end;
            }

            if (!file_found) {
                return crow::response(400, "No file found in the multipart request");
            }

        } else {
            return crow::response(400, "Unsupported Content-Type");
        }

        return crow::response{response_json};
    });
    // app.port(8080).run();
    app.port(8080).multithreaded().run();
    
}