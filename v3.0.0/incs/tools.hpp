#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__
#include <bitset>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <iomanip>
#include <iostream>
using namespace std;

extern mutex mtx;
extern unordered_map<thread*, shared_ptr<mutex>> thread_mutex_map;

uint32_t subBit(const bitset<32>& bs, int high, int low);

void printHex(const bitset<32>& bs);

void joinThread(thread& t);

#endif