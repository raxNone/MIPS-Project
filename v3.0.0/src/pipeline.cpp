#include "../incs/pipeline.hpp"
void IFID_Class::setFlush(bitset<1> flush){
    if(flush[0]){
        IFID.instruction = 0;
    }
}