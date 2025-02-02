#include "../incs/assemble.hpp"

void assemble()
{
    ifstream inFile("../../test/test.s");
    ofstream outFile("../../test/test");
    string line;
    string instruction;
    string section_name[6] = {".text", ".rodata", ".data", ".bss", ".heap", ".stack"};
    
    string token;

    // text segment
    string opcode;
    string rs, rt, rd;
    bitset<5> shamt;
    string funct;
    bitset<16> imm;
    bitset<26> address;
    unsigned int openParenPos;

    int sect = -1;
    unsigned int pos = 0;   // current addr

    if(!inFile.is_open() || !outFile.is_open()){
        cerr << "FILE ERROR" << endl;
        exit(-1);
    }

    while(getline(inFile, line)){
        istringstream iss(line);
        iss >> token;
        
        if (token == ".globl"){
            iss >> globl.name;
        }else if (token.back() == ':'){
            token.pop_back();
            labelMap[token] = pos;
            if (token == globl.name)
                globl.addr = pos;
        }else if (token == ".text"){
            if (!iss.eof()){
                iss >> token;
                pos = stoui(token);
            }
            sect = 0;
            section["text"] = pos;
        }else if (token == ".data"){
            if (!iss.eof()){
                iss >> token;
                pos = stoui(token);
            }
            sect = 2;
            section["data"] = pos;
        }else{
            switch(sect){
                case 0:
                    pos+=WORD;
                    break;
                case 2:
                    break;
            }
        }
        

    }
    inFile.close();
    inFile.open("../../test/test.s");

    while(getline(inFile, line)){
        istringstream iss(line);        
        iss >> token;

        if (line.empty()) continue;
        
        if (token == ".globl"){

        }else if (token.back() == ':'){
        }else if (token == ".text"){
            if (!iss.eof()){
                iss >> token;
                pos = stoui(token);
            }
            sect = 0;
        }else if (token == ".data"){
            if (!iss.eof()){
                iss >> token;
                pos = stoui(token);
            }
            sect = 2;
        }else{
            switch(sect){
                case 0:
                    opcode = token;
                    rs = "", rt = "", rd = "", funct = "000000";
                    shamt = 0, imm = 0, address = 0;
                    if (token == "nop"){
                        instruction = "00000000000000000000000000000000";
                    }
                    else if (opcodes[opcode] == "000000"){
                        // R-format
                        if (opcode == "jr"){
                            iss >> rs;
                        }else if (opcode == "sll" || opcode == "srl"){
                            iss >> rd >> rt >> token;
                            shamt = stoui(token);
                            rd.pop_back();
                            rt.pop_back();
                        }else{
                            iss >> rd >> rs >> rt;
                            rd.pop_back();
                            rs.pop_back();
                        }
                        instruction =
                            opcodes[opcode] + registerMap[rs] + registerMap[rt] + registerMap[rd] + shamt.to_string() + functCodes[opcode];
                    }else if (opcodes[opcode] == "000010" || opcodes[opcode] == "000011"){
                        // J-format
                        iss >> token;
                        address = stoui(token);
                        instruction =
                            opcodes[opcode] + address.to_string();
                    }
                    else{
                        // I-format or Undefined
                        if(opcode == "sw" || opcode == "lw"){
                            iss >> rt >> token;
                            rt.pop_back();
                            token.pop_back();
                            openParenPos = token.find('(');
                            imm = stoui(token.substr(0, openParenPos));
                            rs = token.substr(openParenPos+1);
                        }else if (opcode == "beq"){
                            iss >> rs >> rt >> token;
                            rs.pop_back();
                            rt.pop_back();
                            imm = stoui(token);
                        }else if (opcode == "lui"){
                            iss >> rs >> token;
                            rs.pop_back();
                            imm = stoui(token);
                        }else{
                            iss >> rt >> rs >> token;
                            rt.pop_back();
                            rs.pop_back();
                            imm = stoui(token);
                        }
                        instruction =
                            opcodes[opcode] + registerMap[rs] + registerMap[rt] + imm.to_string();
                    }
                    iMem.load(pos, stoul(instruction, nullptr, 2));
                    pos += WORD;
                    break;
                case 2:
                    break;
                default:
                    break;
                
            }
        }


    }
    inFile.close();
    outFile.close();
    
}

unsigned int stoui(const string& token){
    if (token.substr(0, 2) == "0x" || token.substr(0, 3) == "-0x" || token.substr(0, 2) == "0X" || token.substr(0, 3) == "-0X")
        return stoul(token, nullptr, 16);
    else if (labelMap.find(token) != labelMap.end()){
        return labelMap[token];
    }else
        return stoul(token, nullptr, 10);
}

