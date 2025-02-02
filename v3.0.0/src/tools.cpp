#include "../incs/tools.hpp"

unsigned int subBit(const bitset<32>& bs, int high, int low) {
    return (static_cast<unsigned int>(bs.to_ulong()) >> low) & ((1U << (high - low + 1)) - 1);
}

void joinThread(thread& t){
    if(t.joinable())
        t.join();
}