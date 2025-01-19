#ifndef __MEM_HPP__
#define __MEM_HPP__
#include <vector>
#include <bitset>
#include <string>
#include <fstream>
#include "circuit.hpp"

using namespace std;

typedef struct _MEMORY{
    bitset<32> addr;
    bitset<32> value;
} MEMORY;

class InstructionMemory{
private:
    vector<MEMORY> mem;
public:
    bitset<32> read(bitset<32> addr);
    bool load(ifstream& inFile);
};

class DataMemory{
private:
    vector<MEMORY> mem;
public:
    bitset<32> work(bitset<32> address, bitset<32> writeData, bitset<1> MemRead, bitset<1> MemWrite);
};

#endif