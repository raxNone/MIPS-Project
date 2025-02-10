#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__
#include <bitset>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <iomanip>
#include <iostream>
#include "crow.h"
#include "pipeline.hpp"
using namespace std;

extern mutex mtx;
extern unordered_map<thread*, shared_ptr<mutex>> thread_mutex_map;

uint32_t subBit(const bitset<32>& bs, int high, int low);

// string printHex(const bitset<32>& bs);

void joinThread(thread& t);

// bitset을 16진수로 변환하는 함수 선언
std::string printHex(std::bitset<32> value);
std::string printBin(std::bitset<1> value);
std::string printBin(std::bitset<2> value);
std::string printBin(std::bitset<5> value);

// JSON 변환 함수들 선언
void to_json(crow::json::wvalue& j, const EX_SIG& ex_sig);
void to_json(crow::json::wvalue& j, const MEM_SIG& mem_sig);
void to_json(crow::json::wvalue& j, const WB_SIG& wb_sig);
void to_json(crow::json::wvalue& j, const IFID_reg& ifid);
void to_json(crow::json::wvalue& j, const IDEX_reg& idex);
void to_json(crow::json::wvalue& j, const EXMEM_reg& exmem);
void to_json(crow::json::wvalue& j, const MEMWB_reg& memwb);

#endif