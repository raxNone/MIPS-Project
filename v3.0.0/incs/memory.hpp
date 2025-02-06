#ifndef __MEM_HPP__
#define __MEM_HPP__
#include <vector>
#include <bitset>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>
#include <iostream>
#include "circuit.hpp"
using namespace std;

#define BYTE 1
#define HALF 2
#define WORD 4
#define DOUBLE 8
#define PAGE_SIZE 16
#define BLOCK_SIZE 4

using namespace std;



class Memory{
public:
    Memory(const string &file_name);
    ~Memory();
protected:
    fstream mem;
    uint64_t mem_size = 0;
    map<uint32_t, uint32_t> VtoP; // {VA, PA}
    map<uint32_t, uint32_t> PtoV; // {PA, VA}
    // string buffer;
    bool valid_page(const uint32_t& page_addr);
    bitset<32> read(const bitset<32>& addr);           // 4byte read
    void write_BYTE(const bitset<32>& addr, const uint8_t& data);
    void write_HALF(const bitset<32>& addr, const uint16_t& data);
    void write_WORD(const bitset<32>& addr, const uint32_t& data);
    void write_DOUBLE(const bitset<32>& addr, const uint64_t& data);
};

class InstructionMemory : protected Memory{
public:
    InstructionMemory(const string& file_name);    // 생성자 선언
    bitset<32> work(const bitset<32>& addr); // only read
    void load(const bitset<32>& addr, const uint32_t& data); // 4byte load
};

class DataMemory : protected Memory{
public:
    DataMemory(const string& file_name);    //생성자 선언
    bitset<32> work(const bitset<32>& addr, const bitset<32>& writeData, const bitset<1>& MemRead, const bitset<1>& MemWrite);  // read write
    
    void load(const bitset<32>& addr, const uint8_t data);
    void load(const bitset<32>& addr, const uint16_t data);
    void load(const bitset<32>& addr, const uint32_t data);
    void load(const bitset<32>& addr, const uint64_t data);
    void load(const bitset<32>& addr, const string data);
        
};




#endif