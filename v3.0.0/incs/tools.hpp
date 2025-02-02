#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__
#include <bitset>
#include <thread>
using namespace std;


unsigned int subBit(const bitset<32>& bs, int high, int low);

void joinThread(thread& t);

#endif