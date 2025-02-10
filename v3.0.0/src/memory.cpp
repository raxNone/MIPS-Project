#include "../incs/memory.hpp"

Memory::Memory(const string &file_name){
    mem.open(file_name, ios::in | ios::out | ios::binary);
    if(!mem.is_open())
        cerr << "FILE_ERROR" << endl;
}
Memory::~Memory(){
    mem.close();
}
void Memory::reset(){
    mem_size = 0;
    VtoP.clear();
    PtoV.clear();
}
bool Memory::valid_page(const uint32_t &page_addr)
{
    if (VtoP.find(page_addr) == VtoP.end()){
        char zeroes[_PAGE_SIZE] = {0,};
        VtoP[page_addr] = mem_size;
        PtoV[mem_size] = page_addr;
        mem.seekg(mem_size);
        mem.write(zeroes, _PAGE_SIZE);
        mem_size += _PAGE_SIZE;
        return false;
    }
    return true;
}
bitset<32> Memory::read(const bitset<32> &addr)
{
    if (!valid_page(addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE)){
        // cout << "pc : " <<hex<<setw(8)<<addr.to_ulong()<<endl;
        // cerr << "segment fault" << endl;
        // exit(-1);
        return 0;
    }
    uint32_t page = VtoP[addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE];
    uint32_t pa = page + addr.to_ulong()%_PAGE_SIZE;
    uint32_t value;
    mem.seekg(pa);
    mem.read(reinterpret_cast<char*>(&value), _WORD); // 4 byte read
    return value;
}
void Memory::write_BYTE(const bitset<32> &addr, const uint8_t &data){
    valid_page(addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE);
    uint32_t page = VtoP[addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE];
    uint32_t pa = page + addr.to_ulong()%_PAGE_SIZE;
    mem.seekp(pa);
    mem.write(reinterpret_cast<const char*>(&data), _BYTE);
}
void Memory::write_HALF(const bitset<32> &addr, const uint16_t &data){
    valid_page(addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE);
    uint32_t page = VtoP[addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE];
    uint32_t pa = page + addr.to_ulong()%_PAGE_SIZE;
    mem.seekp(pa);
    mem.write(reinterpret_cast<const char*>(&data), _HALF);
}
void Memory::write_WORD(const bitset<32> &addr, const uint32_t &data){
    valid_page(addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE);
    uint32_t page = VtoP[addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE];
    uint32_t pa = page + addr.to_ulong()%_PAGE_SIZE;
    mem.seekp(pa);
    mem.write(reinterpret_cast<const char*>(&data), _WORD);
}
void Memory::write_DOUBLE(const bitset<32> &addr, const uint64_t &data){
    valid_page(addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE);
    uint32_t page = VtoP[addr.to_ulong()/_PAGE_SIZE*_PAGE_SIZE];
    uint32_t pa = page + addr.to_ulong()%_PAGE_SIZE;
    mem.seekp(pa);
    mem.write(reinterpret_cast<const char*>(&data), _DOUBLE);
}

InstructionMemory::InstructionMemory(const string& file_name) : Memory(file_name){}
bitset<32> InstructionMemory::work(const bitset<32> &addr){
    return Memory::read(addr);
}
void InstructionMemory::load(const bitset<32>& addr, const uint32_t& data){
    Memory::write_WORD(addr, data);
}

DataMemory::DataMemory(const string& file_name) : Memory(file_name){}
bitset<32> DataMemory::work(const bitset<32>& addr, const bitset<32>& writeData, const bitset<1>& MemRead, const bitset<1>& MemWrite){
    if (MemWrite[0]){
        Memory::write_WORD(addr, static_cast<uint32_t>(writeData.to_ulong()));
    }
    if (MemRead[0]){
        return Memory::read(addr);
    }
    return 0;
}

void DataMemory::load(const bitset<32> &addr, const uint8_t data){
    Memory::write_BYTE(addr, data);
}
void DataMemory::load(const bitset<32> &addr, const uint16_t data){
    Memory::write_HALF(addr, data);
}
void DataMemory::load(const bitset<32> &addr, const uint32_t data){
    Memory::write_WORD(addr, data);
}
void DataMemory::load(const bitset<32> &addr, const uint64_t data){
    Memory::write_DOUBLE(addr, data);
}
void DataMemory::load(const bitset<32> &addr, const string data){
    for(int i=0; i<data.size(); i++){
        Memory::write_BYTE(addr.to_ulong()+i, static_cast<uint8_t>(data[i]));
    }
}
