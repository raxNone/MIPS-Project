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

void setSig(){

}

void print(){
   
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
        setSig();
        print();
        cin.get(input);
    }
    
}