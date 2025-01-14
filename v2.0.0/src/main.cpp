#include "../incs/main.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iostream>
using namespace std;

Register reg = Register();
InstructionMemory iMem = InstructionMemory();
DataMemory dMem = DataMemory();

_IF IF = _IF();
_ID ID = _ID();
_EX EX = _EX();
_MEM MEM = _MEM();
_WB WB = _WB();

IF_OUT IFID;
ID_OUT IDEX;
EX_OUT EXMEM;
MEM_OUT MEMWB;

void print(){
    // cout << "pc : " << toHexString(IF.pc) << endl;
}
void pipeline(){
    //upload

    ID.upload(IFID);
    EX.upload(IDEX);
    MEM.upload(EXMEM);
    WB.upload(MEMWB);
    
    //setSig
    ID.setSig();    //Control, Hazard
    EX.setSig();    //ALU_control, Forwarding

    //work
    IDEX = ID.work();
    IFID = IF.work();
    EXMEM = EX.work();
    MEMWB = MEM.work();
    WB.work();
    

}
int main(int argc, char **argv){
    char input = '\n';
    while(input != 'q'){
        pipeline();
        print();
        cin.get(input);
    }
    
}