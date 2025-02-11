#include "../incs/tools.hpp"

uint32_t subBit(const bitset<32>& bs, int high, int low) {
    return (static_cast<unsigned int>(bs.to_ulong()) >> low) & ((1U << (high - low + 1)) - 1);
}

void joinThread(thread& t){
    lock_guard<mutex> lock(*thread_mutex_map[&t]);
    if(t.joinable())
        t.join();
}


bool isNumber(const string& str) {
    for (char ch : str) {
        if (!isdigit(ch)) return false; // 숫자가 아닌 문자가 있으면 false
    }
    return !str.empty(); // 빈 문자열은 숫자가 아님
}


// bitset을 16진수로 변환하는 함수 구현 (자릿수를 8자리로 고정)
std::string printHex(std::bitset<32> value) {
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << std::hex << value.to_ulong() << std::dec;
    return ss.str();
}

// bitset을 2진수로 변환하는 함수 구현 (자릿수를 일정하게 맞추기)
std::string printBin(std::bitset<1> value) {
    std::stringstream ss;
    ss << std::setw(1) << std::setfill('0') << value.to_string();
    return ss.str();  // 1비트는 그대로 출력
}

std::string printBin(std::bitset<2> value) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << value.to_string();
    return ss.str();  // 2비트는 자릿수를 맞춰 출력
}

std::string printBin(std::bitset<5> value) {
    std::stringstream ss;
    ss << std::setw(5) << std::setfill('0') << value.to_string();
    return ss.str();  // 5비트는 자릿수를 맞춰 출력
}

// JSON 변환 함수들 구현
void to_json(crow::json::wvalue& j, const EX_SIG& ex_sig) {
    j["RegDst"] = printBin(ex_sig.RegDst);
    j["ALUSrc"] = printBin(ex_sig.ALUSrc);
    j["ALUOp"] = printBin(ex_sig.ALUOp);
}

void to_json(crow::json::wvalue& j, const MEM_SIG& mem_sig) {
    j["MemRead"] = printBin(mem_sig.MemRead);
    j["MemWrite"] = printBin(mem_sig.MemWrite);
    j["Branch"] = printBin(mem_sig.Branch);
    j["Jump"] = printBin(mem_sig.Jump);
    j["JumpReg"] = printBin(mem_sig.JumpReg);
}

void to_json(crow::json::wvalue& j, const WB_SIG& wb_sig) {
    j["MemtoReg"] = printBin(wb_sig.MemtoReg);
    j["RegWrite"] = printBin(wb_sig.RegWrite);
}

void to_json(crow::json::wvalue& j, const IFID_reg& ifid) {
    j["IFIDWrite"] = printBin(ifid.IFIDWrite);
    j["flush"] = printBin(ifid.flush);
    j["next_pc"] = printHex(ifid.reg.next_pc);
    j["instruction"] = printHex(ifid.reg.instruction);
}

void to_json(crow::json::wvalue& j, const IDEX_reg& idex) {
    j["IDEXWrite"] = printBin(idex.IDEXWrite);
    j["flush"] = printBin(idex.flush);
    to_json(j["signal_wb"], idex.reg.signal_wb);
    to_json(j["signal_mem"], idex.reg.signal_mem);
    to_json(j["signal_ex"], idex.reg.signal_ex);
    j["next_pc"] = printHex(idex.reg.next_pc);
    j["a"] = printHex(idex.reg.a);
    j["b"] = printHex(idex.reg.b);
    j["constant"] = printHex(idex.reg.constant);
    j["rs"] = printBin(idex.reg.rs);
    j["rt"] = printBin(idex.reg.rt);
    j["rd"] = printBin(idex.reg.rd);
}

void to_json(crow::json::wvalue& j, const EXMEM_reg& exmem) {
    to_json(j["signal_wb"], exmem.reg.signal_wb);
    to_json(j["signal_mem"], exmem.reg.signal_mem);
    j["next_pc"] = printHex(exmem.reg.next_pc);
    j["zero"] = printBin(exmem.reg.zero);
    j["ALUOut"] = printHex(exmem.reg.ALUOut);
    j["b"] = printHex(exmem.reg.b);
    j["rd"] = printBin(exmem.reg.rd);
}

void to_json(crow::json::wvalue& j, const MEMWB_reg& memwb) {
    to_json(j["signal_wb"], memwb.reg.signal_wb);
    j["next_pc"] = printHex(memwb.reg.next_pc);
    j["wb_data"] = printHex(memwb.reg.wb_data);
    j["rd"] = printBin(memwb.reg.rd);
}
