#include "../incs/tools.hpp"

uint32_t subBit(const bitset<32>& bs, int high, int low) {
    return (static_cast<unsigned int>(bs.to_ulong()) >> low) & ((1U << (high - low + 1)) - 1);
}

void printHex(const bitset<32>& bs){
    cout << "0x" << hex << setw(8) << setfill('0') << bs.to_ulong() << dec << endl;
}

void joinThread(thread& t){
    lock_guard<mutex> lock(*thread_mutex_map[&t]);
    if(t.joinable())
        t.join();
}