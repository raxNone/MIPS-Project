#include "../incs/main.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>

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
    cout << "pc : " << toHexString(IF.pc) << endl;
    cout << "$t0 : " << toHexString(reg[$t0]) << endl;
    cout << toHexString(MEM.write_data);
}
void pipeline(){
    //load
    ID.load(IFID);
    EX.load(IDEX);
    MEM.load(EXMEM);
    WB.load(MEMWB);
    
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
    ifstream inFile("../../test/test");
    if (not iMem.load(inFile)){
        cout << "FILE ERROR"<<endl;
        inFile.close();
        exit(-1);
    }
    inFile.close();
    while(input != 'q'){
        pipeline();
        print();
        cin.get(input);
    }
    
}