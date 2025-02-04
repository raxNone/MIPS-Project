#include "../incs/assemble.hpp"

void assemble()
{
    if(BYTE_ORDER != LITTLE_ENDIAN){
        cerr << "use x86 or ARM" << endl;
        exit(-1);
    }
    ifstream inFile("../../test/test.s");
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
    int openParenPos;
    
    // data segment
    string space;

    int sect = -1;
    uint32_t pos = 0;   // current addr
    
    // for word align
    string labelName;
    bool labelPosed = true;

    if(!inFile.is_open()){
        cerr << "FILE ERROR" << endl;
        exit(-1);
    }

    while(getline(inFile, line)){
        istringstream iss(line);
        iss >> token;

        if (line.find(':') != string::npos){
            labelPosed = false;
            if(token.back() == ':'){
                token.pop_back();
                labelName = token;
            }else{
                labelName = token;
                iss >> token;
            }
            iss >> token;
            if (empty(token))   continue;
        }
        if (line.empty()) continue;

        if (token == ".globl"){
            iss >> globl.name;
            globl.addr = &labelMap[globl.name];
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
                    if (!labelPosed){
                        labelPosed = true;
                        pos += pos%WORD;
                        labelMap[labelName] = pos;
                    }
                    pos += WORD;
                    break;
                case 2:
                    if (token == ".space"){
                        iss >> token;
                        if(!labelPosed){
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        space.resize(stoui(token), '\0');
                        dMem.load(pos, space);
                    }else if (token == ".byte"){
                        iss >> token;
                        if(!labelPosed){
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        if (token.back() == '\''){
                            dMem.load(pos, static_cast<uint8_t>(atoi(token.substr(1,1).c_str())));
                        }else
                            dMem.load(pos, static_cast<uint8_t>(stoui(token)));
                        pos += BYTE;
                    }else if (token == ".half"){
                        iss >> token;
                        if(!labelPosed){
                            pos += pos%HALF;
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        dMem.load(pos, static_cast<uint16_t>(stoui(token)));
                        pos += HALF;
                    }else if (token == ".word"){
                        iss >> token;
                        if(!labelPosed){
                            pos += pos%WORD;
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        dMem.load(pos, static_cast<uint32_t>(stoui(token)));
                        pos += WORD;
                    }else if (token == ".float"){
                        iss >> token;
                        if(!labelPosed){
                            pos += pos%FLOAT;
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        dMem.load(pos, static_cast<uint32_t>(stoui(token)));
                        pos += FLOAT;
                    }else if (token == ".double"){
                        iss >> token;
                        if(!labelPosed){
                            pos += pos%DOUBLE;
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        dMem.load(pos, static_cast<uint64_t>(stoui(token)));
                        pos += DOUBLE;
                    }else if (token == ".ascii"){
                        iss >> token;
                        if(!labelPosed){
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        dMem.load(pos, token.substr(1, token.size()-2));
                        pos += token.size()-2;
                    }else if (token == ".asciiz"){
                        iss >> token;
                        if(!labelPosed){
                            labelPosed = true;
                            labelMap[labelName] = pos;
                        }
                        dMem.load(pos, token.substr(1, token.size()-2)+"\0");
                        pos += token.size()-1;
                    }
                    break;
            }
        }
        
    }
    inFile.close();
    inFile.open("../../test/test.s");

    while(getline(inFile, line)){
        istringstream iss(line);        
        iss >> token;
        if (line.find(':') != string::npos){
            labelPosed = false;
            if(token.back() != ':'){
                iss >> token;
            }
            if (iss.eof())   continue;
        }
        if (line.empty()) continue;
        
        if (token == ".globl"){
            continue;
        }else if (token == ".text"){
            pos += pos%WORD;
            if (!iss.eof()){
                iss >> token;
                pos = stoui(token);
            }
            sect = 0;
        }else if (token == ".data"){
            pos += pos%WORD;
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
                    if (!labelPosed){
                        pos += pos%WORD;
                        labelPosed = true;
                    }
                    iMem.load(pos, stoul(instruction, nullptr, 2));
                    pos += WORD;
                    break;
                case 2:
                    if (token == ".space"){
                        if(!labelPosed){
                            labelPosed = true;
                        }
                        iss >> token;
                        pos += stoui(token);
                    }else if (token == ".byte"){
                        if (!labelPosed){
                            labelPosed = true;
                        }
                        pos += BYTE;
                    }else if (token == ".half"){
                        if (!labelPosed){
                            pos += pos%HALF;
                            labelPosed = true;
                        }
                        pos += HALF;
                    }else if (token == ".word"){
                        if (!labelPosed){
                            pos += pos%WORD;
                            labelPosed = true;
                        }
                        pos += WORD;
                    }else if (token == ".float"){
                        if (!labelPosed){
                            pos += pos%FLOAT;
                            labelPosed = true;
                        }
                        pos += FLOAT;
                    }else if (token == ".double"){
                        if (!labelPosed){
                            pos += pos%DOUBLE;
                            labelPosed = true;
                        }
                        pos += DOUBLE;
                    }else if (token == ".ascii"){
                        iss >> token;
                        if (!labelPosed){
                            labelPosed = true;
                        }
                        pos += token.size()-2;
                    }else if (token == ".asciiz"){
                        iss >> token;
                        if (!labelPosed){
                            labelPosed = true;
                        }
                        pos += token.size()-1;
                    }
                    break;
                default:
                    break;
                
            }
        }

    }
    inFile.close();

    pc = *globl.addr;
}

uint32_t stoui(const string& token){
    if (token.substr(0, 2) == "0x" || token.substr(0, 3) == "-0x" || token.substr(0, 2) == "0X" || token.substr(0, 3) == "-0X"){
        return stoul(token, nullptr, 16);
    }else if (labelMap.find(token) != labelMap.end()){
        return labelMap[token];
    }else{
        return stoul(token, nullptr, 10);
    }
}

