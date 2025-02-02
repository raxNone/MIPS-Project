#ifndef __ASSEMBLE_HPP__
#define __ASSEMBLE_HPP__
#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "memory.hpp"

#define BYTE 1
#define WORD 4


using namespace std;

extern InstructionMemory iMem;


struct LABEL_TABLE{
    string name;
    unsigned int addr;
};

extern LABEL_TABLE globl;
extern unordered_map<string, unsigned int> labelMap;
extern unordered_map<string, unsigned int> section;  // [text, rodata, data, bss, heap, stack]

// Mapping MIPS Instruction
inline unordered_map<string, string> opcodes = {
    // R-format
    {"add", "000000"}, {"sub", "000000"}, {"and", "000000"}, {"or", "000000"},
    {"jr", "000000"}, {"sll", "000000"}, {"srl", "000000"},
    // J-format
    {"j", "000010"}, {"jal", "000011"},
    // I-format
    {"addi", "001000"}, {"ori", "001101"},  {"lw", "100011"}, {"sw", "101011"}, {"beq", "000100"}, {"bne", "000101"},
    {"slti", "001010"}, {"lui", "001111"}, 
    
};

inline unordered_map<string, string> functCodes = {
    {"add", "100000"}, {"sub", "100010"}, {"and", "100100"}, {"or", "100101"}
};

inline unordered_map<string, string> registerMap = {
    {"", "00000"},
    {"$zero", "00000"}, {"$at", "00001"}, {"$v0", "00010"}, {"$v1", "00011"},
    {"$a0", "00100"}, {"$a1", "00101"}, {"$a2", "00110"}, {"$a3", "00111"},
    {"$t0", "01000"}, {"$t1", "01001"}, {"$t2", "01010"}, {"$t3", "01011"},
    {"$t4", "01100"}, {"$t5", "01101"}, {"$t6", "01110"}, {"$t7", "01111"},
    {"$s0", "10000"}, {"$s1", "10001"}, {"$s2", "10010"}, {"$s3", "10011"},
    {"$s4", "10100"}, {"$s5", "10101"}, {"$s6", "10110"}, {"$s7", "10111"},
    {"$t8", "11000"}, {"$t9", "11001"}, {"$k0", "11010"}, {"$k1", "11011"},
    {"$gp", "11100"}, {"$sp", "11101"}, {"$fp", "11110"}, {"$ra", "11111"}
};


void assemble();
unsigned int stoui(const string& token);


#endif